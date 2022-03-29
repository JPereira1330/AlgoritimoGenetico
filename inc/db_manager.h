#ifndef _INC_DB_MANAGER_H_
#define _INC_DB_MANAGER_H_

#include <stdbool.h>

#include "param.h"
#include "type_structs.h"

bool db_manager_load_database(PARAM param, MDB *database);
_DB_ROW db_manager_ret_row_by_index(PARAM param, MDB database, int index);

#endif  // _INC_DB_MANAGER_H_