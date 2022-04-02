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

    memset(new_pop, 0, sizeof(PERSON)* DEF_MAX_MULT_POP);
    for(int index = 0; index < DEF_MAX_MULT_POP; index++) {
        index_init = index+1;
        for(int i_itens = 0; i_itens < DEF_MAX_LEN_ITENS; i_itens++) {
            for(int i_pop = index_init; i_pop <= index_init; i_pop++) {
                if(i_itens > (DEF_MAX_LEN_ITENS/2)) {
                    if(i_pop == DEF_MAX_LEN_POPULATION) {
                        new_pop[index].itens[i_itens] = population[0].itens[i_itens];
                    }else{
                        new_pop[index].itens[i_itens] = population[i_pop].itens[i_itens];
                    }
                }else {
                    new_pop[index].itens[i_itens] = population[index].itens[i_itens];
                }
            }
        }
    }
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
    int quant;
    bool selected[DEF_MAX_LEN_ITENS];

    memset(selected, 0, sizeof(bool)*DEF_MAX_LEN_ITENS);
    for(int repeat = 0; repeat < amount_population; repeat++ ) {
        quant = 0;
        index = 0;
        for(int i_pop = 0; i_pop < amount_population; i_pop++) {
            if(selected[i_pop] == false){
                if(quant < new_pop[i_pop].amount_itens) {
                    index = i_pop;
                    quant = new_pop[i_pop].amount_itens;
                }
            }
        }
        selected[index] = true;
        for(int i = 0; i < DEF_MAX_LEN_ITENS; i++) {
            population[repeat].itens[i] = new_pop[index].itens[i];
        }
    }
    
    genetic_manager_calc_fit(param, population, amount_population);

    return true;
}

/// ! Otimizar
bool genetic_manager_kill_population(PARAM param, PERSON *population, int amount_population) {
    bool killed[DEF_MAX_MULT_POP];

    memset(killed, 0, sizeof(bool) * DEF_MAX_MULT_POP);
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
        printf("[%d;%d;R$%2.f]", index, population[index].amount_itens, population[index].total_value);
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

/*
bool _genetic_manager_active_random(PARAM param, PERSON *population, int amount_population) {
    long index,j;

    log(param, INIT, "Gerando aleatoriamente a populacao (Populacao: %d / Itens: %d)", amount_population, DEF_MAX_LEN_ITENS);

    srand(time(0));
    j = rand() % DEF_MAX_INIT_ITENS;
    for(int i = 0; i < amount_population; i++) {
        index = 0;
        for(int k = 0; k < j; k++) {
            index = rand() % DEF_MAX_INIT_ITENS;
            population[i].itens[index] = true;
        }
    }

    log(param, DONE, "Gerando aleatoriamente a populacao (Populacao: %d / Itens: %d)", amount_population, DEF_MAX_LEN_ITENS);
    return true;
}*/
