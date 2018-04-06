#pragma once

#include "Transporter.h"

#include <cstddef>

#include <stdexcept>

#include "Buffer.h"
#include "ssize_t.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			class TRANSPORTER_DLL IBytesInput
			{
			public:
				IBytesInput() noexcept = default;
				virtual ~IBytesInput() noexcept = default;

				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept = 0;
			};
		}
	}
}
