#ifndef _INC_LOG_MANAGER_H_
#define _INC_LOG_MANAGER_H_

#include "param.h"

#include <stdarg.h>
#include <unistd.h>

#define FAIL 0
#define DONE 1
#define INIT 2
#define PROG 3

#define log(PARAM, TAG, D_FORMAT, ...) log_write_i(PARAM, TAG, __FILE__, __LINE__, D_FORMAT, ##__VA_ARGS__)

void log_write_i(PARAM param, int tag, const char *fileName, int line, const char *format, ...){
    clock_t end;
    char buf[1024];
    double time_spent = 0.0;
    
    va_list vl;
    va_start(vl, format);

    end = clock();
    time_spent += (double)(end - param->begin) / CLOCKS_PER_SEC;

    vsnprintf( buf, sizeof( buf), format, vl);
    printf("[\033[33m%f\033[0m] ", time_spent);
    switch (tag) {
        case FAIL:
            printf("[\033[31mFAIL\033[0m] ");
            break;
        case INIT:
            printf("[\033[34mINIT\033[0m] ");
            break;
        case PROG:
            printf("[\033[35mPROG\033[0m] ");
            break;
        default:
            printf("[\033[32mDONE\033[0m] ");
            break;
    }

    if(tag == INIT || tag == PROG) {
        printf("~> \033[1m%s\033[0m {%s:%d}\r",buf,fileName, line);
    } else {
        printf("~> \033[1m%s\033[0m {%s:%d}\n",buf,fileName, line);
    }
    va_end( vl);
}

#endif  // _INC_LOG_MANAGER_H_