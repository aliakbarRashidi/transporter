#pragma once

#if defined(TRANSPORTER_BUILD_INTERFACE_QT_QIODEVICE)

#include "Transporter.h"

#include <QIODevice>

#include "IByteStream.h"

namespace transporter
{
	namespace interfaces
	{
		namespace qt
		{
			class TRANSPORTER_DLL TransporterInterfaceQtQIODevice : public transporter::data::io::IByteStream
			{
			public:
				TransporterInterfaceQtQIODevice(QIODevice *device);

				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;


			private:
				QIODevice *m_device;
			};
		}
	}
}

#endif
