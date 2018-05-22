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

#include <cstdint>

#include "Buffer.h"
#include "ssize_t.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			/**
			* @brief Interface class for byte inputs
			*/
			class TRANSPORTER_DLL IByteInput
			{
			public:
				IByteInput() noexcept = default;
				IByteInput(const IByteInput&) = delete;
				IByteInput(IByteInput&&) = delete;
				virtual ~IByteInput() noexcept = default;

				/**
				* @brief Attempt to read \p count bytes from the underlying input and return them in a pointer to a newly-constructed transporter::data::Buffer
				*
				* @details The returned buffer is:
				* - At most \p count bytes, less if not enough bytes were available (including 0)
				* - \c nullptr if there was an error
				*
				* @param count Number of bytes to read
				*
				* @return Pointer to newly-constructed transporter::data::Buffer containing the read bytes
				*/
				virtual transporter::data::BufferPtr readBytes(std::size_t count) noexcept = 0;
			};
		}
	}
}
