#pragma once

#include "Transporter.h"

#include <cstdint>

#include <string>

#include "Buffer.h"
#include "IByteInput.h"
#include "ssize_t.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			class TRANSPORTER_DLL IDataInput : public transporter::data::io::IByteInput
			{
			public:
				IDataInput() noexcept = default;
				virtual ~IDataInput() noexcept = default;

				virtual std::int8_t readInt8() noexcept = 0;
				virtual std::int16_t readInt16() noexcept = 0;
				virtual std::int32_t readInt32() noexcept = 0;
				virtual std::int64_t readInt64() noexcept = 0;
				virtual std::uint8_t readUInt8() noexcept = 0;
				virtual std::uint16_t readUInt16() noexcept = 0;
				virtual std::uint32_t readUInt32() noexcept = 0;
				virtual std::uint64_t readUInt64() noexcept = 0;
				virtual float readFloat() noexcept = 0;
				virtual double readDouble() noexcept = 0;
				virtual std::string readString() noexcept = 0;
			};
		}
	}
}
