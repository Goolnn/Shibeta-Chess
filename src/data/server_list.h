#ifndef SHIBETA_CHESS_DATA_SERVER_LIST
#define SHIBETA_CHESS_DATA_SERVER_LIST

#include <stdbool.h>

#define SERVER_LIST_FILE_PATH "./data/Serverlist.scd"
#define SERVER_LIST_FILE_DIR "./data"

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 2021

bool shibeta_chess_server_list_is_exist();
void shibeta_chess_server_list_create();

#endif
