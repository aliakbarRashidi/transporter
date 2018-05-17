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
	find_path(QTCORE_INCLUDE_DIR QtCore/qiodevice.h)
	
	target_compile_definitions(${TRANSPORTER_TARGET} PUBLIC -DTRANSPORTER_BUILD_INTERFACE_QT_QIODEVICE)
	target_include_directories(${TRANSPORTER_TARGET} PUBLIC ${QTCORE_INCLUDE_DIR})
	target_link_libraries(${TRANSPORTER_TARGET} ${Qt5Core_LIBRARIES})
endif()

if(TRANSPORTER_BUILD_INTERFACE_SFML_SOCKETS)
	find_package(SFML 2 REQUIRED COMPONENTS network)
	find_path(SFML_INCLUDE_DIR SFML/Network.hpp)
	
	target_compile_definitions(${TRANSPORTER_TARGET} PUBLIC -DTRANSPORTER_BUILD_INTERFACE_SFML_SOCKETS)
	target_include_directories(${TRANSPORTER_TARGET} PUBLIC ${SFML_INCLUDE_DIR})
	target_link_libraries(${TRANSPORTER_TARGET} ${SFML_LIBRARIES})
endif()

if(TRANSPORTER_BUILD_INTERFACE_BOOST_ASIO_SOCKETS)
	find_package(Boost REQUIRED COMPONENTS system)
	find_path(BOOST_INCLUDE_DIR boost/asio.hpp)
	
	target_compile_definitions(${TRANSPORTER_TARGET} PUBLIC -DTRANSPORTER_BUILD_INTERFACE_BOOST_ASIO_SOCKETS)
	target_include_directories(${TRANSPORTER_TARGET} PUBLIC ${BOOST_INCLUDE_DIR})
	target_link_libraries(${TRANSPORTER_TARGET} ${Boost_LIBRARIES})
endif()
