/**
 * @file param.h
 * @author José C. Pereira (jpereira1330@gmail.com)
 * @brief Parametros do sistema
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022
 */

#ifndef INC_PARAM_H_
#define INC_PARAM_H_

///
/// DEPENDENCIAS
///

#include <stdio.h>

///
/// DECLARACAO DAS STRUCTS
///

typedef struct strct_param {
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

#endif  // INC_PARAM_H_
