#include <stdio.h>
#include <stdlib.h>

#include "param.h"
#include "internal_types.h"
#include "genetic_manager.h"

//////////////////////////////////////////
///     VARIAVEIS GLOBAIS
//////////////////////////////////////////

PERSON new_pop[DEF_MAX_LEN_POPULATION];
PERSON person[DEF_MAX_LEN_POPULATION];

int main(void) {

    PARAM param;   
 
    param = param_init();
    if(param == NULL) {
        return EXIT_FAILURE;
    }

    param->epoch = 200;
    param->budget = 5000;

    genetic_manager_create_population(param, person, DEF_MAX_LEN_POPULATION);
    genetic_manager_show_better(param, person, DEF_MAX_LEN_POPULATION, 10);
    for (int i = 1; i < param->epoch; i++) {
        printf("Epoch: %d \t", i);
        genetic_manager_mate_population(param, person, new_pop);                            /// Crusando a populacao
        genetic_manager_kill_population(param, new_pop, DEF_MAX_LEN_POPULATION);            /// Matando o excesso
        /// Mutando
        genetic_manager_fix_population(param, person, new_pop, DEF_MAX_LEN_POPULATION);     /// Convergindo
        genetic_manager_show_better(param, person, DEF_MAX_LEN_POPULATION, 10);              /// Exibindo resultado
    }

    param_free(param);

    return 0;
}