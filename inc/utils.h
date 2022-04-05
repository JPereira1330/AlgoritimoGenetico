#ifndef _INC_UTILS_H_
#define _INC_UTILS_H_
#include "param.h"
#include "internal_types.h"

int utils_array_get_population(PERSON *person);
void utils_array_calc_total_value(PARAM param, PERSON *person);
void utils_array_get_better(PARAM param, PERSON* population, PERSON* pop_out);
void utils_array_add_array(PARAM param, PERSON *pop_out, PERSON *pop_add, int i_init, int i_last);

#endif  // _INC_UTILS_H_