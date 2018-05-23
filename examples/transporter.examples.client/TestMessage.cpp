#include "TestMessage.h"

/*
* /!\ THIS CLASS MUST BE IDENTICAL IN BOTH CLIENT AND SERVER PROJECTS. /!\
* It is best to put all network messages in a SHARED LIBRARY (it is not the case in this example for the sake of simplicity and clarity).
*/

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


const std::string &TestMessage::getStr1() const
{
	return m_str1;
}

std::int32_t TestMessage::getI1() const
{
	return m_i1;
}


void TestMessage::setStr1(const std::string &str1)
{
	m_str1 = str1;
}

void TestMessage::setI1(std::int32_t i1)
{
	m_i1 = i1;
}
