//
//  SomeWhereServer.hpp
//  Server
//
//  Created by dong da kuan on 2019/11/18.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef SomeWhereServer_hpp
#define SomeWhereServer_hpp

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread/thread.hpp>

#include "SomeHeader.h"
#include "SomeWhereLog.h"


#define BOOST_SPIRIT_THREADSAFE
#define END_CHAR '0'

using namespace std;
using namespace boost::asio;

class SomeWhereServer{
    
    typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
    typedef MysqlConnect Mysql;
    typedef RedisConnect Redis;
    
#pragma mark 变量
    private:
        io_service* io_serv;//boost.asio使用io_service同操作系统的输入/输出进行交互
        ip::tcp::acceptor* acceptor;
    
        string mysql_name;
        string mysql_password;
        string mysql_host;
        string mysql_database;
        string redis_host;
        string redis_port;
        string server_ip;
        string server_port;
    
    private:
        const static SomeWhereServer* m_instance;
    
    
        bool is_server_started;
        static bool accept_repeat_flag;
        int thread_num;//线程数量
    
    private:
        class Garbo{//用于在析构函数中删除单例类的实例
            public:
                ~Garbo(){
                    if(SomeWhereServer::m_instance){
                        delete SomeWhereServer::m_instance;
                    }
                }
            };
        static Garbo m_garbo;
    
        class MysqlConnect* mysql;
        class RedisConnect* redis;
    
#pragma mark 函数
    private:
        void (*accept_handler)(socket_ptr client_sock
                           ,const boost::system::error_code& err);
    
    private:
        void init_config();
        void start_default_accept() const;
        SomeWhereServer();
    
    
    public:
        void set_accept_handler(void (*accept_handler_t)(socket_ptr client_sock
                                        ,const boost::system::error_code& err));
    public:
        static const SomeWhereServer* get_instance(){
            return m_instance;
        }
        ~SomeWhereServer();
    
        void set_accept_repeat() const;//必须要先调用一次do_default_accept后才能调用，由accept_repeat_flag控制
        void do_default_accept() const;
        Mysql* get_mysql_instance() const;
        Redis* get_redis_instance() const;
    

};
#endif /* SomeWhereServer_hpp */
