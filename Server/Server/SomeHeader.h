//
//  SomeHeader.h
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef SomeHeader_h
#define SomeHeader_h
#include <iostream>
#include <string>
#include "MysqlConnect.hpp"
#include "RedisConnect.hpp"
#include "RequestHandle.hpp"

#define NAME_LENGTH 20
#define PASSWORD_LENGTH 20
#define MESSAGE_LENGTH 512
#define MESSAGE_END 5
#define MESSAGE_NOT_END 6

#define LOGIN_MESSAGE 1
#define DATA_MESSAGE 2

#define SUCCESS_STATUS 1
#define FAIL_STATUS 0

typedef struct somewhere_message{
    uint8_t type;
    uint8_t is_end;//代表这一次的消息是否已经传输结束
    uint8_t message_length;
    char user_name[NAME_LENGTH];//每一次客户端发来的消息都必须带有用户名以及密码,每一次都会对用户名和密码进行验证
    char password[PASSWORD_LENGTH];
    char message_body[MESSAGE_LENGTH];
}somewhere_message;

typedef struct reply_message{
    uint8_t type;
    uint8_t status;
}reply_message;


#endif /* SomeHeader_h */
