/**
 * @file file_manager.h
 * @author José C. Pereira (jpereira1330@gmail.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022
 */

#ifndef INC_FILE_MANAGER_H_
#define INC_FILE_MANAGER_H_
 
 #include "param.h"

 #include <stdbool.h>
 #include <sys/types.h>

/**
 * @brief Abre o arquivo solicitado e armazena em memoria
 * 
 * @param param     Parametros do sistema
 * @param file      Arquivo que sera aberto 
 * @param mode      Modo de abertura de arquivo "W; A; W+; R; ..."
 * @return true     Para sucesso
 * @return false    Para erro
 */
bool file_manager_open(PARAM param, const char *file, const char *mode);

/**
 * @brief Escreve no arquivo ja aberto

 * @param param     Parametros do sistema
 * @param buffer    Buffer a ser salvo
 * @param len       Tamanho do buffer a ser 
 * @return int      (+) Para bytes salvos, (-) para erro.
 */
int file_manager_write(PARAM param, const char *buffer, uint len);

/**
 * @brief Le o arquivo ja aberto
 * 
 * @param param     Parametros do sistema
 * @param buffer    Buffer a ser preenchido
 * @param len_max   Tamanho maximo do buffer
 * @return int      (+) Para bytes salvos, (-) para erro.
 */
int file_manager_read(PARAM param, char *buffer, uint len_max);

/**
 * @brief Fecha o arquivo armazenado em memoria
 * 
 * @param param     Parametros do sistema
 * @return true     Para sucesso
 * @return false    Para erro
 */
bool file_manager_close(PARAM param);

#endif  // INC_FILE_MANAGER_H_