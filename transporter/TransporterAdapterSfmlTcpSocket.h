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
			class TRANSPORTER_DLL TransporterAdapterSfmlTcpSocket : public transporter::data::io::IByteStream
			{
			public:
				TransporterAdapterSfmlTcpSocket(sf::TcpSocket &socket);

				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;


			private:
				sf::TcpSocket &m_socket;
			};
		}
	}
}
