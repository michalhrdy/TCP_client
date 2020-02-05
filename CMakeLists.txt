cmake_minimum_required(VERSION 3.15)
project(ASIO_test_client)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASIO_STANDALONE)

include_directories(C:/ASIO/asio-1.12.2/include C:/Users/michalh/CLionProjects/includes)

add_executable(ASIO_test_client main.cpp TCPClient.h TCPClient.cpp)

if(WIN32)
    target_link_libraries(ASIO_test_client wsock32 ws2_32)
endif()
if(UNIX)
    target_link_libraries(ASIO_test_client rt)
endif()