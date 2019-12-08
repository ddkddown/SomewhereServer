//
//  RequestHandle.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/25.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include "RequestHandle.hpp"
string RequestHandle::redis_login_op(RedisConnect* redis,string& exec_redis){
    string ret;
    redisReply* redis_reply = nullptr;
    
    ret.clear();
    
    if(redis == nullptr){
        LOGE("redis is null!");
        goto out;
    }
    
    if(!redis->get_redis_status()){
        LOGE("redis is not connect");
        goto out;
    }
    
    redis->set_cmd(exec_redis);
    if(!redis->exec_cmd()){
        LOGE("redis exec login cmd failed!");
        goto out;
    }
    
    redis_reply = redis->get_reply();
    
    if (!redis_reply){
        LOGE("after get_reply redis_reply is null!");
        goto out;
    }
    
    //redis查询成功
    if(redis_reply->type == REDIS_REPLY_STRING){
        ret = redis_reply->str;
    }else{
        LOGE("redis reply type wrong:%d str:%s",redis_reply->type,redis_reply->str);
        goto out;
    }
    
   
out:
    redis->clean_reply();
    return ret;
}

string RequestHandle::mysql_login_op(MysqlConnect *mysql, string &exec_sql){
    string ret;
    sql::ResultSet* mysql_reply = nullptr;
    ret.clear();
    
    if(!mysql){
        LOGE("get mysql,failed,return");
        goto out;
    }
    
    if(!mysql->get_conn_state()){
        LOGE("mysql is not connect!");
        goto out;
    }
    
    mysql->set_sql(exec_sql);
    if(!mysql->exec_sql()){
        LOGE("mysql exec login sql failed!,return");
        goto out;
    }
    
    mysql_reply = mysql->get_res();
    ret = mysql_reply->getString(2);
out:
    return ret;
}

bool RequestHandle::mysql_signup_op(MysqlConnect *mysql, string &exec_sql){
    bool ret = false;
    
    if(!mysql){
        LOGE("get mysql,failed,return");
        goto out;
    }
    
    if(!mysql->get_conn_state()){
        LOGE("mysql is not connect!");
        goto out;
    }
    
    mysql->set_sql(exec_sql);
    if(!mysql->exec_update_sql()){
        LOGE("mysql exec signup sql failed!, return");
        goto out;
    }else{
        LOGI("mysql exec signup sql success!return");
    }
    
    ret = true;
out:
    return ret;
}

