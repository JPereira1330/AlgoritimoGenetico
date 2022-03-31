#include "param.h"

#include <stdlib.h>
#include <stdbool.h>

#include "db_manager.h"
#include "type_structs.h"
#include "genetic_manager.h"

int main() {

    bool ret;
    PARAM param;   
    MDB database;
    POP population;

    param = param_init();
    if(param == NULL) {
        return EXIT_FAILURE;
    }

    param->epoch = 25;
    param->budget = 5000;

    ret = db_manager_load_database(param, &database);
    if(ret == false) {
        return EXIT_FAILURE;
    }

    population = genetic_manager_new();

    genetic_manager_create_population(param, database, population);
    for(int i = 0; i <= param->epoch; i++) {
        printf(" => EPOCA %d\n", i);
        genetic_manager_recreate_population(param, database, population);
        genetic_manager_debug_better(param, population, 5);
    }

    genetic_manager_free(population);


    param_free(param);

    return EXIT_SUCCESS;
}