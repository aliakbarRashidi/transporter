set(TRANSPORTER_TESTS_SRC
	Buffer_tests.cpp
	main.cpp
	MemoryStream_tests.cpp
	NetworkStream_tests.cpp
	TestMessage.cpp
)

add_executable(${TRANSPORTER_TESTS_TARGET} ${TRANSPORTER_TESTS_SRC})
target_include_directories(${TRANSPORTER_TESTS_TARGET} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${TRANSPORTER_TESTS_TARGET} ${TRANSPORTER_TARGET})
