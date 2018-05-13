#pragma once

#include "Transporter.h"

#include "IByteInput.h"
#include "IByteOutput.h"

namespace transporter
{
	namespace data
	{
		namespace io
		{
			class TRANSPORTER_DLL IByteStream : public transporter::data::io::IByteInput, public transporter::data::io::IByteOutput
			{
			public:
				IByteStream() noexcept = default;
				virtual ~IByteStream() noexcept = default;
			};
		}
	}
}
