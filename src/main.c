#include <stdio.h>
#include <stdlib.h>

#include "param.h"
#include "internal_types.h"
#include "genetic_manager.h"

PERSON person[DEF_MAX_LEN_POPULATION];

int main(void) {

    PARAM param;   
 
    param = param_init();
    if(param == NULL) {
        return EXIT_FAILURE;
    }

    param->epoch = 25;
    param->budget = 5000;
    genetic_manager_create_population(param, person, DEF_MAX_LEN_POPULATION);

    param_free(param);

    return 0;
}