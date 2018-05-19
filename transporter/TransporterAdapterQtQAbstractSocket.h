#pragma once

#include "Transporter.h"

#include <QtNetwork/QAbstractSocket>

#include "IByteStream.h"

namespace transporter
{
	namespace adapters
	{
		namespace qt
		{
			/**
			* @brief Adapter class for Qt's sockets (QTcpSocket and QUdpSocket)
			*
			* @details For UDP sockets (QUdpSocket), the socket must be "connected" using QUdpSocket::connectToHost()
			*/
			class TRANSPORTER_DLL TransporterAdapterQtQAbstractSocket : public transporter::data::io::IByteStream
			{
			public:
				TransporterAdapterQtQAbstractSocket(QAbstractSocket &socket) : IByteStream{},
					m_socket{ socket }
				{
				}


				virtual transporter::data::BufferPtr readBytes(std::size_t count) noexcept override
				{
					try
					{
						transporter::data::BufferPtr buffer{ new transporter::data::Buffer{ count } };
						qint64 readNb = m_socket.read(buffer->getRawBuffer(), count);

						if (readNb < 0)
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
					return m_socket.write(buffer.getRawBuffer(), buffer.getSize());
				}


			private:
				QAbstractSocket &m_socket;
			};
		}
	}
}
