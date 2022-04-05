#include "utils.h"
#include <string.h>

int utils_array_get_population(PERSON *person) {
    int index;
    int amount;

    amount = 0;
    for (index = 0; index < DEF_MAX_LEN_POPULATION; index++) {
        if(person[index].killed == false){
            amount++;
        }
    }

    return amount;
}

void utils_array_calc_total_value(PARAM param, PERSON *person) {
    for (size_t i = 0; i < DEF_MAX_LEN_POPULATION; i++) {
        person[i].total_value = 0;
        if (person[i].killed)
            continue;
        for (size_t j = 0; j < DEF_MAX_LEN_ITENS; j++) {
            if ( person[i].itens[j] ) {
                person[i].total_value += param->db_itens[i].value;
            }
        }
    }
}

void utils_array_get_better(PARAM param, PERSON* population, PERSON* pop_out) {
    int value_max;
    int index_max;
    int amount_pop;
    bool selected[DEF_MAX_LEN_POPULATION];

    memset(selected, 0, sizeof(bool) * DEF_MAX_LEN_POPULATION);
    amount_pop = utils_array_get_population(population);

    for (int index = 0; index < amount_pop; index++) {
        value_max = 0;
        index_max = 0;
        for (int index_pop = 0; index_pop < amount_pop; index_pop++) {
            if (selected[index_pop] == true)
                continue;
            if (value_max < population[index_pop].amount_itens) {
                index_max = index_pop;
                value_max = population[index_pop].amount_itens;
            }
        }

        /// Realizando atribuincoes
        selected[index_max] = true;
        pop_out[index].total_value = population[index_max].total_value;
        pop_out[index].amount_itens = population[index_max].amount_itens;
        for (int index_iten = 0; index_iten < DEF_MAX_LEN_ITENS; index_iten++) {
            pop_out[index].itens[index_iten] = population[index_max].itens[index_iten];
        }
    }
}

void utils_array_add_array(PARAM param, PERSON *pop_out, PERSON *pop_add, int i_init, int i_last) {
    int count;
    int index;
    PERSON p_temp[DEF_MAX_LEN_POPULATION];

    utils_array_get_better(param, pop_add, p_temp);
    count = 0;
    for (index = 0; index < i_last; index++) {
        if(index > i_init && index < i_last){
            pop_out[index].total_value = pop_add[count].total_value;
            pop_out[index].amount_itens = pop_add[count].amount_itens;
            for(int j = 0; j <= DEF_MAX_LEN_ITENS; j++) {
                pop_out[index].itens[j] = pop_add[count].itens[j];
            }
            count++;
        }
    }
}