#include "TransporterInterfaceSfmlUdpSocket.h"

#if defined(TRANSPORTER_BUILD_INTERFACE_SFML_SOCKETS)

transporter::interfaces::sfml::TransporterInterfaceSfmlUdpSocket::TransporterInterfaceSfmlUdpSocket(sf::UdpSocket &socket, const sf::IpAddress &receiverAddress, unsigned short receiverPort) : IByteStream{},
m_socket{ socket },
m_receiverAddress { receiverAddress },
m_receiverPort{ receiverPort }
{
}


std::unique_ptr<transporter::data::Buffer> transporter::interfaces::sfml::TransporterInterfaceSfmlUdpSocket::readBytes(std::size_t count) noexcept
{
	try
	{
		transporter::data::BufferPtr buffer = transporter::data::BufferPtr{ new transporter::data::Buffer{ count } };
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

ssize_t transporter::interfaces::sfml::TransporterInterfaceSfmlUdpSocket::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	sf::Socket::Status status = m_socket.send(buffer.getRawBuffer(), buffer.getSize(), m_receiverAddress, m_receiverPort);

	if (status == sf::Socket::Status::Done)
	{
		return buffer.getSize();
	}

	return -1;
}

#endif
