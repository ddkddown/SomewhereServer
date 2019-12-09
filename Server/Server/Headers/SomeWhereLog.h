//
//  SomeWhereLog.h
//  Server
//
//  Created by dong da kuan on 2019/12/8.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#pragma once

#include <syslog.h>
#define IDENT "SERVER"

#define LOGI(msg,...) \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
syslog(LOG_INFO, "[%s]: %s",__func__,msg); \
closelog();SA

#define LOGD(msg,...) \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
syslog(LOG_DEBUG, "[%s]: %s",__func__,msg); \
closelog(); \

#define LOGW(msg,...) \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
syslog(LOG_WARNING, "[%s]: %s",__func__,msg); \
closelog(); \

#define LOGE(msg,...) \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
syslog(LOG_ERR, "[%s]: %s",__func__,msg); \
closelog(); \

#define LOGMERGE(msg,...) \
openlog(IDENT, LOG_ODELAY, LOG_LOCAL0); \
syslog(LOG_EMERG, "[%s]: %s",__func__,msg); \
closelog(); \


