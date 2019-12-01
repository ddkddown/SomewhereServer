//
//  SomeWhere_Login.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include "SomeWhere_Login.hpp"
Login::Login(){
    driver = sql::mysql::get_mysql_driver_instance();
    if(NULL == driver){
        cout<<"init Login error!"<<std::endl;
        exit(-1);
    }
}

Login::~Login(){
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

bool Login::initDB(string user, string passwd,string host,string database){
    bool ret = false;
    
    conn = driver->connect(host.c_str(), user.c_str(), passwd.c_str());
    if(NULL == conn){
        std::cout<<"connect mysql failed!"<<std::endl;
        goto out;
    }
    else{
        std::cout<<"connect mysql success!"<<std::endl;
    }
    
    conn->setSchema(database.c_str());
    if(conn->isClosed())
    {
        std::cout<<"conn is closed"<<std::endl;
        goto out;
    }
    
    ret = true;
out:
    return ret;
}

void Login::setSql(string &tmp){
    sql = tmp;
}

void Login::cleanSql(){
    sql.clear();
}

bool Login::excSQL(){
    if (sql.empty()) {
        return false;
    }
    stmt = conn->createStatement();
    res = stmt->executeQuery(sql.c_str());
    return true;
}

sql::ResultSet* Login::getRes(){
    return res;
}
