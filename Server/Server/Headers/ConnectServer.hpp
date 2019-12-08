//
//  ConnectServer.hpp
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef ConnectServer_h
#define ConnectServer_h
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "SomeWhereLog.h"

namespace ddk{
    //连接服务器的类
    class ConnectServer{
    private:
        std::string server_ip;
        int         server_port;
        int         threads_num = 0;
        
        std::fstream finput;
        
    private:
        bool is_connected = false;
        
    protected:
        std::string  send_buff;
        std::string  recv_buff;
        
    public:
        ConnectServer(){
            finput.open("./conf/ip_port", std::fstream::in);
            if(finput.is_open()){
                std::string ip,port;
                finput>>ip>>port;
                LOGI("server_ip:%s, server_port:%s",ip.c_str(),port.c_str());
                server_ip = ip;
                try{
                    server_port = boost::lexical_cast<int>(port.c_str());
                }catch (boost::bad_lexical_cast& e){
                    LOGE("convert server_port error:%s",e.what());
                    server_ip.clear();
                    server_port = -1;
                }
            }
            else{
                LOGI("get server ip and port error ,set it to -1");
                server_ip.clear();server_port = -1;
            }
        }
        virtual ~ConnectServer(){};
        
    public:
        virtual bool connect_to_server() = 0;
        void set_server_ip(std::string tmp){ server_ip = tmp;}
        std::string& get_server_ip(){ return server_ip;}
        void clean_server_ip(){ server_ip.clear();}
        void set_server_port(int tmp){ server_port = tmp;}
        int  get_server_port(){ return server_port;}
        void clean_server_port(){ server_port = -1;}
        void set_send_buffer(std::string& tmp){ send_buff = tmp;}
        std::string get_send_buffer(){ return send_buff;}
        std::string get_recv_buffer(){ return recv_buff;}
        void set_status(bool status){ is_connected = status;}
        bool get_status(){  return is_connected;}
        void set_threads_num(int num){ threads_num = num; }
        int get_threads_num(){ return threads_num; }
    };
    
}



#endif /* ConnectServer_h */
