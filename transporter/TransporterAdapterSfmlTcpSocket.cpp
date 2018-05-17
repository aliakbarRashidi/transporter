#if defined(TRANSPORTER_BUILD_ADAPTER_SFML)
#include "TransporterAdapterSfmlTcpSocket.h"

transporter::adapters::sfml::TransporterAdapterSfmlTcpSocket::TransporterAdapterSfmlTcpSocket(sf::TcpSocket &socket) : IByteStream{},
m_socket{ socket }
{
}


std::unique_ptr<transporter::data::Buffer> transporter::adapters::sfml::TransporterAdapterSfmlTcpSocket::readBytes(std::size_t count) noexcept
{
	try
	{
		transporter::data::BufferPtr buffer = transporter::data::BufferPtr{ new transporter::data::Buffer{ count } };
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

ssize_t transporter::adapters::sfml::TransporterAdapterSfmlTcpSocket::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	std::size_t sent = 0;
	sf::Socket::Status status = m_socket.send(buffer.getRawBuffer(), buffer.getSize(), sent);

	if (status == sf::Socket::Status::Done || status == sf::Socket::Status::Partial)
	{
		return sent;
	}

	return -1;
}
#endif
