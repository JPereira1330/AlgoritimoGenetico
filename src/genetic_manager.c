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
bool _genetic_manager_active_random(PARAM param, PERSON *population, int amount_population);
bool _genetic_manager_init_pop(PARAM param, PERSON *population, int amount_population);

//////////////////////////////////////////
///     IMPLEMENTACAO DOS METODOS PUBLICOS
//////////////////////////////////////////

bool genetic_manager_create_population(PARAM param, PERSON *population, int amount_population) {

    /// Carregando populacao em memoria e populando
    _genetic_manager_load_database(param);
    _genetic_manager_init_pop(param, population, amount_population);
    _genetic_manager_active_random(param, population, amount_population);

    return true;
}

bool genetic_manager_mate_population(PARAM param, PERSON *population, PERSON *new_pop) {
    int index_init;

    log(param, INIT, "Cruzando populacao");

    memset(new_pop, 0, sizeof(PERSON)* DEF_MAX_LEN_POPULATION);

    // Percorre toda a população
    for(int i = 0; i < DEF_MAX_LEN_POPULATION; i++) {
        index_init = i+1;
        for(int j = 0; j < DEF_MAX_LEN_ITENS; j++) {
            if(j > (DEF_MAX_LEN_ITENS/2)) {
                if(index_init == DEF_MAX_LEN_POPULATION) {
                    new_pop[i].itens[j] = population[0].itens[j];
                }else{
                    new_pop[i].itens[j] = population[index_init].itens[j];
                }
            }else {
                new_pop[i].itens[j] = population[i].itens[j];
            }
        }
    }

    genetic_manager_calc_fit(param, new_pop, DEF_MAX_LEN_POPULATION);

    log(param, DONE, "Cruzando populacao");
    return true;
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

bool genetic_manager_fix_population(PARAM param, PERSON *population, PERSON *new_pop, int amount_population) {

    int index;
    int value;
    bool selected_dad[DEF_MAX_LEN_ITENS];
    bool selected_son[DEF_MAX_LEN_ITENS];

    log(param, INIT, "Atualizando populacao");

    memset(selected_dad, 0, sizeof(bool) * DEF_MAX_LEN_ITENS);
    memset(selected_son, 0, sizeof(bool) * DEF_MAX_LEN_ITENS);

    for (int i = 0; i < amount_population; i++) {
        index = 0;
        value = 0;

        /// Capturando os melhores da populacao pai
        if (i < DEF_MAX_BETTER_PASS) {
            /// Capturando os melhores
            for (int j = 0; j < amount_population; j++) {
                if (selected_dad[j])
                    continue;
                if (value < population[j].amount_itens) {
                    index = j;
                    value = population[j].amount_itens;
                }
            }

            /// Salvando o melhor
            for(int k = 0; k < DEF_MAX_LEN_ITENS; k++){
                selected_dad[index] = true;
                person_aux[i].itens[k] = population[index].itens[k];
            }
        
        /// Capturando os melreos da populacao filho
        } else {
            /// Capturando os melhores
            for (int j = 0; j < amount_population; j++) {
                if (selected_son[j])
                    continue;
                if (value < new_pop[j].amount_itens) {
                    index = j;
                    value = new_pop[j].amount_itens;
                }
            }

            /// Salvando o melhor
            for(int k = 0; k < DEF_MAX_LEN_ITENS; k++){
                selected_son[index] = true;
                person_aux[i].itens[k] = new_pop[index].itens[k];
            }
        }
    }
    genetic_manager_calc_fit(param, person_aux, DEF_MAX_LEN_POPULATION);
    memset(population, 0, sizeof(PERSON) * DEF_MAX_LEN_POPULATION);
    memcpy(population, person_aux, sizeof(PERSON) * DEF_MAX_LEN_POPULATION);

    log(param, DONE, "Atualizando populacao");

    return true;
}

/// ! Otimizar
bool genetic_manager_kill_population(PARAM param, PERSON *population, int amount_population) {
    bool killed[DEF_MAX_LEN_POPULATION];

    memset(killed, 0, sizeof(bool) * DEF_MAX_LEN_POPULATION);
    for(int i_pop = 0; i_pop < amount_population; i_pop++) {
        if(population[i_pop].total_value > param->budget) {
            killed[i_pop] = true;
        }
    }

    for(int i_pop = 0; i_pop < amount_population; i_pop++) {

        if(killed[i_pop] == false) {
            continue;
        }

        population[i_pop].total_value = 0;
        population[i_pop].amount_itens = 0;
        for(int i_itens = 0; i_itens < DEF_MAX_LEN_ITENS; i_itens++) {
            population[i_pop].itens[i_itens] = 0;
        }

    }

    return true;
}

bool genetic_manager_show_better(PARAM param, PERSON *population, int amount_population, int amount_to_show) {
    int index;
    int quant;
    int len_pop;
    bool selected[DEF_MAX_LEN_ITENS];

    memset(selected, 0, sizeof(bool)*DEF_MAX_LEN_ITENS);
    for(int repeat = 0; repeat < amount_to_show; repeat++ ) {
        quant = 0;
        index = 0;
        for(int i_pop = 0; i_pop < amount_population; i_pop++) {
            if(selected[i_pop] == false){
                if(quant < population[i_pop].amount_itens) {
                    index = i_pop;
                    quant = population[i_pop].amount_itens;
                }
            }
        }
        selected[index] = true;
        printf("[%d;%d;R$%2.f]\t", index, population[index].amount_itens, population[index].total_value);
    }

    len_pop = 0;
    for(int i = 0; i < amount_population; i++ ) {
        if(population[i].amount_itens != 0) {
            len_pop++;
        }
    }

    printf("(POP: %d)\n", len_pop);
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
