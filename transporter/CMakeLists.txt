set(TRANSPORTER_SRC
	Buffer.cpp
	MemoryStream.cpp
	NetworkStream.cpp
	TransactionException.cpp
	UnknownMessage.cpp
)

add_library(${TRANSPORTER_TARGET} ${TRANSPORTER_SRC})
target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_EXPORTS)
