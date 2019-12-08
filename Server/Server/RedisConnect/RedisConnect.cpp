//
//  RedisConnect.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/18.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include "RedisConnect.hpp"
bool RedisConnect::connect(){
    bool ret = false;
    
    if(!conn){
        redisFree(conn);
    }
    
    conn = redisConnect(host.c_str(), port);
    if(nullptr == conn || conn->err){
        if(conn){
            LOGE("redis connect error :%s",conn->errstr);
            goto out;
        }else{
            LOGE("can't allocate redis context");
            goto out;
        }
    }
    ret = true;
    is_connected = true;
out:
    return ret;
    
}
RedisConnect::RedisConnect(string h, int p ):
               host(h),port(p),is_connected(false) {
    conn = nullptr;
    reply = nullptr;
    cmd.clear();
    if(!connect()){
        LOGE("[error] connect redis failed!");
        return;
    }
    
    is_connected = true;
}

RedisConnect::~RedisConnect(){
    if(reply){
        freeReplyObject(reply);
        reply = nullptr;
    }
    
    if(conn){
        redisFree(conn);
        conn = nullptr;
    }
    
    is_connected = false;
}

void RedisConnect::set_cmd(string& tmp){
    cmd = tmp;
}

void RedisConnect::free_redis_connect(){
    redisFree(conn);
    is_connected = false;
}

bool RedisConnect::exec_cmd(){
    bool ret = false;
    if(!is_connected){
        LOGE("redis is not connected ,can't execute cmd");
        goto out;
    }
    
    if(cmd.empty()){
        LOGE("cmd is empty!");
        goto out;
    }
    
    if(!reply){
        LOGW("clean the redis reply before execute cmd");
        clean_reply();
    }
    
    reply = (redisReply*)redisCommand(conn, cmd.c_str());
    if(nullptr == reply){
        LOGE("redis command execute failed,err:%d,errstr:%s",conn->err,conn->errstr);
        free_redis_connect();//这里出错后需要进行重连
        connect();
        cmd.clear();
        goto out;
    }
    
    ret = true;
out:
    return ret;
}

redisReply* RedisConnect::get_reply(){
    return reply;
}

void RedisConnect::clean_reply(){
    if(reply){
        freeReplyObject(reply);
        reply = nullptr;
    }
}

bool RedisConnect::get_redis_status(){
    return is_connected;
}
