#pragma once

#include "Transporter.h"

#include "IBytesInput.h"
#include "IBytesOutput.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			class TRANSPORTER_DLL IBytesStream : public transporter::data::io::IBytesInput, public transporter::data::io::IBytesOutput
			{
			public:
				IBytesStream() noexcept = default;
				virtual ~IBytesStream() noexcept = default;
			};
		}
	}
}
