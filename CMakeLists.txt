project (asio_server)

cmake_minimum_required(VERSION 3.15)

set (asio_server_INCLUDE_DIRS ./)
set (asio_server_SOURCES src/main.cpp src/blocking_tcp_echo_server.cpp)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11")

# add_library (asio STATIC ${asio_HEAD} ${asio_SOURCES})

# Library properties.
#"${CMAKE_BINARY_DIR}/lib"
# set (asio_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${platform}/lib")
# set_target_properties (asio_test PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${asio_OUTPUT_DIRECTORY})

set (Boost_package_option thread system regex)
set (Boost_INCLUDE_DIR "c:/boost_1_56_0")
include_directories (include . "${Boost_INCLUDE_DIR}")
add_definitions (-DBOOST_ALL_NO_LIB)
set (Boost_USE_STATIC_LIBS ON)

set (LIBRARY_DIRS "c:/boost_1_56_0/stage/lib")

find_package(Boost REQUIRED COMPONENTS ${Boost_package_option})

add_executable(asio_server ${asio_server_SOURCES})

TARGET_LINK_LIBRARIES(asio_server "${Boost_SYSTEM_LIBRARY}" ws2_32)