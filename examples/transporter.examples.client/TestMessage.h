#pragma once

#include "transporter/INetworkMessage.h"

#include <cstdint>

#include <string>

/*
* /!\ THIS CLASS MUST BE IDENTICAL IN BOTH CLIENT AND SERVER PROJECTS. /!\
* It is best to put all network messages in a SHARED LIBRARY (it is not the case in this example for the sake of simplicity and clarity).
*/

class TestMessage : public transporter::network::messages::INetworkMessage
{
public:
	static const transporter::network::messages::NetworkMessageId MESSAGE_ID = 1;

	virtual transporter::network::messages::NetworkMessageId getMessageId() const noexcept override;

	virtual void serialize(transporter::data::io::IDataOutput &output) const noexcept override;
	virtual void deserialize(transporter::data::io::IDataInput &input) noexcept override;


	const std::string &getStr1() const;
	std::int32_t getI1() const;

	void setStr1(const std::string &str1);
	void setI1(std::int32_t i1);


private:
	std::string m_str1;
	std::int32_t m_i1;
};
