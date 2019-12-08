//
//  MysqlConnect.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/18.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include "MysqlConnect.hpp"
MysqlConnect::MysqlConnect(string u,
                           string p,
                           string h = "localhost",
                           string d = "somewhere"
                           ):user(u),passwd(p),host(h),database(d),is_connected(false){
    driver = sql::mysql::get_mysql_driver_instance();
    if(nullptr == driver){
        LOGE("init mysql driver instance failed!");
        goto out;
    }
    
    conn = driver->connect(host.c_str(), user.c_str(), passwd.c_str());
    if(nullptr == conn){
        LOGE("connect mysql failed!");
        goto out;
    }
    
    conn->setSchema(database.c_str());
    if(conn->isClosed()){
        LOGE("conn setSchema closed!");
        goto out;
    }
    
    is_connected = true;
out:
    return;
}

MysqlConnect::~MysqlConnect(){
    if(conn){
        conn->close();
        delete conn;
    }
    
    if(stmt){
        stmt->close();
        delete stmt;
    }
    
    if(res){
        res->close();
        delete res;
    }
}

void MysqlConnect::set_sql(string& tmp){
    sql = tmp;
}

void MysqlConnect::clean_sql(){
    sql.clear();
}

bool MysqlConnect::exec_sql(){
    bool ret = false;
    
    if(!is_connected){
        LOGE("mysql is not connect!");
        goto out;
    }
    
    if(sql.empty()){
        LOGE("sql is empty!");
        goto out;
    }
    
    stmt = conn->createStatement();
    if(!stmt){
        LOGE("createStatement failed!");
        goto out;
    }
    
    res = stmt->executeQuery(sql.c_str());
    if(!res){
        LOGE("executeQuery failed!");
        goto out;
    }
    
    stmt->close();
    
    ret = true;
out:
    return ret;
}

bool MysqlConnect::exec_update_sql(){
    bool ret = false;
    
    if(!is_connected){
        LOGE("mysql is not connect!");
        goto out;
    }
    
    if(sql.empty()){
        LOGE("sql is empty!");
        goto out;
    }
    
    stmt = conn->createStatement();
    if(!stmt){
        LOGE("createStatement failed!");
        goto out;
    }
    
    stmt->executeUpdate(sql.c_str()) >= 1? ret = true : ret = false;
    
    stmt->close();
out:
    return ret;
}


sql::ResultSet* MysqlConnect::get_res(){
    return res;
}

bool MysqlConnect::get_conn_state(){
    return is_connected;
}
