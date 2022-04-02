#include <stdio.h>
#include <stdlib.h>

#include "param.h"
#include "internal_types.h"
#include "genetic_manager.h"

//////////////////////////////////////////
///     VARIAVEIS GLOBAIS
//////////////////////////////////////////

PERSON person[DEF_MAX_LEN_POPULATION];
PERSON new_pop[DEF_MAX_MULT_POP];

int main(void) {

    PARAM param;   
 
    param = param_init();
    if(param == NULL) {
        return EXIT_FAILURE;
    }

    param->epoch = 100;
    param->budget = 5000;

    genetic_manager_create_population(param, person, DEF_MAX_LEN_POPULATION);
    genetic_manager_show_better(param, person, DEF_MAX_LEN_POPULATION, 7);

    //for(int epoch = 1; epoch <= param->epoch; epoch++) {
    ///    printf("Epoca %d: ", epoch);
        /// Cruzando
        genetic_manager_mate_population(param, person, new_pop);
        genetic_manager_calc_fit(param, new_pop, DEF_MAX_MULT_POP);

        /// Matando o excesso
        genetic_manager_kill_population(param, new_pop, DEF_MAX_MULT_POP);
        genetic_manager_calc_fit(param, new_pop, DEF_MAX_MULT_POP);
        genetic_manager_show_better(param, new_pop, DEF_MAX_LEN_POPULATION, 7);
        /// Mutando

        /// Convergindo
        genetic_manager_fix_population(param, person, new_pop, DEF_MAX_LEN_POPULATION);
        genetic_manager_show_better(param, person, DEF_MAX_LEN_POPULATION, 7);
    //}

    param_free(param);

    return 0;
}