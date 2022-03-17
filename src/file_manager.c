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

int file_manager_write(PARAM param, const char *buffer, uint len) {

    if (param == NULL || param->file_open == NULL) {
        return -1;
    }

    if (buffer == NULL || len <= 0) {
        return -2;
    }

    return fwrite(buffer, sizeof(char), len, param->file_open);
}

// TODO: Arruma o read, deixar mais performatico
int file_manager_read(PARAM param, char *buffer, uint len_max) {
    int ret;
    int bytes;

    if (param == NULL || param->file_open == NULL) {
        return -1;
    }

    if (buffer == NULL || len_max <= 0) {
        return -2;
    }

    fseek(param->file_open, 0, SEEK_SET);
    for(ret = 1, bytes = 0; bytes < len_max && ret != 0;bytes++){
        ret = fread(buffer+bytes, sizeof(char), sizeof(char), param->file_open);
    }

    return bytes;
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
