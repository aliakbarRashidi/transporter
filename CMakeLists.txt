cmake_minimum_required(VERSION 3.1.3)

project(transporter)

set(CMAKE_CXX_STANDARD 11)

set(TRANSPORTER_TARGET transporter)
set(TRANSPORTER_TESTS_TARGET transporter.tests)
set(TRANSPORTER_EXAMPLES_CLIENT_TARGET transporter.examples.client)
set(TRANSPORTER_EXAMPLES_SERVER_TARGET transporter.examples.server)

set(TRANSPORTER_SRC_DIR ${CMAKE_SOURCE_DIR}/transporter)
set(TRANSPORTER_TESTS_SRC_DIR ${CMAKE_SOURCE_DIR}/transporter.tests)
set(TRANSPORTER_EXAMPLES_CLIENT_SRC_DIR ${CMAKE_SOURCE_DIR}/examples/transporter.examples.client)
set(TRANSPORTER_EXAMPLES_SERVER_SRC_DIR ${CMAKE_SOURCE_DIR}/examples/transporter.examples.server)

add_subdirectory(${TRANSPORTER_SRC_DIR})
add_subdirectory(${TRANSPORTER_TESTS_SRC_DIR})
add_subdirectory(${TRANSPORTER_EXAMPLES_CLIENT_SRC_DIR})
add_subdirectory(${TRANSPORTER_EXAMPLES_SERVER_SRC_DIR})
