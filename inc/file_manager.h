/**
 * @file file_manager.h
 * @author José C. Pereira (jpereira1330@gmail.com)
 * @version 1.0
 * @date 2022-03-17
 * @copyright Copyright (c) 2022
 */

#ifndef _INC_FILE_MANAGER_H_
#define _INC_FILE_MANAGER_H_

 #include <stdio.h>
 #include <stdbool.h>
 #include <sys/types.h>

/**
 * @brief Abre o arquivo solicitado e armazena em memoria
 * 
 * @param file      Arquivo que sera aberto 
 * @param mode      Modo de abertura de arquivo "W; A; W+; R; ..."
 * @return file_id   Identificacao do arquivo no sistema operacional
 */
FILE *file_manager_open(const char *file, const char *mode);

/**
 * @brief Escreve no arquivo ja aberto

 * @param file_id   Identificacao do arquivo no sistema operacional
 * @param buffer    Buffer a ser salvo
 * @param len       Tamanho do buffer a ser 
 * @return int      (+) Para bytes salvos, (-) para erro.
 */
int file_manager_write(FILE *file_id, const char *buffer, uint len);

/**
 * @brief Le o arquivo ja aberto
 * 
 * @param file_id   Identificacao do arquivo no sistema operacional
 * @param buffer    Buffer a ser preenchido
 * @param len_max   Tamanho maximo do buffer
 * @return int      (+) Para bytes salvos, (-) para erro.
 */
int file_manager_read(FILE *file_id, char *buffer, uint len_max);

/**
 * @brief Fecha o arquivo armazenado em memoria
 * 
 * @param file_id   Identificacao do arquivo no sistema operacional
 * @return true     Para sucesso
 * @return false    Para erro
 */
bool file_manager_close(FILE *file_id);

#endif  // _INC_FILE_MANAGER_H_