set(TRANSPORTER_TARGET transporter)

set(TRANSPORTER_SRC
	Buffer.cpp
	MemoryStream.cpp
	NetworkStream.cpp
	TransactionException.cpp
	TransporterInterfaceQt.cpp
	UnknownMessage.cpp
)

add_library(${TRANSPORTER_TARGET} ${TRANSPORTER_SRC})

if(TRANSPORTER_BUILD_INTERFACE_QT)
	find_package(Qt5Core REQUIRED)
	target_compile_definitions(${TRANSPORTER_TARGET} PRIVATE -DTRANSPORTER_BUILD_INTERFACE_QT)
	target_link_libraries(${TRANSPORTER_TARGET} Qt5::Core)
endif()
