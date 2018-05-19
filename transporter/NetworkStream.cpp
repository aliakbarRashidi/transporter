/*
* MIT License
*
* Copyright (c) 2018 Guillaume Truchot - guillaume.truchot@outlook.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "NetworkStream.h"

#include <limits>

#include "UnknownMessage.h"

transporter::network::io::NetworkStream::NetworkStream(data::io::IByteStream &stream) noexcept : IDataInput{}, IDataOutput{},
m_stream{ stream },
m_readBuffer{},
m_readTransactionBackupBuffer{},
m_writeTransctionBuffer{},
m_readTransaction{ false },
m_readTransactionError{ false },
m_writeTransaction{ false },
m_writeTransactionError{ false }
{
}


void transporter::network::io::NetworkStream::sendMessage(const transporter::network::messages::INetworkMessage &message)
{
	if (this->startWriteTransaction())
	{
		transporter::data::Buffer msgData{};

		message.serialize(*this);

		msgData = m_writeTransctionBuffer;
		m_writeTransctionBuffer = data::Buffer{};

		this->writeInt32(message.getMessageId());
		this->writeUInt32(msgData.getSize());
		m_writeTransctionBuffer += msgData;

		if (!this->commitWriteTransaction())
		{
			this->rollbackWriteTransaction();

			throw transporter::exceptions::TransactionException{ "Unable to commit transaction" };
		}
	}
	
	else
	{
		throw transporter::exceptions::TransactionException{ "Can't start write transaction: already started" };
	}
}

transporter::network::messages::NetworkMessagePtr transporter::network::io::NetworkStream::receiveMessage(const transporter::network::io::NetworkMessageSelector &selector)
{
	if (this->startReadTransaction())
	{
		transporter::network::messages::NetworkMessageId msgId = this->readInt32();
		std::uint32_t msgDataSize = this->readUInt32();
		transporter::network::messages::NetworkMessagePtr msg = selector(msgId);

		if (msg == nullptr)
		{
			msg = std::make_unique<transporter::network::messages::UnknownMessage>(msgId, msgDataSize);
		}

		msg->deserialize(*this);

		if (this->commitReadTransaction())
		{
			return msg;
		}

		return nullptr;
	}

	else
	{
		throw transporter::exceptions::TransactionException{ "Can't start read transaction: already started" };
	}
}


bool transporter::network::io::NetworkStream::startReadTransaction() noexcept
{
	if (m_readTransaction)
	{
		return false;
	}

	m_readTransaction = true;
	
	return true;
}

bool transporter::network::io::NetworkStream::commitReadTransaction() noexcept
{
	if (!m_readTransaction || m_readTransactionError)
	{
		return false;
	}

	m_readTransactionBackupBuffer = data::Buffer{};
	m_readTransaction = false;

	return true;
}

bool transporter::network::io::NetworkStream::rollbackReadTransaction() noexcept
{
	if (!m_readTransaction)
	{
		return false;
	}

	m_readBuffer = m_readTransactionBackupBuffer + m_readBuffer;
	m_readTransactionBackupBuffer = data::Buffer{};
	m_readTransaction = false;
	m_readTransactionError = false;

	return true;
}


bool transporter::network::io::NetworkStream::startWriteTransaction() noexcept
{
	if (m_writeTransaction)
	{
		return false;
	}

	m_writeTransaction = true;

	return true;
}

bool transporter::network::io::NetworkStream::commitWriteTransaction() noexcept
{
	if (!m_writeTransaction || m_writeTransactionError)
	{
		return false;
	}


	if (m_stream.writeBytes(m_writeTransctionBuffer) == m_writeTransctionBuffer.getSize())
	{
		m_writeTransctionBuffer = data::Buffer{};
		m_writeTransaction = false;

		return true;
	}

	else
	{
		return false;
	}
}

bool transporter::network::io::NetworkStream::rollbackWriteTransaction() noexcept
{
	if (!m_writeTransaction)
	{
		return false;
	}

	m_writeTransctionBuffer = data::Buffer{};
	m_writeTransaction = false;
	m_writeTransactionError = false;

	return true;
}


transporter::data::BufferPtr transporter::network::io::NetworkStream::readBytes(std::size_t count) noexcept
{
	transporter::data::BufferPtr buffer{};

	if (count > m_readBuffer.getSize())
	{
		transporter::data::BufferPtr buffer = m_stream.readBytes(count > READ_BLOCK_SIZE ? count : READ_BLOCK_SIZE);

		if (buffer)
		{
			try
			{
				m_readBuffer += *buffer;
			}

			catch (...)
			{
				if (m_readBuffer.getSize() == 0)
				{
					if (m_readTransaction)
					{
						m_readTransactionError = true;
					}

					return nullptr;
				}
			}
		}

		else if (m_readBuffer.getSize() == 0)
		{
			if (m_readTransaction)
			{
				m_readTransactionError = true;
			}

			return nullptr;
		}
	}

	if (m_readTransaction && count > m_readBuffer.getSize())
	{
		m_readTransactionError = true;
	}

	count = count <= m_readBuffer.getSize() ? count : m_readBuffer.getSize();

	try
	{
		buffer = m_readBuffer.slice(0, count);

		if (m_readTransaction)
		{
			m_readTransactionBackupBuffer += *buffer;
		}

		return buffer;
	}

	catch (...)
	{
		return nullptr;
	}
}

ssize_t transporter::network::io::NetworkStream::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	if (m_writeTransaction)
	{
		try
		{
			m_writeTransctionBuffer += buffer;
			
			return buffer.getSize();
		}

		catch (...)
		{
			m_writeTransactionError = true;
			return -1;
		}
	}

	else
	{
		return m_stream.writeBytes(buffer);
	}
}


std::int8_t transporter::network::io::NetworkStream::readInt8() noexcept
{
	return this->readInt<std::int8_t>();
}

std::int16_t transporter::network::io::NetworkStream::readInt16() noexcept
{
	return this->readInt<std::int16_t>();
}

std::int32_t transporter::network::io::NetworkStream::readInt32() noexcept
{
	return this->readInt<std::int32_t>();
}

std::int64_t transporter::network::io::NetworkStream::readInt64() noexcept
{
	return this->readInt<std::int64_t>();
}

std::uint8_t transporter::network::io::NetworkStream::readUInt8() noexcept
{
	return this->readInt<std::int8_t>();
}

std::uint16_t transporter::network::io::NetworkStream::readUInt16() noexcept
{
	return this->readInt<std::uint16_t>();
}

std::uint32_t transporter::network::io::NetworkStream::readUInt32() noexcept
{
	return this->readInt<std::uint32_t>();
}

std::uint64_t transporter::network::io::NetworkStream::readUInt64() noexcept
{
	return this->readInt<std::uint64_t>();
}

float transporter::network::io::NetworkStream::readFloat() noexcept
{
	return this->readData<float>();
}

double transporter::network::io::NetworkStream::readDouble() noexcept
{
	return this->readData<double>();
}

std::string transporter::network::io::NetworkStream::readString() noexcept
{
	std::size_t length = this->readUInt32();

	if (length > 0)
	{
		transporter::data::BufferPtr buffer = this->readBytes(length);

		if (buffer && buffer->getSize() == length)
		{
			return std::string{ buffer->getRawBuffer(), length };
		}
	}

	return std::string{};
}


void transporter::network::io::NetworkStream::writeInt8(std::int8_t value) noexcept
{
	this->writeInt<std::int8_t>(value);
}

void transporter::network::io::NetworkStream::writeInt16(std::int16_t value) noexcept
{
	this->writeInt<std::int16_t>(value);
}

void transporter::network::io::NetworkStream::writeInt32(std::int32_t value) noexcept
{
	this->writeInt<std::int32_t>(value);
}

void transporter::network::io::NetworkStream::writeInt64(std::int64_t value) noexcept
{
	this->writeInt<std::int64_t>(value);
}

void transporter::network::io::NetworkStream::writeUInt8(std::uint8_t value) noexcept
{
	this->writeInt<std::uint8_t>(value);
}

void transporter::network::io::NetworkStream::writeUInt16(std::uint16_t value) noexcept
{
	this->writeInt<std::uint16_t>(value);
}

void transporter::network::io::NetworkStream::writeUInt32(std::uint32_t value) noexcept
{
	this->writeInt<std::uint32_t>(value);
}

void transporter::network::io::NetworkStream::writeUInt64(std::uint64_t value) noexcept
{
	this->writeInt<std::uint64_t>(value);
}

void transporter::network::io::NetworkStream::writeFloat(float value) noexcept
{
	this->writeData(value);
}

void transporter::network::io::NetworkStream::writeDouble(double value) noexcept
{
	this->writeData(value);
}

void transporter::network::io::NetworkStream::writeString(const std::string &value) noexcept
{
	std::size_t strLen = value.length();
	data::Buffer strBuffer{ value.c_str(), strLen };

	this->writeUInt32(strLen);
	this->writeBytes(strBuffer);
}


constexpr bool transporter::network::io::NetworkStream::isBigEndian() noexcept
{
	static const uint16_t dummy{ 0x00FF };
	return reinterpret_cast<const uint8_t*>(&dummy)[1] == 0xFF ? true : false;
}
