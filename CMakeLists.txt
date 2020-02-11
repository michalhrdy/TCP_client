cmake_minimum_required(VERSION 3.15)
project(ASIO_test_client)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASIO_STANDALONE)

add_executable(ASIO_test_client main.cpp TCPClient.h TCPClient.cpp)

if(WIN32)
    include_directories(C:/Users/michalh/CLionProjects/ASIO/asio-1.12.2/include C:/Users/michalh/CLionProjects/includes)
    target_link_libraries(ASIO_test_client wsock32 ws2_32)
endif(WIN32)
if(UNIX)
    include_directories(/root/CLionProjects/ASIO/asio-1.12.2/include/ /root/CLionProjects/includes/)
    target_link_libraries(ASIO_test_client pthread)
endif(UNIX)