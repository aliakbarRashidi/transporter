#if defined(TRANSPORTER_BUILD_ADAPTER_BOOST_ASIO)
#include "TransporterAdapterBoostAsioUdpSocket.h"

#include <boost/asio/buffer.hpp>

transporter::adapters::boost::asio::TransporterAdapterBoostAsioUdpSocket::TransporterAdapterBoostAsioUdpSocket(::boost::asio::ip::udp::socket &socket) : IByteStream{},
m_socket{ socket }
{
}


std::unique_ptr<transporter::data::Buffer> transporter::adapters::boost::asio::TransporterAdapterBoostAsioUdpSocket::readBytes(std::size_t count) noexcept
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

ssize_t transporter::adapters::boost::asio::TransporterAdapterBoostAsioUdpSocket::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	return m_socket.send(::boost::asio::buffer(buffer.getRawBuffer(), buffer.getSize()));
}
#endif
