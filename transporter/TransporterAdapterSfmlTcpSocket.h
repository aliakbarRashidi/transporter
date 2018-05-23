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

#include <SFML/Network/TcpSocket.hpp>

#include "IByteStream.h"

namespace transporter
{
	namespace adapters
	{
		namespace sfml
		{
			/**
			* @brief Adapter class for SFML's TCP sockets (sf::TcpSocket)
			*/
			class TransporterAdapterSfmlTcpSocket : public transporter::data::io::IByteStream
			{
			public:
				TransporterAdapterSfmlTcpSocket(sf::TcpSocket &socket) : IByteStream{},
					m_socket{ socket }
				{
				}


				virtual transporter::data::BufferPtr readBytes(std::size_t count) noexcept override
				{
					try
					{
						transporter::data::BufferPtr buffer{ new transporter::data::Buffer{ count } };
						std::size_t readNb = 0;
						sf::Socket::Status status = m_socket.receive(buffer->getRawBuffer(), count, readNb);

						if (status != sf::Socket::Status::Done)
						{
							return nullptr;
						}

						if (readNb < count)
						{
							buffer->shrinkEnd(count - readNb);
						}

						return buffer;
					}

					catch (...)
					{
						return nullptr;
					}
				}

				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override
				{
					std::size_t sent = 0;
					sf::Socket::Status status = m_socket.send(buffer.getRawBuffer(), buffer.getSize(), sent);

					if (status == sf::Socket::Status::Done || status == sf::Socket::Status::Partial)
					{
						return sent;
					}

					return -1;
				}


			private:
				sf::TcpSocket &m_socket;
			};
		}
	}
}
