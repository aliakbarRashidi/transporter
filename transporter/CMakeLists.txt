set(TRANSPORTER_SOURCES
	Buffer.cpp
	MemoryStream.cpp
	NetworkStream.cpp
	TransactionException.cpp
	UnknownMessage.cpp
)

add_library(transporter ${TRANSPORTER_SOURCES})
