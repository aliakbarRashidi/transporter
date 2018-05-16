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
				virtual void writeInt8(std::int8_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write a signed 2-byte integer number to the underlying output
				*/
				virtual void writeInt16(std::int16_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write a signed 4-byte integer number to the underlying output
				*/
				virtual void writeInt32(std::int32_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write a signed 8-byte integer number to the underlying output
				*/
				virtual void writeInt64(std::int64_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write an unsigned 1-byte integer number to the underlying output
				*/
				virtual void writeUInt8(std::uint8_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write an unsigned 2-byte integer number to the underlying output
				*/
				virtual void writeUInt16(std::uint16_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write an unsigned 4-byte integer number to the underlying output
				*/
				virtual void writeUInt32(std::uint32_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write an unsigned 8-byte integer number to the underlying output
				*/
				virtual void writeUInt64(std::uint64_t value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write a single-precision floating point number to the underlying output
				*/
				virtual void writeFloat(float value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write a double-precision floating point number to the underlying output
				*/
				virtual void writeDouble(double value) throw(std::bad_alloc) = 0;

				/**
				* @brief Write a string of characters prefixed with its size (as an unsigned 4-byte integer number) to the underlying output
				*/
				virtual void writeString(const std::string &value) throw(std::bad_alloc) = 0;
			};
		}
	}
}
