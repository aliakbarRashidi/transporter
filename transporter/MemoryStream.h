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
