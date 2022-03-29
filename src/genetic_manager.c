#include "genetic_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db_manager.h"

bool genetic_manager_create_population(PARAM param, MDB database, POP *population) {
    
    float value;
    int index, i,j;
    _DB_ROW db_row;

    population->last_dna = 0;
    for (i = 0; i < MAX_POPULATION; i++) {
        for (value = 0, j = 0; ; j++) {
            // Capturando um indice aleatorio
            index = random() % database.last_id;
            db_row = db_manager_ret_row_by_index(param, database, index);

            // Validado se pode continuar
            if(j == 0 && db_row.value > param->budget){
                j--;
                continue;
            }

            value += db_row.value;
            if(value > param->budget){
                break;
            }

            // Salvando dados
            population->dna[i].selecter = false;
            population->dna[i].amount_itens = j;
            population->dna[i].itens_value = value;
            population->dna[i].itens[j].iten_value = db_row.value;
            snprintf(population->dna[i].itens[j].iten, strlen(db_row.iten), "%s", db_row.iten);
        }

        population->last_dna++;
    }

    return true;
}

bool genetic_manager_get_better(PARAM param, POP population, POP *new_pop) {
//    int amount;
//    int index;
//
//    index = 0;
//    amount = 0;
//
//    for (int i = 0; i < MAX_BETTER_POP; i++) {
//        for (int j = 0; j < population.last_dna; j++) {
//            if (amount < population.dna[j].amount_itens && population.dna[j].selecter == false) {
//                index = j;
//                amount = population.dna[j].amount_itens;
//            }
//        }
//        population.dna[index].selecter = true;
//
//        // Alimentando nova Populacao
//        pop_out->last_dna = 1;
//        pop_out->dna[i].selecter = false;
//        pop_out->dna[i].amount_itens =  population.dna[index].amount_itens;
//        pop_out->dna[i].itens_value =  population.dna[index].itens_value;
//        for (int j = 0; j < population.last_dna; j++) {
//            snprintf(pop_out->dna[i].itens[j].iten, MAX_LEN_NAME_ITEN, "%s", population.dna[i].itens[j].iten);
//            pop_out->dna[i].itens[j].iten_value = population.dna[i].itens[j].iten_value;
//        }
//        printf(" %d - %d R$ %.2f (%d)\n", i, pop_out->dna[i].amount_itens, pop_out->dna[i].itens_value, pop_out->dna[i].selecter);
//    }

    return true;
}
