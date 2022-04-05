/**
 * @file    genetic_manager.c
 * @author  José C. Pereira (jpereira1330@gmail.com)
 * @date    2022-03-31
 * @copyright Copyright (c) 2022
 */

#include "genetic_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "log_manager.h"
#include "file_manager.h"
//////////////////////////////////////////
///     VARIAVEIS GLOBAIS
//////////////////////////////////////////

PERSON person_aux[DEF_MAX_LEN_POPULATION];

//////////////////////////////////////////
///     DECLARACAO DOS METODOS PRIVADOS
//////////////////////////////////////////

bool _genetic_manager_load_database(PARAM param);
bool _genetic_manager_init_pop(PARAM param, PERSON *population, int amount_population);
bool _genetic_manager_active_random(PARAM param, PERSON *population, int amount_population);
int _genetic_manager_change_to_new_array(PARAM param, PERSON *population, PERSON *pop_out, int amount_population, int init);

//////////////////////////////////////////
///     IMPLEMENTACAO DOS METODOS PUBLICOS
//////////////////////////////////////////

int genetic_manager_create_population(PARAM param, PERSON *population, int amount_pop) {

    /// Carregando populacao em memoria e populando
    _genetic_manager_load_database(param);
    _genetic_manager_init_pop(param, population, amount_pop);
    _genetic_manager_active_random(param, population, amount_pop);

    return amount_pop;
}

int genetic_manager_mate_population(PARAM param, PERSON *population, PERSON *new_pop, int amount_population) {
    int index_init;

    log(param, INIT, "Cruzando populacao");

    memset(new_pop, 0, sizeof(PERSON)* DEF_MAX_LEN_POPULATION);

    // Percorre toda a população
    for(int i = 0; i < amount_population; i++) {
        index_init = i+1;
        for(int j = 0; j < DEF_MAX_LEN_ITENS; j++) {
            if(j > (DEF_MAX_LEN_ITENS/2)) {
                if(index_init == amount_population) {
                    new_pop[i].itens[j] = population[0].itens[j];
                }else{
                    new_pop[i].itens[j] = population[index_init].itens[j];
                }
            }else {
                new_pop[i].itens[j] = population[i].itens[j];
            }
        }
    }

    genetic_manager_calc_fit(param, new_pop, amount_population);

    log(param, DONE, "Cruzando populacao");
    return index_init-1;
}

int genetic_manager_mutable_population(PARAM param, PERSON *population, PERSON *mult_pop, int amount_population) {
    int amount;
    int index_rand;

    log(param, INIT, "Mutando populacao");
    memset(mult_pop, 0, sizeof(PERSON)* DEF_MAX_LEN_POPULATION);
    memcpy(mult_pop, population, sizeof(PERSON)* DEF_MAX_LEN_POPULATION);

    amount = 0;
    for (int index = 0; index < amount_population; index++) {
        index_rand = rand() % DEF_MAX_LEN_ITENS;
        mult_pop[index].itens[index_rand] = !population[index].itens[index_rand];
        amount++;
    }

    genetic_manager_calc_fit(param, mult_pop, amount);

    return amount;
}

bool genetic_manager_calc_fit(PARAM param, PERSON *population, int amount_population) {
    log(param, INIT, "Atualizando valores da nova populacao");
    for(int i_pop = 0; i_pop < amount_population; i_pop++) {
        population[i_pop].total_value = 0;
        population[i_pop].amount_itens = 0;
        for(int i_itens = 0; i_itens < DEF_MAX_LEN_ITENS; i_itens++) {
            if(population[i_pop].itens[i_itens]){
                population[i_pop].amount_itens++;
                population[i_pop].total_value+=param->db_itens[i_itens].value;
            }
        }
    } 
    log(param, DONE, "Atualizando valores da nova populacao");
    return true;
}

int genetic_manager_fix_population(PARAM param, PERSON *population, PERSON *new_pop, PERSON *mut_pop, int amount_population) {

    int last_new_pop;

    last_new_pop = DEF_MAX_BETTER_PASS+DEF_MAX_MATE_PASS;

    memset(person_aux, 0, sizeof(PERSON) * DEF_MAX_LEN_POPULATION);
    utils_array_add_array(param, person_aux, population, 0, DEF_MAX_BETTER_PASS);
    utils_array_add_array(param, person_aux, new_pop, DEF_MAX_BETTER_PASS, last_new_pop);
    utils_array_add_array(param, person_aux, mut_pop, last_new_pop, DEF_MAX_LEN_POPULATION);

    utils_array_get_better(param, person_aux, population);

    return DEF_MAX_LEN_POPULATION;
}

int genetic_manager_kill_population(PARAM param, PERSON *population, int amount_population) {
    int amount_pop;

    amount_pop = 0;

    for (int i_pop = 0; i_pop < amount_population; i_pop++) {
        if (population[i_pop].total_value > param->budget) {
            population[i_pop].killed = true;
            population[i_pop].total_value = 0;
            population[i_pop].amount_itens = 0;
            memset(population[i_pop].itens, 0, DEF_MAX_LEN_ITENS);
        } else {
            amount_pop++;
            population[i_pop].killed = false;
        }
    }

    return amount_pop;
}

bool genetic_manager_show_better(PARAM param, PERSON *population, int amount_to_show) {
    int index;
    int quant;
    int len_pop;
    bool selected[DEF_MAX_LEN_ITENS];

    len_pop = utils_array_get_population(population);
    printf("(POP: %d) ", len_pop);

    memset(selected, 0, sizeof(bool)*DEF_MAX_LEN_ITENS);
    for(int repeat = 0; repeat < amount_to_show; repeat++ ) {
        quant = 0;
        index = 0;
        for(int i_pop = 0; i_pop < len_pop; i_pop++) {
            if(selected[i_pop] == false){
                if(quant < population[i_pop].amount_itens) {
                    index = i_pop;
                    quant = population[i_pop].amount_itens;
                }
            }
        }
        if(selected[index] == false){
            selected[index] = true;
            printf("[%d;%d;R$%2.f]\t", index, population[index].amount_itens, population[index].total_value);
        }
    }
    printf("\n");

    return true;
}


bool genetic_manager_show_all(PARAM param, PERSON *population, int amount_to_show) {
    int len_pop;

    len_pop = utils_array_get_population(population);
    printf("(POP: %d) - ", len_pop);

    for(int i = 0; i < amount_to_show; i++ ) {
        printf("[%d/R$%.2f] ", population[i].amount_itens, population[i].total_value);
    }
    printf("\n");

    return true;
}

//////////////////////////////////////////
///     IMPLEMENTACAO DOS METODOS PRIVADOS
//////////////////////////////////////////

bool _genetic_manager_load_database(PARAM param) {
    int bytes;
    char *token;
    FILE *file_open;
    char buffer[DEF_MAX_LEN_NAME];

    log(param, INIT, "Carregando banco de dados para memoria");
    file_open = file_manager_open(DEF_DATABASE_NAME, "r");
    if(file_open == NULL) {
        log(param, FAIL, "Carregando banco de dados para memoria");
        return false;
    }

    for(int index = 0; index < DEF_MAX_LEN_ITENS; index++) {
        memset(buffer, 0, sizeof(buffer));
        bytes = file_manager_read(file_open, buffer, sizeof(buffer));
        if(bytes < 0) {
            log(param, FAIL, "Carregando banco de dados para memoria");
            return false;
        }

        /// Capturando nome
        token = strtok(buffer, ";");
        snprintf(param->db_itens[index].name, DEF_MAX_LEN_NAME, "%s", token);
        
        /// Capturando valor
        token = strtok(NULL, ";");
        param->db_itens[index].value= atof(token);

        /// Valores padrao
        param->db_itens[index].id = index;
    }

    file_manager_close(file_open);

    log(param, DONE, "Carregando banco de dados para memoria");
    return true;
}

bool _genetic_manager_init_pop(PARAM param, PERSON *population, int amount_population) {
    log(param, INIT, "Inicializando populacao com valores zerados");
    for(int i = 0; i < amount_population; i++) {
        for(int j = 0; j < DEF_MAX_LEN_ITENS; j++) {
            population[i].itens[j] = false;
        }
        population[i].killed = false;
        population[i].total_value = 0;
        population[i].amount_itens = 0;
    }
    log(param, DONE, "Inicializando populacao com valores zerados");
    return true;
}

bool _genetic_manager_active_random(PARAM param, PERSON *population, int amount_population) {
    long index;
    float value;

    log(param, INIT, "Gerando aleatoriamente a populacao (Populacao: %d / Itens: %d)", amount_population, DEF_MAX_LEN_ITENS);

    srand(time(0));
    for(int i = 0; i < amount_population; i++) {
        value = 0;
        index = 0;
        do {
            index = rand() % DEF_MAX_LEN_ITENS;
            if ((value + param->db_itens[index].value) > param->budget) { 
                break;
            }

            value += param->db_itens[index].value;
            population[i].amount_itens++;
            population[i].total_value = value;
            population[i].itens[index] = true;
        }while(true);
    }

    log(param, DONE, "Gerado aleatoriamente a populacao (Populacao: %d / Itens: %d)", amount_population, DEF_MAX_LEN_ITENS);
    return true;
}

int _genetic_manager_change_to_new_array(PARAM param, PERSON *population, PERSON *pop_out, int amount_population, int init) {
    bool pass;
    int index_total;
    int amount_selected[DEF_MAX_LEN_POPULATION];
    float value_selected[DEF_MAX_LEN_POPULATION];

    if(init == 0){  /// Caso seja a primeira vez
        memset(pop_out, 0, sizeof(PERSON) * amount_population+1);
        memcpy(pop_out, population, sizeof(PERSON) * amount_population+1);
        return amount_population;
    }

    /// Capturando os ja selecionados
    for (int index = 0; index < amount_population; index++) {
        value_selected[index]  = population[index].total_value;
        amount_selected[index] = population[index].amount_itens;
    }

    /// Transferindo dados para nova array
    index_total = 0;
    for (int index = init; index < DEF_MAX_LEN_POPULATION; index++) {
        pass = false;
        for (int j = 0; j < DEF_MAX_LEN_POPULATION; j++) {
            if (amount_selected[j] == population[index].amount_itens) {
                if (value_selected[j] == population[index].total_value) {
                    pass = true;
                    break;
                }
            }
        }

        if (pass == true) {
            continue;
        }

        index_total++;
        pop_out[index].amount_itens = population[index].amount_itens;
        pop_out[index].total_value = population[index].total_value;
        if (index_total == amount_population) {
            break;
        }
    }
    memset(population+index_total, 0, sizeof(PERSON) * index_total);
    return index_total;
}
