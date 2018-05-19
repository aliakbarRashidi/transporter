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

#include <string>

#include "Buffer.h"
#include "IByteOutput.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			/**
			* @brief Interface class for data outputs capable of writing standard integer, floating point and string types
			*/
			class TRANSPORTER_DLL IDataOutput : public transporter::data::io::IByteOutput
			{
			public:
				IDataOutput() noexcept = default;
				virtual ~IDataOutput() noexcept = default;


				/**
				* @brief Write a signed 1-byte integer number to the underlying output
				*/
				virtual void writeInt8(std::int8_t value) = 0;

				/**
				* @brief Write a signed 2-byte integer number to the underlying output
				*/
				virtual void writeInt16(std::int16_t value) = 0;

				/**
				* @brief Write a signed 4-byte integer number to the underlying output
				*/
				virtual void writeInt32(std::int32_t value) = 0;

				/**
				* @brief Write a signed 8-byte integer number to the underlying output
				*/
				virtual void writeInt64(std::int64_t value) = 0;

				/**
				* @brief Write an unsigned 1-byte integer number to the underlying output
				*/
				virtual void writeUInt8(std::uint8_t value) = 0;

				/**
				* @brief Write an unsigned 2-byte integer number to the underlying output
				*/
				virtual void writeUInt16(std::uint16_t value) = 0;

				/**
				* @brief Write an unsigned 4-byte integer number to the underlying output
				*/
				virtual void writeUInt32(std::uint32_t value) = 0;

				/**
				* @brief Write an unsigned 8-byte integer number to the underlying output
				*/
				virtual void writeUInt64(std::uint64_t value) = 0;

				/**
				* @brief Write a single-precision floating point number to the underlying output
				*/
				virtual void writeFloat(float value) = 0;

				/**
				* @brief Write a double-precision floating point number to the underlying output
				*/
				virtual void writeDouble(double value) = 0;

				/**
				* @brief Write a string of characters prefixed with its size (as an unsigned 4-byte integer number) to the underlying output
				*/
				virtual void writeString(const std::string &value) = 0;
			};
		}
	}
}
