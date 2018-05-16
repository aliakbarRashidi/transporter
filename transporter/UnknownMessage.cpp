#include "UnknownMessage.h"

transporter::network::messages::UnknownMessage::UnknownMessage(transporter::network::messages::NetworkMessageId unknownMessageId, std::size_t unknownMessageSize) : INetworkMessage{},
m_messageId{ unknownMessageId },
m_messageSize{ unknownMessageSize },
m_messageData{}
{
}


void transporter::network::messages::UnknownMessage::serialize(transporter::data::io::IDataOutput &output) const noexcept
{
	output.writeBytes(*m_messageData);
}

void transporter::network::messages::UnknownMessage::deserialize(transporter::data::io::IDataInput &input) noexcept
{
	m_messageData = input.readBytes(m_messageSize);
}


transporter::network::messages::NetworkMessageId transporter::network::messages::UnknownMessage::getMessageId() const noexcept
{
	return m_messageId;
}


const transporter::data::Buffer &transporter::network::messages::UnknownMessage::getUnknownMessageData() const
{
	return *m_messageData;
}
