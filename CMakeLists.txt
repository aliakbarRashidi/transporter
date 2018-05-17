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

transporter_set_option(TRANSPORTER_BUILD_INTERFACE_QT_QIODEVICE OFF BOOL "Build interface for Qt's QIODevice (requires Qt 5 to be installed)")
transporter_set_option(TRANSPORTER_BUILD_INTERFACE_SFML_SOCKETS OFF BOOL "Build interface for SFML's sf::TcpSocket and sf::UdpSocket (requires SFML 2 to be installed)")

add_subdirectory(${TRANSPORTER_SRC_DIR})
