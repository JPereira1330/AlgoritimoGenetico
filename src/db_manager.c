#include "db_manager.h"

#include <string.h>
#include <stdlib.h>

#include "file_manager.h"
#include "internal_defines.h"

bool db_manager_load_database(PARAM param, MDB *database) {

    bool ret;
    int bytes;
    unsigned int index;
    char *token;
    char buffer[MAX_LEN_NAME_ITEN];

    ret = file_manager_open(param, DATABASE_NAME, "r");
    if(ret == false) {
        return false;
    }

    index = -1;
    do { // '-' para erro, '0' para EOF, '+' para bytes faltantes
        index++;
        memset(buffer, 0, sizeof(buffer));
        bytes = file_manager_read(param, buffer, sizeof(buffer));
        if(bytes < 0)
            return false;

        // Atribuindo ao banco de dados
        database->last_id = index;
        database->rows[index].id = index;

        token = strtok(buffer, ";");
        snprintf(database->rows[index].iten, MAX_LEN_NAME_ITEN, "%s", token);

        token = strtok(NULL, ";");
        database->rows[index].value = atof(token);

    } while(bytes > 0);

    return true;
}

_DB_ROW db_manager_ret_row_by_index(PARAM param, MDB database, int index) {
    return database.rows[index];
}