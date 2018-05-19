#include "MemoryStream.h"

#include <limits>

transporter::data::io::MemoryStream::MemoryStream() noexcept :
m_buffer{},
m_position{ 0 }
{
}


transporter::data::BufferPtr transporter::data::io::MemoryStream::readBytes(std::size_t count) noexcept
{
	count = count <= m_buffer.getSize() ? count : m_buffer.getSize();

	try
	{
		transporter::data::BufferPtr slice = m_buffer.getSlice(m_position, count);

		m_position += count;

		return slice;
	}

	catch (...)
	{
		return nullptr;
	}
}

ssize_t transporter::data::io::MemoryStream::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	if (buffer.getSize() <= std::numeric_limits<ssize_t>::max())
	{
		try
		{
			m_buffer.insert(buffer, m_position);
			m_position += buffer.getSize();

			return buffer.getSize();
		}

		catch (...)
		{
			return -1;
		}
	}

	else
	{
		return -1;
	}
}


std::size_t transporter::data::io::MemoryStream::getPosition() const noexcept
{
	return m_position;
}

std::size_t transporter::data::io::MemoryStream::getLength() const noexcept
{
	return m_buffer.getSize();
}


void transporter::data::io::MemoryStream::setPosition(std::size_t pos) throw(std::invalid_argument)
{
	if (pos <= m_buffer.getSize())
	{
		m_position = pos;
	}

	else
	{
		throw std::invalid_argument("pos is greater than stream size");
	}
}
