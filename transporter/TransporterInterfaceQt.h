#pragma once

#if defined(TRANSPORTER_BUILD_INTERFACE_QT)

#include "Transporter.h"

#include <QIODevice>

#include "IByteStream.h"

namespace transporter
{
	namespace interfaces
	{
		class TRANSPORTER_DLL TransporterInterfaceQt : public transporter::data::io::IByteStream
		{
		public:
			TransporterInterfaceQt(QIODevice *device);

			virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
			virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;


		private:
			QIODevice *m_device;
		};
	}
}

#endif
