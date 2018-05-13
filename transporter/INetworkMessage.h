#pragma once

#include "Transporter.h"

#include <cstdint>

#include <memory>

#include "IDataInput.h"
#include "IDataOutput.h"

namespace transporter
{
	namespace network
	{
		namespace messages
		{
			typedef std::int32_t NetworkMessageId;
			
			class TRANSPORTER_DLL INetworkMessage
			{
			public:
				virtual ~INetworkMessage() = default;

				virtual transporter::network::messages::NetworkMessageId getMessageId() const noexcept = 0;

				virtual void serialize(transporter::data::io::IDataOutput &output) const noexcept = 0;
				virtual void deserialize(transporter::data::io::IDataInput &input) noexcept = 0;
			};

			typedef std::unique_ptr<transporter::network::messages::INetworkMessage> NetworkMessagePtr;
		}
	}
}
