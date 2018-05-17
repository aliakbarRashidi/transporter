#if defined(TRANSPORTER_BUILD_ADAPTER_SFML)
#include "TransporterAdapterSfmlUdpSocket.h"

transporter::adapters::sfml::TransporterAdapterSfmlUdpSocket::TransporterAdapterSfmlUdpSocket(sf::UdpSocket &socket, const sf::IpAddress &receiverAddress, unsigned short receiverPort) : IByteStream{},
m_socket{ socket },
m_receiverAddress { receiverAddress },
m_receiverPort{ receiverPort }
{
}


std::unique_ptr<transporter::data::Buffer> transporter::adapters::sfml::TransporterAdapterSfmlUdpSocket::readBytes(std::size_t count) noexcept
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

ssize_t transporter::adapters::sfml::TransporterAdapterSfmlUdpSocket::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	sf::Socket::Status status = m_socket.send(buffer.getRawBuffer(), buffer.getSize(), m_receiverAddress, m_receiverPort);

	if (status == sf::Socket::Status::Done)
	{
		return buffer.getSize();
	}

	return -1;
}
#endif
