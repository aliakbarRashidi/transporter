#if defined(TRANSPORTER_BUILD_ADAPTER_QT)
#include "TransporterAdapterQtQAbstractSocket.h"

transporter::adapters::qt::TransporterAdapterQtQAbstractSocket::TransporterAdapterQtQAbstractSocket(QAbstractSocket *socket) : IByteStream{},
m_socket{ socket }
{
}


std::unique_ptr<transporter::data::Buffer> transporter::adapters::qt::TransporterAdapterQtQAbstractSocket::readBytes(std::size_t count) noexcept
{
	try
	{
		transporter::data::BufferPtr buffer = transporter::data::BufferPtr{ new transporter::data::Buffer{ count } };
		qint64 readNb = m_socket->read(buffer->getRawBuffer(), count);

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

ssize_t transporter::adapters::qt::TransporterAdapterQtQAbstractSocket::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	return m_socket->write(buffer.getRawBuffer(), buffer.getSize());
}
#endif
