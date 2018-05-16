#pragma once

#include "Transporter.h"

#include <cstdint>

#include <string>

#include "Buffer.h"
#include "IByteInput.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			/**
			* @brief Interface class for data inputs capable of reading standard integer, floating point and string types
			*/
			class TRANSPORTER_DLL IDataInput : public transporter::data::io::IByteInput
			{
			public:
				IDataInput() noexcept = default;
				virtual ~IDataInput() noexcept = default;


				/**
				* @brief Read a signed 1-byte integer number from the underlying input
				*/
				virtual std::int8_t readInt8() noexcept = 0;

				/**
				* @brief Read a signed 2-byte integer number from the underlying input
				*/
				virtual std::int16_t readInt16() noexcept = 0;

				/**
				* @brief Read a signed 4-byte integer number from the underlying input
				*/
				virtual std::int32_t readInt32() noexcept = 0;

				/**
				* @brief Read a signed 8-byte integer number from the underlying input
				*/
				virtual std::int64_t readInt64() noexcept = 0;

				/**
				* @brief Read an unsigned 1-byte integer number from the underlying input
				*/
				virtual std::uint8_t readUInt8() noexcept = 0;

				/**
				* @brief Read an unsigned 2-byte integer number from the underlying input
				*/
				virtual std::uint16_t readUInt16() noexcept = 0;

				/**
				* @brief Read an unsigned 4-byte integer number from the underlying  input
				*/
				virtual std::uint32_t readUInt32() noexcept = 0;

				/**
				* @brief Read an unsigned 8-byte integer number from the underlying input
				*/
				virtual std::uint64_t readUInt64() noexcept = 0;

				/**
				* @brief Read a single-precision floating point number from the underlying input
				*/
				virtual float readFloat() noexcept = 0;

				/**
				* @brief Read a double-precision floating point number from the underlying input
				*/
				virtual double readDouble() noexcept = 0;

				/**
				* @brief Read a string of characters prefixed with its size (as an unsigned 4-byte integer number) from the underlying input
				*/
				virtual std::string readString() noexcept = 0;
			};
		}
	}
}
