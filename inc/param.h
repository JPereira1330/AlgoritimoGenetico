/**
 * @file param.h
 * @author José C. Pereira (jpereira1330@gmail.com)
 * @brief Parametros do sistema
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022
 */

#ifndef _INC_PARAM_H_
#define _INC_PARAM_H_

///
/// DEPENDENCIAS
///

#include <stdio.h>

///
/// DECLARACAO DAS STRUCTS
///

typedef struct strct_param {
    int epoch;
    float budget;

    FILE *file_open;
} *PARAM;

///
/// DECLARACAO DAS FUNCOES PUBLICAS
///

/**
 * @brief Aloca na memoria os dados do sistema
 * @return PARAM    Parametros do sistema
 */
PARAM param_init();

/**
 * @brief Libera da memoria os dados da aplciacao
 * @param param     Parametros do sistema
 */
void param_free(PARAM param);

#endif  // _INC_PARAM_H_
