/**
 * @file    genetic_manager.h
 * @author  José C. Pereira (jpereira1330@gmail.com)
 * @date    2022-03-31
 * @copyright Copyright (c) 2022
 */

#ifndef _INC_GENETIC_MANAGER_H_
#define _INC_GENETIC_MANAGER_H_

#include <stdbool.h>

#include "param.h"
#include "internal_types.h"

/**
 * @brief Carrega do banco de dados e preenche os dados de forma aleatoria
 * 
 * @param param                 Parametros do sistema
 * @param population            Ponteiro da populacao
 * @param amount_population     Tamanho da populacao 
 * @return true                 Para sucesso na operacao
 * @return false                Para erro na operacao
 */
bool genetic_manager_create_population(PARAM param, PERSON *population, int amount_population);

/**
 * @brief 
 * 
 * @param param                 Parametros do sistema
 * @param population            Ponteiro da populacao
 * @param amount_population     Tamanho da populacao 
 * @return true                 Para sucesso na operacao
 * @return false                Para erro na operacao
 */
bool genetic_manager_mate_population(PARAM param, PERSON *population, int amount_population);

void genetic_manager_mutable_population();
void genetic_manager_kill_population();

#endif  // _INC_GENETIC_MANAGER_H_