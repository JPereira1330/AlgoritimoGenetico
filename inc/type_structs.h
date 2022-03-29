#ifndef _INC_TYPE_POPULATION_H_
#define _INC_TYPE_POPULATION_H_

#include <stdbool.h>
#include "internal_defines.h"

typedef struct stc_rna {
    char iten[MAX_LEN_NAME_ITEN];
    float iten_value;
} RNA;

typedef struct stc_dna {
    RNA itens[MAX_LEN_ARRAY_ITENS];
    int amount_itens;
    float itens_value;
    bool selecter;
} DNA;

typedef struct stc_population {
    int last_dna;
    DNA dna[MAX_LEN_POPULATION];
} POP;

typedef struct _stc_database_row {
    unsigned int id;
    float value;
    char iten[MAX_LEN_NAME_ITEN];
} _DB_ROW;

typedef struct stc_database {
    unsigned int last_id;
    _DB_ROW rows[MAX_DATABASE_LEN];
} MDB;

#endif  // _INC_TYPE_POPULATION_H_