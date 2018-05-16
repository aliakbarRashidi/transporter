#pragma once

#include "Transporter.h"

#include <cstdint>

#include <memory>

#include "INetworkMessage.h"
#include "Buffer.h"

namespace transporter
{
	namespace network
	{
		namespace messages
		{
			/**
			* @brief Special network message that can receive and forward unknown network messages based on their ID and data size
			*
			* @see transporter::network::io::NetworkStream::receiveMessage()
			*/
			class TRANSPORTER_DLL UnknownMessage : public transporter::network::messages::INetworkMessage
			{
			public:
				/**
				* @brief Initialize an unknown network message with \p unknownMessageId as ID and a data size of \p unknownMessageSize
				*/
				UnknownMessage(transporter::network::messages::NetworkMessageId unknownMessageId, std::size_t unknownMessageSize);
				virtual ~UnknownMessage() = default;

				virtual transporter::network::messages::NetworkMessageId getMessageId() const noexcept override;

				virtual void serialize(transporter::data::io::IDataOutput &output) const noexcept override;
				virtual void deserialize(transporter::data::io::IDataInput &input) noexcept override;


				/**
				* @brief Get this unknown message's data
				*
				* @return Reference to this unknown message's data buffer
				*/
				const transporter::data::Buffer &getUnknownMessageData() const;


			private:
				transporter::network::messages::NetworkMessageId m_messageId;
				std::size_t m_messageSize;
				std::unique_ptr<transporter::data::Buffer> m_messageData;
			};
		}
	}
}
