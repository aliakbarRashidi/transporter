#pragma once

#include "Transporter.h"

#include <cstdint>

#include <string>

#include "Buffer.h"
#include "IByteOutput.h"
#include "ssize_t.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			class TRANSPORTER_DLL IDataOutput : public transporter::data::io::IByteOutput
			{
			public:
				IDataOutput() noexcept = default;
				virtual ~IDataOutput() noexcept = default;

				virtual void writeInt8(std::int8_t value) throw(std::bad_alloc) = 0;
				virtual void writeInt16(std::int16_t value) throw(std::bad_alloc) = 0;
				virtual void writeInt32(std::int32_t value) throw(std::bad_alloc) = 0;
				virtual void writeInt64(std::int64_t value) throw(std::bad_alloc) = 0;
				virtual void writeUInt8(std::uint8_t value) throw(std::bad_alloc) = 0;
				virtual void writeUInt16(std::uint16_t value) throw(std::bad_alloc) = 0;
				virtual void writeUInt32(std::uint32_t value) throw(std::bad_alloc) = 0;
				virtual void writeUInt64(std::uint64_t value) throw(std::bad_alloc) = 0;
				virtual void writeFloat(float value) throw(std::bad_alloc) = 0;
				virtual void writeDouble(double value) throw(std::bad_alloc) = 0;
				virtual void writeString(const std::string &value) throw(std::bad_alloc) = 0;
			};
		}
	}
}
