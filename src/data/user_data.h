#ifndef SHIBETA_CHESS_DATA_USER_DATA
#define SHIBETA_CHESS_DATA_USER_DATA

#include <stdbool.h>

#define USER_DATA_FILE_PATH "./data/Userdata.scd"
#define USER_DATA_FILE_DIR "./data"

typedef unsigned char byte;

bool shibeta_chess_user_data_is_exist();
void shibeta_chess_user_data_create();

void shibeta_chess_user_data_init();

char* shibeta_chess_user_data_get_player_name();

void shibeta_chess_user_data_write_player_name(const char*);

#endif
