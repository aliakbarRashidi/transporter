#include "TestMessage.h"

transporter::network::messages::NetworkMessageId TestMessage::getMessageId() const noexcept
{
	return MESSAGE_ID;
}


void TestMessage::serialize(transporter::data::io::IDataOutput &output) const noexcept
{
	output.writeString(m_str1);
	output.writeInt32(m_i1);
}

void TestMessage::deserialize(transporter::data::io::IDataInput &input) noexcept
{
	m_str1 = input.readString();
	m_i1 = input.readInt32();
}
