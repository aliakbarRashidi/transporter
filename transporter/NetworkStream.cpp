#include "NetworkStream.h"

transporter::network::io::NetworkStream::NetworkStream(data::io::IBytesStream &stream) noexcept : IDataInput{}, IDataOutput{},
m_stream{ stream },
m_readBuffer{},
m_transactionBackupReadBuffer{},
m_transaction{ false },
m_transactionReadPastEnd{ false }
{
}


bool transporter::network::io::NetworkStream::startReadTransaction() noexcept
{
	if (m_transaction)
	{
		return false;
	}

	m_transaction = true;
	
	return true;
}

bool transporter::network::io::NetworkStream::commitReadTransaction() noexcept
{
	if (!m_transaction || m_transactionReadPastEnd)
	{
		return false;
	}

	m_transactionBackupReadBuffer = data::Buffer{};
	m_transaction = false;

	return true;
}

bool transporter::network::io::NetworkStream::rollbackReadTransaction() noexcept
{
	if (!m_transaction)
	{
		return false;
	}

	m_readBuffer = m_transactionBackupReadBuffer + m_readBuffer;
	m_transactionBackupReadBuffer = data::Buffer{};
	m_transaction = false;
	m_transactionReadPastEnd = false;

	return true;
}


std::unique_ptr<transporter::data::Buffer> transporter::network::io::NetworkStream::readBytes(std::size_t count) noexcept
{
	std::unique_ptr<data::Buffer> buffer{};

	if (count > m_readBuffer.getSize())
	{
		std::unique_ptr<data::Buffer> buffer = m_stream.readBytes(count > READ_BLOCK_SIZE ? count : READ_BLOCK_SIZE);

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
					if (m_transaction)
					{
						m_transactionReadPastEnd = true;
					}

					return nullptr;
				}
			}
		}

		else if (m_readBuffer.getSize() == 0)
		{
			if (m_transaction)
			{
				m_transactionReadPastEnd = true;
			}

			return nullptr;
		}
	}

	if (m_transaction && count > m_readBuffer.getSize())
	{
		m_transactionReadPastEnd = true;
	}

	count = count <= m_readBuffer.getSize() ? count : m_readBuffer.getSize();

	try
	{
		buffer = m_readBuffer.slice(0, count);

		if (m_transaction)
		{
			m_transactionBackupReadBuffer += *buffer;
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
	return m_stream.writeBytes(buffer);
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
		std::unique_ptr<data::Buffer> buffer = this->readBytes(length);

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
