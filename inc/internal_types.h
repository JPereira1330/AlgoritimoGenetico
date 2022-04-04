#ifndef _INC_INTERNAL_TYPES_H_
#define _INC_INTERNAL_TYPES_H_

#include <stdbool.h>

#define DEF_MAX_LEN_NAME        125
#define DEF_MAX_LEN_ITENS       6200
#define DEF_MAX_LEN_POPULATION  1000
#define DEF_MAX_INIT_ITENS      5

#define DEF_MAX_BETTER_PASS     5

#define DEF_DATABASE_NAME       "data/drugs.db"

typedef struct stc_data_base {
    int id;
    char name[DEF_MAX_LEN_NAME];
    float value;
} DB_ITENS;

typedef struct stc_dna_person {
    bool itens[DEF_MAX_LEN_ITENS];
    int amount_itens;
    float total_value; 
} PERSON;

#endif  // _INC_INTERNAL_TYPES_H_
