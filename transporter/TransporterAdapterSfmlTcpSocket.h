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
			class TRANSPORTER_DLL TransporterAdapterSfmlTcpSocket : public transporter::data::io::IByteStream
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
