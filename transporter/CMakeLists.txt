set(TRANSPORTER_TARGET transporter)

set(TRANSPORTER_SRC
	Buffer.cpp
	MemoryStream.cpp
	NetworkStream.cpp
	TransactionException.cpp
	TransporterAdapterBoostAsioTcpSocket.cpp
	TransporterAdapterBoostAsioUdpSocket.cpp
	TransporterAdapterQtQAbstractSocket.cpp
	TransporterAdapterSfmlTcpSocket.cpp
	TransporterAdapterSfmlUdpSocket.cpp
	UnknownMessage.cpp
)

add_library(${TRANSPORTER_TARGET} ${TRANSPORTER_SRC})
target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_EXPORTS)

if(TRANSPORTER_BUILD_ADAPTER_QT)
	find_package(Qt5Core REQUIRED)
	find_path(QTCORE_INCLUDE_DIR QtNetwork/qabstractsocket.h)
	
	target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_BUILD_ADAPTER_QT)
	target_include_directories(${TRANSPORTER_TARGET} PRIVATE ${QTCORE_INCLUDE_DIR})
	target_link_libraries(${TRANSPORTER_TARGET} ${Qt5Core_LIBRARIES})
endif()

if(TRANSPORTER_BUILD_ADAPTER_SFML)
	find_package(SFML 2 REQUIRED COMPONENTS network)
	find_path(SFML_INCLUDE_DIR SFML/Network.hpp)
	
	target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_BUILD_ADAPTER_SFML)
	target_include_directories(${TRANSPORTER_TARGET} PRIVATE ${SFML_INCLUDE_DIR})
	target_link_libraries(${TRANSPORTER_TARGET} ${SFML_LIBRARIES})
endif()

if(TRANSPORTER_BUILD_ADAPTER_BOOST_ASIO)
	find_package(Boost REQUIRED COMPONENTS system)
	find_path(BOOST_INCLUDE_DIR boost/asio.hpp)
	
	target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_BUILD_ADAPTER_BOOST_ASIO)
	target_include_directories(${TRANSPORTER_TARGET} PRIVATE ${BOOST_INCLUDE_DIR})
	target_link_libraries(${TRANSPORTER_TARGET} ${Boost_LIBRARIES})
endif()
