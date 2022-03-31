#include "genetic_manager.h"

#include <math.h>
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
    int j, value;
    int index;
    _DB_ROW db_row;
    POP pop_better;
    POP pop_mutable;

    pop_better = genetic_manager_new();
    pop_mutable = genetic_manager_new();

    genetic_manager_get_better(param, population, pop_better);
    genetic_manager_get_mutable(param, pop_better, pop_mutable);

    /// Adicionando a populacao
    memset(population, 0, sizeof(struct stc_population));
    for(int index = 0; index <= (pop_better->last_dna+pop_mutable->last_dna)+1; index++) {
        population->last_dna = index;
        if(index <= pop_better->last_dna) {
            population->dna[index].selecter = pop_better->dna[index].selecter;
            population->dna[index].itens_value = pop_better->dna[index].itens_value;
            population->dna[index].amount_itens = pop_better->dna[index].amount_itens;
            for (int j = 0; j < pop_better->dna[index].amount_itens; j++) {
                population->dna[index].itens[j].iten_value = pop_better->dna[index].itens[j].iten_value;
                snprintf(population->dna[index].itens[j].iten, MAX_LEN_NAME_ITEN, "%s", pop_better->dna[index].itens[j].iten);
            }
        } else {
            population->dna[index].selecter = pop_mutable->dna[index-pop_better->last_dna-1].selecter;
            population->dna[index].itens_value = pop_mutable->dna[index-pop_better->last_dna-1].itens_value;
            population->dna[index].amount_itens = pop_mutable->dna[index-pop_better->last_dna-1].amount_itens;
            for (int j = 0; j < pop_mutable->dna[index-pop_better->last_dna-1].amount_itens; j++) {
                population->dna[index].itens[j].iten_value = pop_mutable->dna[index-pop_better->last_dna-1].itens[j].iten_value;
                snprintf(population->dna[index].itens[j].iten, MAX_LEN_NAME_ITEN, "%s", pop_mutable->dna[index-pop_better->last_dna-1].itens[j].iten);
            }
        }
    }

    /// Criando o restante
    for (int i = population->last_dna+1; i < MAX_POPULATION; i++) {
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

    genetic_manager_free(pop_mutable);
    genetic_manager_free(pop_better);

    return true;
}

bool genetic_manager_get_mutable(PARAM param, POP pop_better, POP pop_mutable) {
    float quantia;
    POP auxiliar;
    int decress;
    int index, count;

    auxiliar = genetic_manager_new();

    count = 0;
    index = 0;
    decress = 0;
    while(index < MAX_BETTER_POP) {
        auxiliar->dna[index] = pop_better->dna[index];
        pop_mutable->dna[count-decress].itens_value = 0;
        quantia = floor(pop_better->dna[index+1].amount_itens/2);
        for (int j = 0; j < pop_better->dna[index].amount_itens; j++) {
            if(j < quantia) {
                pop_mutable->dna[count-decress].itens[j].iten_value = pop_better->dna[index].itens[j].iten_value;
                snprintf(pop_mutable->dna[count-decress].itens[j].iten, MAX_LEN_NAME_ITEN, "%s", pop_better->dna[index].itens[j].iten);
            } else {
                pop_mutable->dna[count-decress].itens[j].iten_value = pop_better->dna[index+1].itens[j].iten_value;
                snprintf(pop_mutable->dna[count-decress].itens[j].iten, MAX_LEN_NAME_ITEN, "%s", pop_better->dna[index+1].itens[j].iten);
            }
            pop_mutable->dna[count-decress].amount_itens = j;
            pop_mutable->dna[count-decress].itens_value += pop_mutable->dna[count-decress].itens[j].iten_value;
        }
        
        if( pop_mutable->dna[count-decress].itens_value > param->budget ){
            decress++;
        }

        pop_mutable->last_dna = count-decress;
        pop_mutable->dna[count-decress].selecter = false;

        count++;
        index+=2;
    }

    genetic_manager_free(auxiliar);

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
            }
        }
        population->dna[index].selecter = true;

        /// Salvando dados
        pop_out->last_dna = i;
        pop_out->dna[i].selecter = false;
        pop_out->dna[i].amount_itens =  population->dna[index].amount_itens;
        pop_out->dna[i].itens_value =  population->dna[index].itens_value;
        for (j = 0; j < pop_out->dna[i].amount_itens; j++) {
            snprintf(pop_out->dna[i].itens[j].iten, MAX_LEN_NAME_ITEN, "%s", population->dna[index].itens[j].iten);
            pop_out->dna[i].itens[j].iten_value = population->dna[index].itens[j].iten_value;
        }
    }

    return true;
}

void genetic_manager_debug_better(PARAM param, POP population, int quant) {

    POP pop_better;
    pop_better = genetic_manager_new();

    genetic_manager_get_better(param, population, pop_better);

    for(int i = 0; i < quant && i <= pop_better->last_dna; i++) {
        printf(" [ %d ] %d (R$ %.2f) \t", i, pop_better->dna[i].amount_itens, pop_better->dna[i].itens_value);
    }
    printf("\n\n");

}