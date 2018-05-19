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
#include "ssize_t.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			/**
			* @brief Interface class for byte outputs
			*/
			class TRANSPORTER_DLL IByteOutput
			{
			public:
				IByteOutput() noexcept = default;
				virtual ~IByteOutput() noexcept = default;

				/**
				* @brief Attempt to write the bytes in \p buffer to the underlying output and return the number of bytes written
				*
				* @details The returned number of bytes written is:
				* - At most the size of \p buffer, less if not everything could be written (including 0)
				* - \c -1 if there was an error
				*
				* @param buffer Reference to the buffer containing the bytes to write
				*
				* @return Number of bytes written or -1 in case of error
				*/
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept = 0;
			};
		}
	}
}
