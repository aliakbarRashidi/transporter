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
	if (buffer.getSize() <= static_cast<std::size_t>(std::numeric_limits<ssize_t>::max()))
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


void transporter::data::io::MemoryStream::setPosition(std::size_t pos)
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
