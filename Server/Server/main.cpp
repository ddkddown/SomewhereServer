//
//  main.cpp
//  Server
//
//  Created by dong da kuan on 2019/11/17.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "SomeHeader.h"
#include "SomeWhere_Login.hpp"
#include "SomeWhereServer.hpp"
#include "SomeWhereLog.h"
using namespace boost::asio;

int main(int argc, const char * argv[]) {
    auto i = SomeWhereServer::get_instance();
    i->do_default_accept();
}

