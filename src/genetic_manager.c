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

bool genetic_manager_mate_population(PARAM param, PERSON *population, int amount_population) {
    PERSON new_pop[DEF_MAX_LEN_POPULATION*DEF_MULT_MATE];

    for (int i = 0; i < amount_population; i++) {
        for (int j = 0; j < DEF_MULT_MATE; j++) {
            /// multiplicar sempre com os proximos DEF_MULT_MATE dna
        }
    }
    

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

    log(param, DONE, "Gerando aleatoriamente a populacao (Populacao: %d / Itens: %d)", amount_population, DEF_MAX_LEN_ITENS);
    return true;
}
