//
//  MysqlConnect.hpp
//  Server
//
//  Created by dong da kuan on 2019/11/18.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef MysqlConnect_hpp
#define MysqlConnect_hpp

#include <iostream>
#include <string>
#include <include/mysql/jdbc.h>

#include "SomeWhereLog.h"

using namespace std;

class MysqlConnect{
    private:
        string user;
        string passwd;
        string host;
        string database;
    
        bool is_connected;
    private:
        string sql;
    
    private:
        sql::mysql::MySQL_Driver* driver = nullptr;
        sql::Connection* conn = nullptr;
        sql::Statement* stmt = nullptr;
        sql::ResultSet* res = nullptr;
    
    public:
        MysqlConnect(string u,string p,string h,string data);
        ~MysqlConnect();
    
    public:
        void set_sql(string& tmp);
        void clean_sql();
        bool exec_sql();
        bool exec_update_sql();//用于执行insert，update之类的sql
    
        sql::ResultSet* get_res();
        bool get_conn_state();
};

#endif /* MysqlConnect_hpp */
