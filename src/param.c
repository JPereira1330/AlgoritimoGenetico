/**
 * @file param.c
 * @author José C. Pereira (jpereira1330@gmail.com)
 * @brief Manipula os parametros da aplicacao
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022
 */

#include "param.h"

#include <stdlib.h>
#include <string.h>

PARAM param_init() {
    PARAM param;

    param = (PARAM) malloc(sizeof(struct strct_param));
    if(param == NULL){
        return NULL;
    }

    memset(param, 0, sizeof(struct strct_param));
    
    param->begin = clock();

    return param;
}

void param_free(PARAM param) {
    if(param != NULL){
        free(param);
    }
}
