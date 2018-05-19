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
#pragma once

#include "Transporter.h"

#include "Buffer.h"
#include "IByteStream.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			/**
			* @brief Byte stream using transporter::data::Buffer as storage
			*/
			class TRANSPORTER_DLL MemoryStream : public transporter::data::io::IByteStream
			{
			public:
				MemoryStream() noexcept;
				~MemoryStream() noexcept = default;

				virtual transporter::data::BufferPtr readBytes(std::size_t count) noexcept override;
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;


				/**
				* @brief Get the current position of this stream
				*
				* @return Current position of this stream
				*/
				std::size_t getPosition() const noexcept;

				/**
				* @brief Get the current length of this stream
				*
				* @return Current length of this stream
				*/
				std::size_t getLength() const noexcept;


				/**
				* @brief Set the position of this stream to \p pos
				*
				* @param pos Position to set this stream to
				*/
				void setPosition(std::size_t pos);


			private:
				transporter::data::Buffer m_buffer;
				std::size_t m_position;
			};
		}
	}
}
