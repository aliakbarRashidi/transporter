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
			class INetworkMessage;

			typedef std::int32_t NetworkMessageId;
			typedef std::unique_ptr<transporter::network::messages::INetworkMessage> NetworkMessagePtr;

			/**
			* @brief Interface class for all network messages
			*/
			class TRANSPORTER_DLL INetworkMessage
			{
			public:
				virtual ~INetworkMessage() = default;

				/**
				* @brief Get the numeric identifier (ID) of this network message
				*
				* @return Identifier of the message
				*/
				virtual transporter::network::messages::NetworkMessageId getMessageId() const noexcept = 0;


				/**
				* @brief Serialize this network message into \p output
				*/
				virtual void serialize(transporter::data::io::IDataOutput &output) const noexcept = 0;

				/**
				* @brief Deserialize this network message from \p input
				*/
				virtual void deserialize(transporter::data::io::IDataInput &input) noexcept = 0;
			};
		}
	}
}
