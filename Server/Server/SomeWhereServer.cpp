//
//  SomeWhereServer.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/18.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include "SomeWhereServer.hpp"

const SomeWhereServer* SomeWhereServer::m_instance = new SomeWhereServer;
bool SomeWhereServer::accept_repeat_flag = false;

static void send_login_back_message(boost::asio::ip::tcp::socket& peer,bool state){
    reply_message back_message;
    boost::system::error_code ec;
    char buf[sizeof(reply_message)];
    
    memset(buf, -1, sizeof(buf));
    back_message.type = LOGIN_MESSAGE;
    back_message.status = state;
    memcpy(buf, &back_message, sizeof(reply_message));
    
    peer.write_some(buffer(buf),ec);
    if (ec) {
        std::cout<<"send_login_back_message failed!"<<std::endl;
    }
}

static void put_mysql_data_to_redis(){
    
}
static bool handle_login_message(somewhere_message& client_message,boost::asio::ip::tcp::socket& peer){
    std::string username;
    std::string password;
    std::string result;
    
    std::string exec_redis = "get ";
    std::string exec_sql = "select * from somewhere_login where login_id = '";
    
    
    int username_length = 0;
    int password_length = 0;
    
    for(username_length = 0; username_length < NAME_LENGTH &&
        client_message.user_name[username_length] != '\xff'; ++username_length){
        
        username.push_back(client_message.user_name[username_length]);
    }
    for(password_length = 0; password_length < PASSWORD_LENGTH &&
        client_message.password[password_length] != '\xff'; ++password_length){
        
        password.push_back(client_message.password[password_length]);
    }
    
    std::cout<<"[handle_login_message] username:%s"<<username<<" ,password:%s"<<password<<std::endl;
    
    
    exec_redis.append(username);
    
    //先通过redis查询
    std::cout<<"[info] try search by redis"<<std::endl;
    result = RequestHandle::redis_login_op(SomeWhereServer::get_instance()->get_redis_instance(),exec_redis);
    if(!result.empty()){
        goto out;
    }
    
    result.clear();
    
    //redis查询失败时用 mysql查询
    exec_sql.append(username);
    exec_sql.append("';");
    
    std::cout<<"[info] try search by mysql"<<std::endl;
    result = RequestHandle::mysql_login_op(SomeWhereServer::get_instance()->get_mysql_instance(),exec_sql);
    if(!result.empty()){
        goto out;
    }
    
out:

    //比较字符串,返回密码是否正确
    if(0 == password.compare(result)){
        std::cout<<"password right"<<std::endl;
        //将mysql数据放到redis上
        put_mysql_data_to_redis();
        return true;
    }else {
        std::cout<<"password wrong"<<std::endl;
        return false;
    }
    
    
    
}
static void handle_message(somewhere_message& client_message,boost::asio::ip::tcp::socket& peer){
    
    switch (client_message.type) {
        case LOGIN_MESSAGE:
            if(handle_login_message(client_message,peer)){
                send_login_back_message(peer,true);
            }else{
                send_login_back_message(peer,false);
            }
            break;
            
        default:
            std::cout<<"[warning] wrong message type"<<client_message.message_body<<std::endl;
            break;
    }
    
}

static void handle_socket_request(const boost::system::error_code& error,
                                    boost::asio::ip::tcp::socket& peer){
        while(1){
            //对一次socket请求的处理过程
            boost::system::error_code ec;
            peer.wait(peer.wait_read,ec);
            if(!ec){
                try{
                    char buf[sizeof(somewhere_message)];
                    size_t len = peer.read_some(buffer(buf));
                    if(len > 0){
                        somewhere_message client_message;
                        std::string sql1;
                        std::string sql2;
                        memcpy(&client_message, buf, sizeof(somewhere_message));
                        handle_message(client_message, peer);
                    }
                }catch(std::exception& e){
                    std::cout<<e.what()<<std::endl;
                    peer.close();
                }
            }
            else{
                std::cout<<"error , return"<<std::endl;
                return;
            }
            //对一次socket请求的处理过程
        }
}

static void default_accept_handler(const boost::system::error_code& error,
                                    boost::asio::ip::tcp::socket peer){
    cout<<"[info] get an client request"<<std::endl;
    if(error){
        cout<<"[error] default_accept_handler err occur!"<<error.message()<<endl;
        //发生错误;
        return;
    }else{
        handle_socket_request(error,peer);
        static const SomeWhereServer* instance = SomeWhereServer::get_instance();
        instance->set_accept_repeat();
    }
}
#pragma mark 构造析构函数

SomeWhereServer::SomeWhereServer():io_serv(nullptr)
                                    ,acceptor(nullptr)
                                    ,is_server_started(false){
                                        
    io_serv = new io_service;
    if(!io_serv){
        cout<<"[error] alloc io_service failed! exit!"<<endl;
        exit(-1);
    }
    ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"),7796);
    cout<<"[info] the server ip is "<<ep.address()<<endl;
    acceptor = new ip::tcp::acceptor(*io_serv,ep);
                                     
    mysql = new(std::nothrow)MysqlConnect("root","DDKwestbrook0","localhost","somewhere");
    if(!mysql){
        cout<<"[error] mysql init failed!"<<endl;
        exit(-1);
    }
                                        
    redis = new(std::nothrow)RedisConnect;
    if(!redis){
        cout<<"[error] redis init failed!"<<endl;
        exit(-1);
    }
                                        
    accept_handler = nullptr;
    is_server_started = true;
}

SomeWhereServer::~SomeWhereServer(){
    if(io_serv){
        delete io_serv;
        io_serv = nullptr;
    }
    
    if(acceptor){
        delete acceptor;
        acceptor = nullptr;
    }
    
    if(mysql){
        delete mysql;
        mysql = nullptr;
    }
    
    if(redis){
        delete redis;
        redis = nullptr;
    }
}

#pragma mark 私有函数
void SomeWhereServer::start_default_accept() const{
    if(!is_server_started){
        cout<<"[error] server is not start! can't start accept!"<<endl;
        return;
    }
    acceptor->async_accept(default_accept_handler);
}

#pragma mark 公有函数
void SomeWhereServer::do_default_accept() const{
    cout<<"[info] start do default accept"<<endl;
    start_default_accept();
    accept_repeat_flag = true;
    io_serv->run();//开始循环
}


void SomeWhereServer::set_accept_repeat() const{
    if(!accept_repeat_flag){
        cout<<"[error] accept_repeat_flag is false!"<<endl;
        return;
    }
    start_default_accept();
}

void SomeWhereServer::set_accept_handler(void(*accept_handler_t)(socket_ptr client_sock
                                                               ,const boost::system::error_code& err)){
    if(nullptr == accept_handler_t){
        cout<<"[warning] the input accept_handler_t is null,won't set handler!"<<endl;
        return;
    }
    accept_handler = accept_handler_t;
}

MysqlConnect* SomeWhereServer::get_mysql_instance() const{
    return mysql;
}

RedisConnect* SomeWhereServer::get_redis_instance() const{
    return redis;
}


