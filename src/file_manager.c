/**
 * @file file_manager.c
 * @author José C. Pereira (jpereira1330@gmail.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022
 */

#include "file_manager.h"

bool file_manager_open(PARAM param, const char *file, const char *mode) {

    FILE *file_local;

    if(param == NULL) {
        return false;
    }

    file_local = fopen(file, mode);
    if(file_local == NULL){
        return false;
    }

    param->file_open = file_local;

    return true;
}

bool file_manager_close(PARAM param) {
    int ret;

    if (param == NULL || param->file_open == NULL) {
        return false;
    }

    ret = fclose(param->file_open);
    if (ret != 0) {
        return false;
    }

    return true;
}
