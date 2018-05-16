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
				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept = 0;
			};
		}
	}
}
