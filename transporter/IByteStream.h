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
			/**
			* @brief Interface class for bi-directional byte streams
			*/
			class TRANSPORTER_DLL IByteStream : public transporter::data::io::IByteInput, public transporter::data::io::IByteOutput
			{
			public:
				IByteStream() noexcept = default;
				virtual ~IByteStream() noexcept = default;
			};
		}
	}
}
