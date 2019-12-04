//
//  RedisConnect.hpp
//  Server
//
//  Created by dong da kuan on 2019/11/18.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef RedisConnect_hpp
#define RedisConnect_hpp

#include <iostream>
#include <hiredis.h>
using namespace std;
class RedisConnect{
    private:
        redisContext* conn;//这不是一个线程安全的对象
        redisReply* reply;
    
    private:
        string host;
        int port;
    
    private:
        bool is_connected;
        string cmd;
    
    private:
        bool connect();
        void free_redis_connect();
    
    public:
        void set_cmd(string& tmp);
        bool exec_cmd();
        redisReply* get_reply();
        void clean_reply();
        bool get_redis_status();
    
    public:
        RedisConnect(string h = "127.0.0.1", int p = 6379);
        ~RedisConnect();
};



#endif /* RedisConnect_hpp */
