#include "TransporterInterfaceQtQIODevice.h"

#if defined(TRANSPORTER_BUILD_INTERFACE_QT_QIODEVICE)

transporter::interfaces::qt::TransporterInterfaceQtQIODevice::TransporterInterfaceQtQIODevice(QIODevice *device) : IByteStream{},
m_device{ device }
{
}


std::unique_ptr<transporter::data::Buffer> transporter::interfaces::qt::TransporterInterfaceQtQIODevice::readBytes(std::size_t count) noexcept
{
	try
	{
		transporter::data::BufferPtr buffer = transporter::data::BufferPtr{ new transporter::data::Buffer{ count } };
		qint64 readNb = m_device->read(buffer->getRawBuffer(), count);

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

ssize_t transporter::interfaces::qt::TransporterInterfaceQtQIODevice::writeBytes(const transporter::data::Buffer &buffer) noexcept
{
	return m_device->write(buffer.getRawBuffer(), buffer.getSize());
}

#endif
