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
			class TRANSPORTER_DLL TransporterAdapterQtQAbstractSocket : public transporter::data::io::IByteStream
			{
			public:
				TransporterAdapterQtQAbstractSocket(QAbstractSocket *socket);

				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;


			private:
				QAbstractSocket *m_socket;
			};
		}
	}
}
