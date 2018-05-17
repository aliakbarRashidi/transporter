#pragma once

#if defined(TRANSPORTER_BUILD_INTERFACE_SFML_SOCKETS)

#include "Transporter.h"

#include <SFML/Network/UdpSocket.hpp>

#include "IByteStream.h"

namespace transporter
{
	namespace interfaces
	{
		namespace sfml
		{
			class TRANSPORTER_DLL TransporterInterfaceSfmlUdpSocket : public transporter::data::io::IByteStream
			{
			public:
				TransporterInterfaceSfmlUdpSocket(sf::UdpSocket &socket, const sf::IpAddress &receiverAddress, unsigned short receiverPort);

				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;


			private:
				sf::UdpSocket &m_socket;
				const sf::IpAddress &m_receiverAddress;
				unsigned short m_receiverPort;
			};
		}
	}
}

#endif
