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
            cout<<"[error] redis connect:%s"<<conn->errstr<<endl;
            goto out;
        }else{
            cout<<"[error] can't allocate redis context"<<endl;
            goto out;
        }
    }
    ret = true;
out:
    return ret;
    
}
RedisConnect::RedisConnect(string h, int p ):
               host(h),port(p),is_connected(false) {
    conn = nullptr;
    reply = nullptr;
    cmd.clear();
    if(!connect()){
        cout<<"[error] connect redis failed!"<<endl;
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

bool RedisConnect::exec_cmd(){
    bool ret = false;
    if(!is_connected){
        cout<<"[warning] redis is not connected ,can't execute cmd"<<endl;
        goto out;
    }
    
    if(cmd.empty()){
        cout<<"[warning] cmd is empty!"<<endl;
        goto out;
    }
    
    if(!reply){
        cout<<"[warning] clean the redis reply before execute cmd"<<endl;
        goto out;
    }
    reply = (redisReply*)redisCommand(conn, cmd.c_str());
    if(nullptr == reply){
        cout<<"[error] redis command execute failed: "<<conn->err<<": "<<conn->errstr<<endl;
        redisFree(conn);//这里出错后需要进行重连
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
