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

int genetic_manager_create_population(PARAM param, PERSON *population, int amount_population);

int genetic_manager_mate_population(PARAM param, PERSON *population, PERSON *new_pop, int amount_population);

bool genetic_manager_calc_fit(PARAM param, PERSON *population, int amount_population);
bool genetic_manager_show_better(PARAM param, PERSON *population, int amount_to_show);
bool genetic_manager_show_all(PARAM param, PERSON *population, int amount_to_show);
bool genetic_manager_save_report(PARAM param, PERSON *population, int amount_to_show);
int genetic_manager_kill_population(PARAM param, PERSON *population, int amount_population);
int genetic_manager_fix_population(PARAM param, PERSON *population, PERSON *new_pop, PERSON *mut_pop, int amount_population);
int genetic_manager_mutable_population(PARAM param, PERSON *population, PERSON *mult_pop, int amount_population);


#endif  // _INC_GENETIC_MANAGER_H_