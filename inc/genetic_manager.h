#ifndef _INC_GENETIC_MANAGER_H_
#define _INC_GENETIC_MANAGER_H_

#include <stdbool.h>

#include "param.h"
#include "type_structs.h"
#include "internal_defines.h"

bool genetic_manager_create_population(PARAM param, MDB database, POP *population);
bool genetic_manager_get_better(PARAM param, POP population, POP *new_pop);

#endif  // _INC_GENETIC_MANAGER_H_