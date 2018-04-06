#pragma once

#include "Transporter.h"

#include "Buffer.h"
#include "IBytesStream.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			class TRANSPORTER_DLL MemoryStream : public transporter::data::io::IBytesStream
			{
			public:
				MemoryStream() noexcept;
				~MemoryStream() noexcept = default;

				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;

				std::size_t getPosition() const noexcept;
				std::size_t getLength() const noexcept;

				void setPosition(std::size_t pos) throw(std::invalid_argument);


			private:
				Buffer m_buffer;
				std::size_t m_position;
			};
		}
	}
}
