#include "utils.h"

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