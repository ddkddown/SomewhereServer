//
//  SomeWhereLog.h
//  Server
//
//  Created by dong da kuan on 2019/12/8.
//  Copyright © 2019年 dong da kuan. All rights reserved.
//

#ifndef SomeWhereLog_h
#define SomeWhereLog_h
#include <syslog.h>
#define IDENT "SERVER"

inline void LOGI(const char* message,...){
    if (nullptr == message) {
        return;
    }
    openlog(IDENT, LOG_ODELAY, LOG_LOCAL0);
    syslog(LOG_INFO, "%s", message);
    closelog();
}

inline void LOGD(const char* message,...){
#ifdef SOMEWHERE_DEBUG
    if(nullptr == message){
        return;
    }
    openlog(IDENT, LOG_ODELAY, LOG_LOCAL0);
    syslog(LOG_DEBUG, "%s", message);
    closelog();
#endif
}

inline void LOGW(const char* message,...){
    if(nullptr == message){
        return;
    }
    openlog(IDENT, LOG_ODELAY, LOG_LOCAL0);
    syslog(LOG_WARNING, "%s", message);
    closelog();
}

inline void LOGE(const char* message,...){
    if(nullptr == message){
        return;
    }
    openlog(IDENT, LOG_ODELAY, LOG_LOCAL0);
    syslog(LOG_ERR, "%s", message);
    closelog();
}

inline void LOGEMERGE(const char* message,...){
    if(nullptr == message){
        return;
    }
    openlog(IDENT, LOG_ODELAY, LOG_LOCAL0);
    syslog(LOG_EMERG, "%s", message);
    closelog();
}
#endif /* SomeWhereLog_h */
