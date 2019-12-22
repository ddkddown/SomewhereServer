//
//  SomeWhereServer.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/18.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include "SomeWhereServer.hpp"

const SomeWhereServer* SomeWhereServer::m_instance = new SomeWhereServer;
bool SomeWhereServer::accept_repeat_flag = false;

static void service_to_run(io_service* io_serv){
    if(io_serv == nullptr){
        LOGE("io_serv is null!");
        exit(1);
    }
    io_serv->run();
}

static void send_login_back_message(boost::asio::ip::tcp::socket& peer,bool state){
    reply_message back_message;
    boost::system::error_code ec;
    char buf[sizeof(reply_message)];
    
    memset(buf, -1, sizeof(buf));
    back_message.type = LOGIN_MESSAGE;
    back_message.status = state;
    memcpy(buf, &back_message, sizeof(reply_message));
    
    peer.write_some(buffer(buf),ec);
    if (ec) {
        LOGE("send_login_back_message failed:%s",ec.message().c_str());
    }
}

static void send_signup_back_message(boost::asio::ip::tcp::socket& peer,bool state){
    reply_message back_message;
    boost::system::error_code ec;
    char buf[sizeof(reply_message)];
    
    memset(buf, -1, sizeof(buf));
    back_message.type = SIGNUP_MESSAGE;
    back_message.status = state;
    memcpy(buf, &back_message, sizeof(reply_message));
    
    peer.write_some(buffer(buf),ec);
    if (ec) {
        LOGE("send_signup_back_message failed:%s",ec.message().c_str());
    }
}

static void put_mysql_data_to_redis(string& username,string& result){
    std::string exec_redis = "set ";
    redisReply* redis_reply = nullptr;
    exec_redis += username;
    exec_redis += " ";
    exec_redis += result;
    
    auto redis = SomeWhereServer::get_instance()->get_redis_instance();
    
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
    if(redis_reply->type == REDIS_REPLY_STATUS && strncmp(redis_reply->str, "OK", sizeof(redis_reply->str))){
        LOGI("put login data to redis successed!");
    }else{
        LOGE("redis reply type wrong :%d, redis str:%s",redis_reply->type,redis_reply->str);
        goto out;
    }
    
out:
    redis->clean_reply();
    return;
}
static bool handle_login_message(somewhere_message& client_message,boost::asio::ip::tcp::socket& peer){
    std::string username;
    std::string password;
    std::string result;
    
    bool put_flag = false;
    
    std::string exec_redis = "get ";
    std::string exec_sql = "select * from somewhere_login where login_id = '";
    
    
    int username_length = 0;
    int password_length = 0;
    
    for(username_length = 0; username_length < NAME_LENGTH &&
        client_message.user_name[username_length] != '\xff'; ++username_length){
        
        username.push_back(client_message.user_name[username_length]);
    }
    for(password_length = 0; password_length < PASSWORD_LENGTH &&
        client_message.password[password_length] != '\xff'; ++password_length){
        
        password.push_back(client_message.password[password_length]);
    }
    
    LOGI("handle login message username:%s password:%s",username.c_str(),password.c_str());

    exec_redis.append(username);
    
    //先通过redis查询
    LOGI("try search by redis");
    result = RequestHandle::redis_login_op(SomeWhereServer::get_instance()->get_redis_instance(),exec_redis);
    if(!result.empty()){
        goto out;
    }
    
    result.clear();
    
    //redis查询失败时用 mysql查询
    put_flag = true;
    exec_sql.append(username);
    exec_sql.append("';");
    
    LOGI("try search by mysql");
    result = RequestHandle::mysql_login_op(SomeWhereServer::get_instance()->get_mysql_instance(),exec_sql);
    if(!result.empty()){
        goto out;
    }
    
out:

    //比较字符串,返回密码是否正确
    if(0 == password.compare(result)){
        LOGI("password right");
        //将mysql数据放到redis上
        if(put_flag){
            put_mysql_data_to_redis(username, result);
        }
        return true;
    }else {
        LOGI("password wrong");
        return false;
    }
}

static bool handle_signup_message(somewhere_message& client_message,boost::asio::ip::tcp::socket& peer){
    std::string username;
    std::string password;
    std::string result;
    
    std::string exec_sql = "INSERT INTO somewhere_login VALUES(\"";
    
    int username_length = 0;
    int password_length = 0;
    
    for(username_length = 0; username_length < NAME_LENGTH &&
        client_message.user_name[username_length] != '\xff'; ++username_length){
        
        username.push_back(client_message.user_name[username_length]);
    }
    for(password_length = 0; password_length < PASSWORD_LENGTH &&
        client_message.password[password_length] != '\xff'; ++password_length){
        
        password.push_back(client_message.password[password_length]);
    }
    
    LOGI("handle signup message username :%s password:%s",username.c_str(),password.c_str());
    exec_sql.append(username);
    exec_sql.append("\",");
    exec_sql.append(password);
    exec_sql.append(");");
    
    LOGI("try signup by mysql:%s",exec_sql.c_str());
    
    return RequestHandle::mysql_signup_op(SomeWhereServer::get_instance()->get_mysql_instance(), exec_sql);
}

static void handle_message(somewhere_message& client_message,boost::asio::ip::tcp::socket& peer){
    
    switch (client_message.type) {
        case LOGIN_MESSAGE:
            if(handle_login_message(client_message,peer)){
                send_login_back_message(peer,true);
            }else{
                send_login_back_message(peer,false);
            }
            break;
        
        case SIGNUP_MESSAGE:
            if(handle_signup_message(client_message,peer)){
                send_signup_back_message(peer,true);
            }else{
                send_signup_back_message(peer,false);
            }
            break;
        default:
            LOGE("wrong message type:%s",client_message.message_body);
            break;
    }
    
}

static void handle_socket_request(const boost::system::error_code& error,
                                    boost::asio::ip::tcp::socket& peer){
        while(1){
            //对一次socket请求的处理过程
            boost::system::error_code ec;
            peer.wait(peer.wait_read,ec);
            if(!ec){
                try{
                    char buf[sizeof(somewhere_message)];
                    size_t len = peer.read_some(buffer(buf));
                    if(len > 0){
                        somewhere_message client_message;
                        memcpy(&client_message, buf, sizeof(somewhere_message));
                        handle_message(client_message, peer);
                    }
                }catch(std::exception& e){
                    LOGE("handle socket request read some error :%s",e.what());
                    peer.close();
                }
            }
            else{
                LOGE("handle socket request error :%s",ec.message().c_str());
                return;
            }
            //对一次socket请求的处理过程
        }
}

static void default_accept_handler(const boost::system::error_code& error,
                                    boost::asio::ip::tcp::socket peer){
    LOGI("get an client request");
    if(error){
        LOGE("default_accept_handler err occur:%s",error.message().c_str());
        //发生错误;
        return;
    }else{
        handle_socket_request(error,peer);
        static const SomeWhereServer* instance = SomeWhereServer::get_instance();
        instance->set_accept_repeat();
    }
}
#pragma mark 构造析构函数

SomeWhereServer::SomeWhereServer():io_serv(nullptr)
                                    ,acceptor(nullptr)
                                    ,is_server_started(false)
                                    ,thread_num(10){
                                        
    io_serv = new io_service;
    if(!io_serv){
        LOGI("alloc io_service failed! exit!");
        exit(1);
    }
    init_config();
    ip::tcp::endpoint ep(ip::address_v4::from_string(server_ip.c_str()),atoi(server_port.c_str()));
    LOGI("the server ip is:%s,port is%d",server_ip.c_str(),atoi(server_port.c_str()));
    acceptor = new ip::tcp::acceptor(*io_serv,ep);
                                     
    mysql = new(std::nothrow)MysqlConnect(mysql_name.c_str(),mysql_password.c_str(),mysql_host.c_str(),mysql_database.c_str());
    if(!mysql){
        LOGE("mysql init failed!");
        exit(1);
    }
    else{
        LOGI("mysql init success!");
    }
                                        
    redis = new(std::nothrow)RedisConnect(redis_host.c_str(),atoi(redis_port.c_str()));
    if(!redis){
        LOGE("redis init failed!");
        exit(1);
    }
    else{
        LOGI("redis init success!");
    }
                                        
    accept_handler = nullptr;
    is_server_started = true;
}

SomeWhereServer::~SomeWhereServer(){
    if(io_serv){
        delete io_serv;
        io_serv = nullptr;
    }
    
    if(acceptor){
        delete acceptor;
        acceptor = nullptr;
    }
    
    if(mysql){
        delete mysql;
        mysql = nullptr;
    }
    
    if(redis){
        delete redis;
        redis = nullptr;
    }
}

#pragma mark 私有函数
void SomeWhereServer::start_default_accept() const{
    if(!is_server_started){
        LOGE("server is not start! can't start accept!");
        return;
    }
    acceptor->async_accept(default_accept_handler);
}

#pragma mark 公有函数
void SomeWhereServer::do_default_accept() const{
    LOGI("start do default accept");
    start_default_accept();
    accept_repeat_flag = true;
    boost::thread_group tg;
    
    for(int i = 0;i < thread_num;++i){
        tg.create_thread(boost::bind(service_to_run,io_serv));
    }
    tg.join_all();
}


void SomeWhereServer::set_accept_repeat() const{
    if(!accept_repeat_flag){
        LOGE("accept_repeat_flag is false!");
        return;
    }
    start_default_accept();
}

void SomeWhereServer::set_accept_handler(void(*accept_handler_t)(socket_ptr client_sock
                                                               ,const boost::system::error_code& err)){
    if(nullptr == accept_handler_t){
        LOGW("the input accept_handler_t is null,won't set handler!");
        return;
    }
    accept_handler = accept_handler_t;
}

void SomeWhereServer::init_config(){
    ifstream fs;
    boost::property_tree::ptree ptree;
    
    fs.open("/Users/dongdakuan/Downloads/SomewhereServer-git/SomewhereServer/Server/Server/conf/server_config.json", fstream::in);
    
    if(!fs.is_open()){
        LOGE("open server_config.json failed!");
        exit(1);
    }
    
    string s((std::istreambuf_iterator<char>(fs)),
                std::istreambuf_iterator<char>());
    stringstream ss(s);
    
    try{
        boost::property_tree::read_json(ss, ptree);
    }catch(exception e){
        LOGE("read json failed! check the json file!");
        exit(1);
    }
    
    auto node = ptree.get_child_optional("MysqlName")->get_value<string>();
    if(node.empty()){
        LOGW("get MysqlName failed!");
    }else{
        mysql_name = node;
    }
    
    node = ptree.get_child_optional("MysqlPassword")->get_value<string>();
    if(node.empty()){
        LOGW("get MysqlPassword failed!");
    }else{
        mysql_password = node;
    }
    
    node = ptree.get_child_optional("MysqlHost")->get_value<string>();
    if(node.empty()){
        LOGW("get MysqlHost failed!");
    }else{
        mysql_host = node;
    }
    
    node = ptree.get_child_optional("MysqlDatabase")->get_value<string>();
    if(node.empty()){
        LOGW("get MysqlDatabase failed!");
    }else{
        mysql_database = node;
    }
    
    node = ptree.get_child_optional("RedisHost")->get_value<string>();
    if(node.empty()){
        LOGW("get RedisName failed!");
    }else{
        redis_host = node;
    }
    
    node = ptree.get_child_optional("RedisPort")->get_value<string>();
    if(node.empty()){
        LOGW("get RedisPassword failed!");
    }else{
        redis_port = node;
    }
    
    node = ptree.get_child_optional("ServerIp")->get_value<string>();
    if(node.empty()){
        LOGW("get ServerIp failed!");
    }else{
        server_ip = node;
    }
    
    node = ptree.get_child_optional("ServerPort")->get_value<string>();
    if(node.empty()){
        LOGW("get ServerPort failed!");
    }else{
        server_port = node;
    }
    
    LOGI("MysqlName:%s,MysqlPassword:%s,MysqlHost:%s,MysqlDatabase:%s,RedisHost:%s,RedisPort:%s,ServerIp:%s,ServerPort:%s"
         ,mysql_name.c_str(),mysql_password.c_str(),mysql_host.c_str(),
         mysql_database.c_str(),redis_host.c_str(),redis_port.c_str(),
         server_ip.c_str(),server_port.c_str());

    fs.close();
}
MysqlConnect* SomeWhereServer::get_mysql_instance() const{
    return mysql;
}

RedisConnect* SomeWhereServer::get_redis_instance() const{
    return redis;
}



