//
// Created by michalh on 2/3/2020.
//

#include <iostream>
#include <array>
#include <asio.hpp>
#include <functional>
#include <memory>
#include <string>
#include "connection_globals.h"

#ifndef ASIO_TEST_CLIENT_TCPCLIENT_H
#define ASIO_TEST_CLIENT_TCPCLIENT_H

using asio::ip::tcp;

//Class for communication with TCPServer declared in TCPServer.h
class TCPClient {
public:
    TCPClient(asio::io_context &io_context, std::string adress, std::string service);
    ~TCPClient();

    void Connect();

private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    tcp::resolver::results_type endpoints_;

    std::string server_adress_;
    std::string service_;

    //Outgoing data
    std::string message_to_send_;
    //User-selected command
    unsigned short int selected_command_;
    //Incoming data buffer
    asio::streambuf recieved_message_buffer_;
    //Connection retry count
    unsigned int retry_count_ = 0;

    void HandleResolve(const asio::error_code& error,
                       tcp::resolver::results_type results);

    void HandleConnect(const asio::error_code& error,
            const typename tcp::endpoint& endpoint);

    //Asynchronously reads data from socket_
    void Read();
    //Read handler - Handles Data acquired by Read() and calls Write()
    void HandleRead(const asio::error_code& error, std::size_t bytes_transferred);
    //Asynchronously writes data to socket_
    void Write();
    //Write handler - Calls Read() after Write() finished writing to socket_
    void HandleWrite(const asio::error_code& /*error*/, size_t /*bytes_transferred*/);
    //Displays available commands in console
    void ShowMenu();
    //Reads selected command from console
    void ReadCommandFromConsole();
};

#endif //ASIO_TEST_CLIENT_TCPCLIENT_H
