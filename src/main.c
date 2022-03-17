#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "param.h"
#include "file_manager.h"

void main() {

    PARAM param;
    char buffer[100];

    param = param_init();

    file_manager_open(param, "teste.txt", "w");
    file_manager_write(param, "abcde", strlen("teste"));
    file_manager_close(param);

    file_manager_open(param, "teste.txt", "r");
    file_manager_read(param, buffer, sizeof(buffer));
    printf("Result: | %c | %s", buffer[0], buffer);
    file_manager_close(param);
    param_free(param);

}