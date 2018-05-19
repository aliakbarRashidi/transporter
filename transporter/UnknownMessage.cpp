/*
* MIT License
*
* Copyright (c) 2018 Guillaume Truchot - guillaume.truchot@outlook.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
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
