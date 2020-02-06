//
// Created by michalh on 2/3/2020.
//
#include <connection_globals.h>
#include "TCPClient.h"

TCPClient::TCPClient(asio::io_context &io_context, std::string adress, std::string service):
                                                    resolver_(io_context),
                                                    socket_(io_context),
                                                    server_adress_(adress),
                                                    service_(service){
    //Connect(CParams.server_adress, CParams.service);
}

TCPClient::~TCPClient() {
    std::cout << "Client Closed" << std::endl;
}

void TCPClient::Connect() {

//    auto LHandleConnect = [this](const asio::error_code& error,
//                              const typename tcp::endpoint& endpoint) {
//        if (!error) {
//            Read();
//        }  else if(retry_count_ < CParams.connect_retry_attempts) {
//            std::cout << error.message() << std::endl;
//            std::cout << "Reconnecting. Attempt number: " << retry_count_+1 << std::endl;
//            retry_count_++;
//            Connect();
//        } else {
//            std::cout << "Connection could not be established." << std::endl;
//        }
//    };
//
//    auto LHandleResolve = [this, &LHandleConnect](const asio::error_code& error,
//            tcp::resolver::results_type results) {
//        if (!error) {
//            asio::async_connect(socket_, results, LHandleConnect);
//        } else {
//            std::cout << error.message() << std::endl;
//        }
//    };

    resolver_.async_resolve(server_adress_, service_,     std::bind(&TCPClient::HandleResolve,
                                                                    this,
                                                                    std::placeholders::_1,
                                                                    std::placeholders::_2));

//    std::bind(&TCPClient::HandleResolve,
//              this,
//              std::placeholders::_1,
//              std::placeholders::_2)
}

void TCPClient::HandleResolve(const asio::error_code& error,
                   tcp::resolver::results_type results) {
    if (!error) {
        asio::async_connect(socket_, results, std::bind(&TCPClient::HandleConnect,
                                                        this,
                                                        std::placeholders::_1,
                                                        std::placeholders::_2));
    } else {
        std::cout << error.message() << std::endl;
    }
}

void TCPClient::HandleConnect(const asio::error_code& error,
                              const typename tcp::endpoint& endpoint) {

    if (!error) {
        Read();
    }
    else if(retry_count_  < CParams.connect_retry_attempts) {
        std::cout << error.message() << std::endl;
        std::cout << "Reconnecting. Attempt number: " << retry_count_+1 << std::endl;
        retry_count_++;
        Connect();
    } else {
        std::cout << "Connection could not be established." << std::endl;
    }

}

void TCPClient::Read() {

//    auto LHandleRead = [this](const asio::error_code& error, std::size_t bytes_transferred) {
//        std::string s( (std::istreambuf_iterator<char>(&recieved_message_buffer_)), std::istreambuf_iterator<char>() );
//        std::cout << s;
//        std::cout << std::endl;
//        ReadCommandFromConsole();
//        Write();
//    };

    asio::async_read_until(socket_, recieved_message_buffer_, CParams.message_delimiter,std::bind(&TCPClient::HandleRead,
                                                                                                       this,
                                                                                                       std::placeholders::_1,
                                                                                                       std::placeholders::_2));

//    std::bind(&TCPClient::HandleRead,
//              this,
//              std::placeholders::_1,
//              std::placeholders::_2)
}

void TCPClient::HandleRead(const asio::error_code& error, std::size_t bytes_transferred) {
    if(error) {
        std::cout << error.message() << std::endl;
        return;
    } else {
        std::string s( (std::istreambuf_iterator<char>(&recieved_message_buffer_)),
                       std::istreambuf_iterator<char>() );
        //Remove delimiter
        s.erase(s.size() - 1);

        std::cout << s << std::endl;
        ReadCommandFromConsole();
        Write();
    }
}

void TCPClient::Write() {

//    auto LHandleWrite = [this](const asio::error_code& error, std::size_t bytes_transferred) {
//        if(selected_command_ == services.size()-1) {
//            return;
//        }
//        Read();
//    };

    asio::async_write(socket_, asio::buffer(message_to_send_),
            std::bind(&TCPClient::HandleWrite, this,
                    std::placeholders::_1,
                    std::placeholders::_2));

//    std::bind(&TCPClient::HandleWrite, this,
//              std::placeholders::_1,
//              std::placeholders::_2)
}

void TCPClient::HandleWrite(const asio::error_code& error, size_t bytes_transferred) {
    if(!error){
        Read();
    }
}

void TCPClient::ShowMenu() {
    std::cout << "Choose request: " << std::endl;
    unsigned int index = 1;

    for(auto it = service_table.begin(); it != service_table.end(); ++it) {
        std::cout << index << " - " << it->second << std::endl;
        ++index;
    }
}

void TCPClient::ReadCommandFromConsole() {
    ShowMenu();
    std::cin >> selected_command_;

    if(selected_command_ > service_table.size()) {
        std::cout << "Invalid input." << std::endl;
        ReadCommandFromConsole();
    } else {
        message_to_send_ = service_table[selected_command_-1].first + CParams.message_delimiter;
    }
}
