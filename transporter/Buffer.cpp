#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

#include "Buffer.h"

#include <algorithm>

transporter::data::Buffer::Buffer() noexcept :
m_buffer{},
m_bufferSize{ 0 }
{
}

transporter::data::Buffer::Buffer(std::size_t size) :
m_buffer{ std::make_unique<char[]>(size) },
m_bufferSize{ size }
{
}

transporter::data::Buffer::Buffer(const transporter::data::Buffer &other) : Buffer{}
{
	this->concatenate(other);
}

transporter::data::Buffer::Buffer(transporter::data::Buffer &&other) noexcept :
m_buffer{ std::move(other.m_buffer) },
m_bufferSize{ other.m_bufferSize }
{
	other.reset();
}

transporter::data::Buffer::Buffer(const char *buffer, std::size_t bufferSize) :
m_buffer{},
m_bufferSize{ bufferSize }
{
	m_buffer = std::make_unique<char[]>(bufferSize);
	std::copy(buffer, buffer + bufferSize, m_buffer.get());
}

transporter::data::Buffer::Buffer(std::unique_ptr<char[]> &&buffer, std::size_t bufferSize) noexcept :
m_buffer{ std::move(buffer) },
m_bufferSize{ bufferSize }
{
}


transporter::data::Buffer &transporter::data::Buffer::operator=(const transporter::data::Buffer &rhs)
{
	this->reset();
	this->concatenate(rhs);

	return *this;
}

transporter::data::Buffer &transporter::data::Buffer::operator=(transporter::data::Buffer &&rhs) noexcept
{
	m_buffer = std::move(rhs.m_buffer);
	m_bufferSize = rhs.m_bufferSize;

	rhs.reset();

	return *this;
}


transporter::data::Buffer transporter::data::Buffer::operator+(const transporter::data::Buffer &rhs)
{
	Buffer buffer{ *this };

	buffer.concatenate(rhs);

	return buffer;
}

transporter::data::Buffer &transporter::data::Buffer::operator+=(const transporter::data::Buffer &rhs)
{
	this->concatenate(rhs);

	return *this;
}


bool transporter::data::Buffer::operator==(const transporter::data::Buffer &rhs) noexcept
{
	if (m_bufferSize != rhs.m_bufferSize)
	{
		return false;
	}

	return std::equal(m_buffer.get(), m_buffer.get() + m_bufferSize, rhs.m_buffer.get(), rhs.m_buffer.get() + m_bufferSize);
}


char transporter::data::Buffer::operator[](std::size_t index) const
{
	if (index < m_bufferSize)
	{
		return m_buffer[index];
	}

	throw std::out_of_range("index is out of buffer range");
}

char &transporter::data::Buffer::operator[](std::size_t index)
{
	if (index < m_bufferSize)
	{
		return m_buffer[index];
	}

	throw std::out_of_range("index is out of buffer range");
}


const char *transporter::data::Buffer::getRawBuffer() const noexcept
{
	return m_buffer.get();
}

char *transporter::data::Buffer::getRawBuffer() noexcept
{
	return m_buffer.get();
}

std::size_t transporter::data::Buffer::getSize() const noexcept
{
	return m_bufferSize;
}

transporter::data::BufferPtr transporter::data::Buffer::getSlice(std::size_t beginning, std::size_t length) const
{
	if (beginning + length > beginning)
	{
		if (beginning + length <= m_bufferSize)
		{
			transporter::data::BufferPtr sliceBuffer{ new data::Buffer{ length } };

			if (m_buffer)
			{
				std::copy(m_buffer.get() + beginning, m_buffer.get() + beginning + length, sliceBuffer->m_buffer.get());
			}

			return sliceBuffer;
		}

		else
		{
			throw std::invalid_argument("beginning + length exceeds buffer size");
		}
	}

	else
	{
		throw std::invalid_argument("beginning + length overflows std::size_t");
	}
}


char *transporter::data::Buffer::release() noexcept
{
	char *rawBuffer = m_buffer.release();

	this->reset();

	return rawBuffer;
}

void transporter::data::Buffer::extend(std::size_t increase)
{
	if (m_bufferSize + increase > m_bufferSize)
	{
		std::unique_ptr<char[]> newBuffer = std::make_unique<char[]>(m_bufferSize + increase);

		if (m_buffer)
		{
			std::copy(m_buffer.get(), m_buffer.get() + m_bufferSize, newBuffer.get());
		}

		m_buffer = std::move(newBuffer);
		m_bufferSize += increase;
	}

	else if (increase != 0)
	{
		throw std::overflow_error("Buffers sizes overflow std::size_t");
	}
}

void transporter::data::Buffer::insert(const transporter::data::Buffer &buffer, std::size_t at)
{
	if (at <= m_bufferSize)
	{
		if (m_bufferSize + buffer.getSize() > m_bufferSize)
		{
			std::unique_ptr<char[]> newBuffer = std::make_unique<char[]>(m_bufferSize + buffer.getSize());
			std::size_t endLength = m_bufferSize - at;

			if (at > 0)
			{
				std::copy(m_buffer.get(), m_buffer.get() + at, newBuffer.get());
			}

			std::copy(buffer.getRawBuffer(), buffer.getRawBuffer() + buffer.getSize(), newBuffer.get() + at);

			if (endLength > 0)
			{
				std::copy(m_buffer.get() + at, m_buffer.get() + at + endLength, newBuffer.get() + at + buffer.getSize());
			}
			
			m_buffer = std::move(newBuffer);
			m_bufferSize += buffer.getSize();
		}

		else
		{
			throw std::overflow_error("Buffers sizes overflow std::size_t");
		}
	}

	else
	{
		throw std::invalid_argument("at parameter is bigger than buffer size");
	}
}

void transporter::data::Buffer::shrinkEnd(std::size_t decrease)
{
	if (decrease <= m_bufferSize && decrease > 0)
	{
		std::unique_ptr<char[]> newBuffer = std::make_unique<char[]>(m_bufferSize - decrease);

		if (m_buffer)
		{
			std::copy(m_buffer.get(), m_buffer.get() + (m_bufferSize - decrease), newBuffer.get());
		}

		m_buffer = std::move(newBuffer);
		m_bufferSize -= decrease;
	}

	else if (decrease != 0)
	{
		throw std::length_error("decrease value is bigger than current buffer size");
	}
}

void transporter::data::Buffer::shrinkBeginning(std::size_t decrease)
{
	if (decrease <= m_bufferSize && decrease > 0)
	{
		std::unique_ptr<char[]> newBuffer = std::make_unique<char[]>(m_bufferSize - decrease);

		if (m_buffer)
		{
			std::copy(m_buffer.get() + decrease, m_buffer.get() + m_bufferSize, newBuffer.get());
		}

		m_buffer = std::move(newBuffer);
		m_bufferSize -= decrease;
	}

	else if (decrease != 0)
	{
		throw std::length_error("decrease value is bigger than current buffer size");
	}
}

void transporter::data::Buffer::reverse() const noexcept
{
	std::reverse(m_buffer.get(), m_buffer.get() + m_bufferSize);
}

transporter::data::BufferPtr transporter::data::Buffer::slice(std::size_t beginning, std::size_t length)
{
	if (beginning + length > beginning)
	{
		if (beginning + length <= m_bufferSize)
		{
			transporter::data::BufferPtr sliceBuffer{ new data::Buffer{ length } };
			std::unique_ptr<char[]> newBuffer = std::make_unique<char[]>(m_bufferSize - length);

			if (m_buffer)
			{
				std::copy(m_buffer.get() + beginning, m_buffer.get() + beginning + length, sliceBuffer->m_buffer.get());

				if (beginning > 0)
				{
					std::copy(m_buffer.get(), m_buffer.get() + beginning, newBuffer.get());
				}

				if (beginning + length < m_bufferSize)
				{
					std::copy(m_buffer.get() + beginning + length, m_buffer.get() + m_bufferSize, newBuffer.get() + beginning);
				}

				m_buffer = std::move(newBuffer);
				m_bufferSize -= length;
			}

			return sliceBuffer;
		}

		else
		{
			throw std::invalid_argument("beginning + length exceeds buffer size");
		}
	}

	else
	{
		throw std::invalid_argument("beginning + length overflows std::size_t");
	}
}


void transporter::data::Buffer::concatenate(const transporter::data::Buffer &buffer)
{
	if (m_bufferSize + buffer.m_bufferSize > m_bufferSize)
	{
		std::unique_ptr<char[]> newBuffer = std::make_unique<char[]>(m_bufferSize + buffer.m_bufferSize);

		if (m_buffer)
		{
			std::copy(m_buffer.get(), m_buffer.get() + m_bufferSize, newBuffer.get());
		}

		std::copy(buffer.m_buffer.get(), buffer.m_buffer.get() + buffer.m_bufferSize, newBuffer.get() + m_bufferSize);

		m_buffer = std::move(newBuffer);
		m_bufferSize += buffer.m_bufferSize;
	}

	else if (buffer.m_bufferSize != 0)
	{
		throw std::overflow_error("Buffers sizes overflow std::size_t");
	}
}

void transporter::data::Buffer::reset() noexcept
{
	m_buffer.reset();
	m_bufferSize = 0;
}
