/**
 * @file file_manager.c
 * @author José C. Pereira (jpereira1330@gmail.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022
 */

#include "file_manager.h"

FILE *file_manager_open(const char *file, const char *mode) {
    FILE *file_id;
    
    file_id = fopen(file, mode);
    if(file_id == NULL){
        return NULL;
    }

    // Configurando arquivo
    fseek(file_id, 0, SEEK_SET);

    return file_id;
}

int file_manager_write(FILE *file_id, const char *buffer, uint len) {
    if (buffer == NULL || len <= 0) {
        return -2;
    }

    return fwrite(buffer, sizeof(char), len, file_id);
}

int file_manager_read(FILE *file_id, char *buffer, uint len_max) {
    int ret;
    int bytes;

    if (buffer == NULL || len_max <= 0) {
        return -2;
    }

    for(ret = 1, bytes = 0; bytes < len_max && ret != 0;bytes++){
        ret = fread(buffer+bytes, sizeof(char), sizeof(char), file_id);
        if(*(buffer+bytes) == '\n')
            return bytes;
    }

    return 0;
}

bool file_manager_close(FILE *file_id) {
    int ret;

    ret = fclose(file_id);
    if (ret != 0) {
        return false;
    }

    return true;
}
