//
//  SomeWhere_Login.hpp
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef SomeWhere_Login_hpp
#define SomeWhere_Login_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <include/mysql/jdbc.h>
using namespace std;
class Login{
private:
    string user;
    string passwd;
    string host;
    string database;
    string sql;
private:
    sql::mysql::MySQL_Driver* driver = nullptr;
    sql::Connection* conn = nullptr;
    sql::Statement* stmt = nullptr;
    sql::ResultSet* res = nullptr;
public:
    Login();
    ~Login();
public:
    bool initDB(string user,string passwd,string host = "localhost",string database = "somewhere");
    void setSql(string& tmp);
    void cleanSql();
    bool excSQL();
    sql::ResultSet* getRes();
};


#endif /* SomeWhere_Login_hpp */
