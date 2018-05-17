cmake_minimum_required(VERSION 3.1.3)

project(transporter)

macro(transporter_set_option var default type docstring)
	if(NOT DEFINED ${var})
		set(${var} ${default})
	endif()
	
	set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

set(CMAKE_CXX_STANDARD 11)
set(TRANSPORTER_SRC_DIR transporter)

transporter_set_option(TRANSPORTER_BUILD_ADAPTER_QT OFF BOOL "Build adapter for Qt's QAbstractSocket (requires Qt 5 to be installed)")
transporter_set_option(TRANSPORTER_BUILD_ADAPTER_SFML OFF BOOL "Build adapters for SFML's sf::TcpSocket and sf::UdpSocket (requires SFML 2 to be installed)")
transporter_set_option(TRANSPORTER_BUILD_ADAPTER_BOOST_ASIO OFF BOOL "Build adapters for Boost.Asio's boost::asio::ip::tcp::socket and boost::asio::ip::udp::socket (requires Boost.Asio to be installed)")

add_subdirectory(${TRANSPORTER_SRC_DIR})
