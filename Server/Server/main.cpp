//
//  main.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "SomeHeader.h"
#include "SomeWhere_Login.hpp"
#include "SomeWhereServer.hpp"
using namespace boost::asio;

Login mysql_db;
io_service iosev;
void func(int i){
    std::cout<<"get func"<<i<<std::endl;
}
void handler(const boost::system::error_code& error,
             size_t bytes_transferred){
    std::cout<<"get in async read"<<std::endl;
    if(!error)
        std::cout<<"async read!"<<endl;
}

void post_handler(boost::asio::ip::tcp::socket& peer){
    char buf[sizeof(somewhere_message)];
    size_t len = peer.read_some(buffer(buf));
    if(len > 0){
        std::cout<<len<<std::endl;
        std::cout<<buf<<std::endl;
        
        somewhere_message client_message;
        std::string sql1;
        std::string sql2;
        memcpy(&client_message, buf, sizeof(somewhere_message));
        if(LOGIN_MESSAGE == client_message.type){
            int username_length = 0;
            int password_length = 0;
            for(username_length = 0; username_length < NAME_LENGTH &&
                client_message.user_name[username_length] != '\xff'; ++username_length){}
            for(password_length = 0; password_length < PASSWORD_LENGTH &&
                client_message.password[password_length] != '\xff'; ++password_length){}
            char username[username_length];
            memset(username, 0, sizeof(username));
            //memcpy(username, client_message.user_name, (username_length*sizeof(char)));
            for (int i = 0; i < username_length; ++i) {
                username[i] = client_message.user_name[i];//不知道为什么字符串数组后面会带其它字符。先这样处理下。
                sql1.push_back(username[i]);
            }
            //std::cout<<username<<std::endl;
            char password[password_length];
            memset(password, 0, sizeof(password));
            //memcpy(password, client_message.password, (password_length*sizeof(char)));
            for (int i = 0; i < password_length; ++i) {
                password[i] = client_message.password[i];//不知道为什么字符串数组后面会带其它字符。先这样处理下。
                sql2.push_back(password[i]);
            }
            //std::cout<<password<<std::endl;
            std::cout<<sql1<<sql2<<std::endl;
            std::string sql = "select * from somewhere_login where login_id = '";
            sql.append(sql1);
            sql.append("';");
            std::cout<<sql<<std::endl;
            mysql_db.setSql(sql);
            mysql_db.excSQL();
            auto res_tmp = mysql_db.getRes();
            reply_message reply_to_client;
            reply_to_client.type = LOGIN_MESSAGE;
            reply_to_client.status = FAIL_STATUS;
            if(!res_tmp->next()){
                std::cout<<"sql result null"<<std::endl;
                //返回结果为空
            }
            else{
                std::cout<<"sql result get"<<std::endl;
                std::string sql_password_result = res_tmp->getString(2);
                std::cout<<"sql password result:"<<sql_password_result<<std::endl;
                if(0 == sql_password_result.compare(sql2)){
                    std::cout<<"sql compare success!"<<std::endl;
                    reply_to_client.status = SUCCESS_STATUS;
                }
            }
            char reply_buf[sizeof(reply_message)];
            memcpy(reply_buf, &reply_to_client, sizeof(reply_message));
            peer.write_some(buffer(reply_buf));
        }
    }
}
void accept_handler(const boost::system::error_code& error,
                    boost::asio::ip::tcp::socket peer){
    std::cout<<"enter1!"<<std::endl;
    if(!error){
        while(1){
            //对一次socket请求的处理过程
            std::cout<<"here"<<std::endl;
            boost::system::error_code ec;
            peer.wait(peer.wait_read,ec);
            if(!ec){
                try{
                    char buf[sizeof(somewhere_message)];
                    //peer.async_read_some(buffer(buf), handler);
                    //iosev.post(boost::bind(post_handler,peer));
                    iosev.post(boost::bind(func,1));
                    size_t len = peer.read_some(buffer(buf));
                    if(len > 0){
                        std::cout<<len<<std::endl;
                        std::cout<<buf<<std::endl;
                        
                        somewhere_message client_message;
                        std::string sql1;
                        std::string sql2;
                        memcpy(&client_message, buf, sizeof(somewhere_message));
                        if(LOGIN_MESSAGE == client_message.type){
                            int username_length = 0;
                            int password_length = 0;
                            for(username_length = 0; username_length < NAME_LENGTH &&
                                client_message.user_name[username_length] != '\xff'; ++username_length){}
                            for(password_length = 0; password_length < PASSWORD_LENGTH &&
                                client_message.password[password_length] != '\xff'; ++password_length){}
                            char username[username_length];
                            memset(username, 0, sizeof(username));
                            //memcpy(username, client_message.user_name, (username_length*sizeof(char)));
                            for (int i = 0; i < username_length; ++i) {
                                username[i] = client_message.user_name[i];//不知道为什么字符串数组后面会带其它字符。先这样处理下。
                                sql1.push_back(username[i]);
                            }
                            //std::cout<<username<<std::endl;
                            char password[password_length];
                            memset(password, 0, sizeof(password));
                            //memcpy(password, client_message.password, (password_length*sizeof(char)));
                            for (int i = 0; i < password_length; ++i) {
                                password[i] = client_message.password[i];//不知道为什么字符串数组后面会带其它字符。先这样处理下。
                                sql2.push_back(password[i]);
                            }
                            //std::cout<<password<<std::endl;
                            std::cout<<sql1<<sql2<<std::endl;
                            std::string sql = "select * from somewhere_login where login_id = '";
                            sql.append(sql1);
                            sql.append("';");
                            std::cout<<sql<<std::endl;
                            mysql_db.setSql(sql);
                            mysql_db.excSQL();
                            auto res_tmp = mysql_db.getRes();
                            reply_message reply_to_client;
                            reply_to_client.type = LOGIN_MESSAGE;
                            reply_to_client.status = FAIL_STATUS;
                            if(!res_tmp->next()){
                                std::cout<<"sql result null"<<std::endl;
                                //返回结果为空
                            }
                            else{
                                std::cout<<"sql result get"<<std::endl;
                                std::string sql_password_result = res_tmp->getString(2);
                                std::cout<<"sql password result:"<<sql_password_result<<std::endl;
                                if(0 == sql_password_result.compare(sql2)){
                                    std::cout<<"sql compare success!"<<std::endl;
                                    reply_to_client.status = SUCCESS_STATUS;
                                }
                            }
                            char reply_buf[sizeof(reply_message)];
                            memcpy(reply_buf, &reply_to_client, sizeof(reply_message));
                            peer.write_some(buffer(reply_buf));
                        }
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
    else{
        std::cout<<"error"<<std::endl;
    }
}
int main(int argc, const char * argv[]) {
    auto i = SomeWhereServer::get_instance();
    i->do_default_accept();
    /*std::cout<<sizeof(somewhere_message)<<std::endl;
    //Login mysql_db;
    mysql_db.initDB("root", "DDKwestbrook0");
    
    //for(int i = 0;i < 10; ++i){
    // iosev.run();
    //}
    ip::tcp::acceptor acceptor (iosev,ip::tcp::endpoint(ip::address_v4::from_string("127.0.0.1"), 7796));
    //iosev.run();
    while (1) {
        //  ip::tcp::socket socket(iosev);
        //acceptor.async_accept(socket);
        //acceptor.accept(socket);
        acceptor.wait(acceptor.wait_read);
        iosev.reset();
        std::cout<<"read!!"<<std::endl;
        acceptor.async_accept(accept_handler);
        //boost::asio::io_service::work work(iosev);
        iosev.run();
        std::cout<<"hi"<<std::endl;
        /* while(1){
         //对一次socket请求的处理过程
         
         socket.wait(socket.wait_read);
         try{
         char buf[sizeof(somewhere_message)];
         size_t len = socket.read_some(buffer(buf));
         if(len > 0){
         std::cout<<len<<std::endl;
         std::cout<<buf<<std::endl;
         
         somewhere_message client_message;
         std::string sql1;
         std::string sql2;
         memcpy(&client_message, buf, sizeof(somewhere_message));
         if(LOGIN_MESSAGE == client_message.type){
         int username_length = 0;
         int password_length = 0;
         for(username_length = 0; username_length < NAME_LENGTH &&
         client_message.user_name[username_length] != '\xff'; ++username_length){}
         for(password_length = 0; password_length < PASSWORD_LENGTH &&
         client_message.password[password_length] != '\xff'; ++password_length){}
         char username[username_length];
         memset(username, 0, sizeof(username));
         //memcpy(username, client_message.user_name, (username_length*sizeof(char)));
         for (int i = 0; i < username_length; ++i) {
         username[i] = client_message.user_name[i];//不知道为什么字符串数组后面会带其它字符。先这样处理下。
         sql1.push_back(username[i]);
         }
         //std::cout<<username<<std::endl;
         char password[password_length];
         memset(password, 0, sizeof(password));
         //memcpy(password, client_message.password, (password_length*sizeof(char)));
         for (int i = 0; i < password_length; ++i) {
         password[i] = client_message.password[i];//不知道为什么字符串数组后面会带其它字符。先这样处理下。
         sql2.push_back(password[i]);
         }
         //std::cout<<password<<std::endl;
         std::cout<<sql1<<sql2<<std::endl;
         std::string sql = "select * from somewhere_login where login_id = '";
         sql.append(sql1);
         sql.append("';");
         std::cout<<sql<<std::endl;
         mysql_db.setSql(sql);
         mysql_db.excSQL();
         auto res_tmp = mysql_db.getRes();
         reply_message reply_to_client;
         reply_to_client.type = LOGIN_MESSAGE;
         reply_to_client.status = FAIL_STATUS;
         if(!res_tmp->next()){
         std::cout<<"sql result null"<<std::endl;
         //返回结果为空
         }
         else{
         std::cout<<"sql result get"<<std::endl;
         std::string sql_password_result = res_tmp->getString(2);
         std::cout<<"sql password result:"<<sql_password_result<<std::endl;
         if(0 == sql_password_result.compare(sql2)){
         std::cout<<"sql compare success!"<<std::endl;
         reply_to_client.status = SUCCESS_STATUS;
         }
         }
         char reply_buf[sizeof(reply_message)];
         memcpy(reply_buf, &reply_to_client, sizeof(reply_message));
         socket.write_some(buffer(reply_buf));
         }
         }
         }catch(std::exception& e){
         std::cout<<e.what()<<std::endl;
         socket.close();
         break;
         }
         
         //对一次socket请求的处理过程
         }*/
 //   }
}

