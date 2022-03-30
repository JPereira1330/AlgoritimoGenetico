#include "genetic_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db_manager.h"

POP genetic_manager_new() {
    POP population;

    population = (POP) malloc(sizeof(struct stc_population));
    if(population == NULL){
        return NULL;
    }

    memset(population, 0, sizeof(struct stc_population));

    return population;
}

void genetic_manager_free(POP population) {
    if(population != NULL){
        free(population);
    }
}

bool genetic_manager_create_population(PARAM param, MDB database, POP population) {
    
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

bool genetic_manager_recreate_population(PARAM param, MDB database, POP population) {
    POP pop_better;
    POP pop_mutable;

    pop_better = genetic_manager_new();
    pop_mutable = genetic_manager_new();

    genetic_manager_get_better(param, population, pop_better);
    //genetic_manager_get_mutable(param, pop_better, pop_mutable);

    genetic_manager_free(pop_mutable);
    genetic_manager_free(pop_better);
    return true;
}

bool genetic_manager_get_mutable(PARAM param, POP pop_better, POP pop_mutable) {
    int i, j;

    for(i = 0; i < MAX_BETTER_POP; i++){
        //printf("%d - Carrinho: %d | R$ %.2f\n", i, pop_better->dna[i].amount_itens, pop_better->dna[i].itens_value);
        for (j = 0; j <= pop_better->dna[i].amount_itens; j++) {
            //printf("\t %d - %s %.2f\n",j, pop_better->dna[i].itens[j].iten, pop_better->dna[i].itens[j].iten_value);
        }
    }

    return true;
}

bool genetic_manager_get_better(PARAM param, POP population, POP pop_out) {
    int i, j;
    int index;
    int amount;

    for (i = 0; i < MAX_BETTER_POP; i++) {
        index = 0;
        amount = 0;
        
        /// Encontrando dados
        for (j = 0; j <= population->last_dna; j++) {
            if (amount < population->dna[j].amount_itens && population->dna[j].selecter == false) {
                index = j;
                amount = population->dna[j].amount_itens;
                printf("Select: [%d/%d] %d - %s\n", i,j, population->dna[j].amount_itens, population->dna[j].itens[0].iten);
            }
        }
        population->dna[index].selecter = true;

        /// Salvando dados
        pop_out->last_dna = i;
        pop_out->dna[i].selecter = false;
        pop_out->dna[i].amount_itens =  population->dna[index].amount_itens;
        pop_out->dna[i].itens_value =  population->dna[index].itens_value;
        for (j = 0; j < pop_out->dna[i].amount_itens; j++) {
            snprintf(pop_out->dna[i].itens[j].iten, MAX_LEN_NAME_ITEN, "%s", population->dna[i].itens[j].iten);
            pop_out->dna[i].itens[j].iten_value = population->dna[i].itens[j].iten_value;
        }

        printf("Melhor: [%d/%d] %d - %s\n", i,j, pop_out->dna[i].amount_itens, pop_out->dna[i].itens[0].iten);
    }

    return true;
}
