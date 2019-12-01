//
//  TcpConnect.hpp
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef TcpConnect_hpp
#define TcpConnect_hpp

#include <stdio.h>
#include "ConnectServer.hpp"
using namespace ddk;
class TcpConnect : public ConnectServer{
private:
    boost::asio::ip::tcp::socket    *socket = nullptr;
    boost::asio::ip::tcp::endpoint  server_point;
    boost::asio::io_service         iosev;//boost库的所有asio类都要有io_service对象
    boost::system::error_code       error_message;
    
private:
    void set_server_point();
public:
    TcpConnect();
    ~TcpConnect();
    void set_server_ip_port(std::string tmp_ip, int tmp_port);
    bool connect_to_server();
    bool send_message_to_server();
    bool get_message_from_server();
    void close_connect();
    void start_threads();
    
};

#endif /* TcpConnect_hpp */
