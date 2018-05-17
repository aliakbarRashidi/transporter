#pragma once

#include "Transporter.h"

#include <boost/asio/ip/tcp.hpp>

#include "IByteStream.h"

namespace transporter
{
	namespace adapters
	{
		namespace boost
		{
			namespace asio
			{
				class TRANSPORTER_DLL TransporterAdapterBoostAsioTcpSocket : public transporter::data::io::IByteStream
				{
				public:
					TransporterAdapterBoostAsioTcpSocket(::boost::asio::ip::tcp::socket &socket);

					virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
					virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;


				private:
					::boost::asio::ip::tcp::socket &m_socket;
				};
			}
		}
	}
}
