#pragma once

#include "Transporter.h"

#include <SFML/Network/UdpSocket.hpp>

#include "IByteStream.h"

namespace transporter
{
	namespace adapters
	{
		namespace sfml
		{
			/**
			* @brief Adapter class for SFML's UDP sockets (sf::UdpSocket)
			*
			* @details As sf::UdpSocket provides no way to bind a socket to a remote endpoint, the receiver of the future messages must be passed in the constructor
			*/
			class TRANSPORTER_DLL TransporterAdapterSfmlUdpSocket : public transporter::data::io::IByteStream
			{
			public:
				/**
				* @param socket sf::UdpSocket to use for sending and receiving messages
				* @param receiverAddress Remote address of the receiver of the messages
				* @param receiverPort Remote port of the receiver of the messages
				*/
				TransporterAdapterSfmlUdpSocket(sf::UdpSocket &socket, const sf::IpAddress &receiverAddress, unsigned short receiverPort) : IByteStream{},
					m_socket{ socket },
					m_receiverAddress{ receiverAddress },
					m_receiverPort{ receiverPort }
				{
				}


				virtual transporter::data::BufferPtr readBytes(std::size_t count) noexcept override
				{
					try
					{
						transporter::data::BufferPtr buffer{ new transporter::data::Buffer{ count } };
						std::size_t readNb = 0;
						sf::IpAddress remoteAddress{};
						unsigned short remotePort = 0;
						sf::Socket::Status status = m_socket.receive(buffer->getRawBuffer(), count, readNb, remoteAddress, remotePort);

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
					sf::Socket::Status status = m_socket.send(buffer.getRawBuffer(), buffer.getSize(), m_receiverAddress, m_receiverPort);

					if (status == sf::Socket::Status::Done)
					{
						return buffer.getSize();
					}

					return -1;
				}


			private:
				sf::UdpSocket &m_socket;
				const sf::IpAddress &m_receiverAddress;
				unsigned short m_receiverPort;
			};
		}
	}
}
