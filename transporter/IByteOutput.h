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
