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
        cout<<"[error] redis is null!"<<endl;
        goto out;
    }
    
    if(!redis->get_redis_status()){
        cout<<"[error] redis is not connect"<<endl;
        goto out;
    }
    
    redis->set_cmd(exec_redis);
    if(!redis->exec_cmd()){
        cout<<"[error] redis exec login cmd failed!";
        goto out;
    }
    
    redis_reply = redis->get_reply();
    
    if (!redis_reply){
        cout<<"[error] after get_reply redis_reply is null!"<<endl;
        goto out;
    }
    
    //redis查询成功
    if(redis_reply->type == REDIS_REPLY_STRING){
        ret = redis_reply->str;
    }else{
        cout<<"redis reply type wrong :"<<redis_reply->type<<endl;
        goto out;
    }
    
    redis->clean_reply();
out:
    return ret;
}

string RequestHandle::mysql_login_op(MysqlConnect *mysql, string &exec_sql){
    string ret;
    sql::ResultSet* mysql_reply = nullptr;
    ret.clear();
    
    if(!mysql){
        cout<<"[error] get mysql,failed,return"<<endl;
        goto out;
    }
    
    if(!mysql->get_conn_state()){
        cout<<"[error] mysql is not connect!"<<endl;
        goto out;
    }
    
    mysql->set_sql(exec_sql);
    if(!mysql->exec_sql()){
        std::cout<<"[error] mysql exec login sql failed!,return";
        goto out;
    }
    
    mysql_reply = mysql->get_res();
    ret = mysql_reply->getString(2);
out:
    return ret;
}
