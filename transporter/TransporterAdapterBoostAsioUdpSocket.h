#pragma once

#include "Transporter.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/udp.hpp>

#include "IByteStream.h"

namespace transporter
{
	namespace adapters
	{
		namespace boost
		{
			namespace asio
			{
				/**
				* @brief Adapter class for Boost.Asio's UDP sockets (boost::asio::ip::udp::socket)
				*
				* @details The socket must be "connected" using boost::asio::ip::udp::connect()
				*/
				class TRANSPORTER_DLL TransporterAdapterBoostAsioUdpSocket : public transporter::data::io::IByteStream
				{
				public:
					TransporterAdapterBoostAsioUdpSocket(::boost::asio::ip::udp::socket &socket) : IByteStream{},
						m_socket{ socket }
					{
					}


					virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override
					{
						try
						{
							transporter::data::BufferPtr buffer = transporter::data::BufferPtr{ new transporter::data::Buffer{ count } };
							std::size_t readNb = m_socket.receive(::boost::asio::buffer(buffer->getRawBuffer(), buffer->getSize()));

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
						return m_socket.send(::boost::asio::buffer(buffer.getRawBuffer(), buffer.getSize()));
					}


				private:
					::boost::asio::ip::udp::socket &m_socket;
				};
			}
		}
	}
}
