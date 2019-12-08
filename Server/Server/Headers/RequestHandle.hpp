//
//  RequestHandle.hpp
//  Server
//
//  Created by dong da kuan on 2019/11/25.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef RequestHandle_hpp
#define RequestHandle_hpp

#include <stdio.h>
#include <iostream>
#include <string>

#include "MysqlConnect.hpp"
#include "RedisConnect.hpp"
#include "SomeWhereLog.h"

using namespace std;
class RequestHandle{
public:
    static string redis_login_op(RedisConnect* redis,string& exec_redis);

    static string mysql_login_op(MysqlConnect* mysql,string& exec_sql);
    
    static bool mysql_signup_op(MysqlConnect* mysql,string& exec_sql);
};
#endif /* RequestHandle_hpp */
