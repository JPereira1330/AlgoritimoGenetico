#include <stdio.h>
#include <stdlib.h>

#include "param.h"
#include "internal_types.h"
#include "genetic_manager.h"

//////////////////////////////////////////
///     VARIAVEIS GLOBAIS
//////////////////////////////////////////

PERSON person[DEF_MAX_LEN_POPULATION];
PERSON new_pop[DEF_MAX_LEN_POPULATION];
PERSON mut_pop[DEF_MAX_LEN_POPULATION];

int main(void) {
    PARAM param;   
    int amount_tot;
    int amount_new;
    int amount_mut;
 
    param = param_init();
    if(param == NULL) {
        return EXIT_FAILURE;
    }

    param->epoch    = 1500;
    param->budget   = 5000;

    /// Criando populacao do zero
    amount_tot = genetic_manager_create_population(param, person, DEF_MAX_LEN_POPULATION);

    for (int i = 1; i < param->epoch; i++) {
        printf("Epoch: %d ", i);

        amount_new = genetic_manager_mate_population(param, person, new_pop, amount_tot);       /// Crusando a populacao
        genetic_manager_kill_population(param, new_pop, amount_new);                            /// Matando o excesso

        amount_mut = genetic_manager_mutable_population(param, new_pop, mut_pop, amount_tot);    /// Mutando a populacao
        genetic_manager_kill_population(param, mut_pop, amount_mut);                            /// Matando o excesso

        amount_tot = genetic_manager_fix_population(param, person, new_pop, mut_pop, DEF_MAX_LEN_POPULATION);    /// Convergindo
        genetic_manager_show_better(param, person, 10);
    }
    param_free(param);

    return 0;
}