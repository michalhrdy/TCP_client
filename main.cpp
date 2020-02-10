#include <connection_globals.h>
#include "TCPClient.h"

int main(int argc, char* argv[]) {

    asio::io_context io_context;
    TCPClient client(io_context, CParams::server_adress, CParams::service);
    asio::post(io_context, [&]() { client.Connect(); });
    //client.Connect();
    //io_context.post([&]() { client.Connect(); });
    io_context.run();

    return 0;
}