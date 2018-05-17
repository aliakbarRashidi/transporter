set(TRANSPORTER_TARGET transporter)

set(TRANSPORTER_SRC
	Buffer.cpp
	MemoryStream.cpp
	NetworkStream.cpp
	TransactionException.cpp
	TransporterInterfaceQtQIODevice.cpp
	TransporterInterfaceSfmlTcpSocket.cpp
	TransporterInterfaceSfmlUdpSocket.cpp
	UnknownMessage.cpp
)

add_library(${TRANSPORTER_TARGET} ${TRANSPORTER_SRC})
target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_EXPORTS)

if(TRANSPORTER_BUILD_INTERFACE_QT_QIODEVICE)
	find_package(Qt5Core REQUIRED)
	target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_BUILD_INTERFACE_QT_QIODEVICE)
	target_link_libraries(${TRANSPORTER_TARGET} Qt5::Core)
endif()

if(TRANSPORTER_BUILD_INTERFACE_SFML_SOCKETS)
	find_package(SFML 2 REQUIRED COMPONENTS network)
	target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_BUILD_INTERFACE_SFML_SOCKETS)
	target_link_libraries(${TRANSPORTER_TARGET} sfml-network)
endif()
