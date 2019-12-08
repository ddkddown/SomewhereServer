//
//  TcpConnect.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include "TcpConnect.hpp"
TcpConnect::TcpConnect(){
    socket = new (std::nothrow) boost::asio::ip::tcp::socket(iosev);
    if(!socket){
        LOGE("init socket failed!, app will terminate!");
        exit(-1);
    }
}

TcpConnect::~TcpConnect(){
    if(socket){
        delete socket;
    }
}

void TcpConnect::set_server_ip_port(std::string tmp_ip, int tmp_port){
    set_server_ip(tmp_ip);
    set_server_port(tmp_port);
}

void TcpConnect::set_server_point(){
    server_point.address(boost::asio::ip::address_v4::from_string(get_server_ip().c_str()));
    server_point.port(get_server_port());
}

bool TcpConnect::send_message_to_server(){
    size_t send_len;
    bool ret = false;
    if(get_send_buffer().empty())
    {
        LOGE("send buffer is empty! close socket and return!");
        socket->close();
        goto out;
    }
    
    send_len = socket->write_some(boost::asio::buffer(get_send_buffer(),get_send_buffer().length()),error_message);
    if(error_message){
        LOGE("send message to server write some error:%s"
             ,boost::system::system_error(error_message).what());
        socket->close();
        goto out;
    }
    
    ret = true;
out:
    return ret;
}

bool TcpConnect::get_message_from_server(){
    size_t recv_len;
    bool ret = false;
    char recv_message[1024] = {0};
    
    recv_len = socket->read_some(boost::asio::buffer(recv_message),error_message);
    if(error_message){
        LOGE("get message from server read some error :%s"
             ,boost::system::system_error(error_message).what());
        socket->close();
        goto out;
    }
    
    ret = true;
out:
    return ret;
}

bool TcpConnect::connect_to_server(){
    bool ret = false;
    
    if(get_server_port() == -1){
        LOGE("port == -1 , that is invalid! return!");
        set_status(false);
        goto out;
    }
    
    set_server_point();
    
    socket->connect(server_point,error_message);
    if(error_message)
    {
        LOGE("connect to server error :%s",
             boost::system::system_error(error_message).what());
        set_status(false);
        goto out;
    }
    
    set_status(true);
    
    ret = true;
out:
    return ret;
}

void TcpConnect::close_connect(){
    if(get_status()){
        socket->close();
    }
    set_status(false);
}

void TcpConnect::start_threads(){
    for(int i = 0; i < get_threads_num(); ++i){
        iosev.run();//开启threads_num个处理线程
    }
}
