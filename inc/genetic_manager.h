#ifndef _INC_GENETIC_MANAGER_H_
#define _INC_GENETIC_MANAGER_H_

#include <stdbool.h>

#include "param.h"
#include "type_structs.h"
#include "internal_defines.h"

POP genetic_manager_new();
void genetic_manager_free(POP population);

bool genetic_manager_create_population(PARAM param, MDB database, POP population, int init);
bool genetic_manager_recreate_population(PARAM param, MDB database, POP population);

bool genetic_manager_get_mutable(PARAM param, POP pop_better, POP pop_mutable);
bool genetic_manager_get_better(PARAM param, POP population, POP pop_out);
void genetic_manager_debug_better(PARAM param, POP population, int quant);

#endif  // _INC_GENETIC_MANAGER_H_