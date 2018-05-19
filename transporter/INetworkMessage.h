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
