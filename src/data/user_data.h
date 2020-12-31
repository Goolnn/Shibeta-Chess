#ifndef SHIBETA_CHESS_DATA_USER_DATA
#define SHIBETA_CHESS_DATA_USER_DATA

#include <stdbool.h>

#define USER_DATA_PATH "./data/user_data"

bool shibeta_chess_user_data_is_exist();

void shibeta_chess_user_data_create();

#endif
