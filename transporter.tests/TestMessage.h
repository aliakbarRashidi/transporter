#pragma once

#include "transporter/INetworkMessage.h"

#include <cstdint>

#include <string>

class TestMessage : public transporter::network::messages::INetworkMessage
{
public:
	static const transporter::network::messages::NetworkMessageId MESSAGE_ID = 1;

	virtual transporter::network::messages::NetworkMessageId getMessageId() const noexcept override;

	virtual void serialize(transporter::data::io::IDataOutput &output) const noexcept override;
	virtual void deserialize(transporter::data::io::IDataInput &input) noexcept override;


	std::string m_str1;
	std::int32_t m_i1;
};
