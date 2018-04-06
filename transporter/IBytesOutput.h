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
			class TRANSPORTER_DLL IBytesOutput
			{
			public:
				IBytesOutput() noexcept = default;
				virtual ~IBytesOutput() noexcept = default;

				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept = 0;
			};
		}
	}
}
