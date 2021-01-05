#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "server_list.h"

bool shibeta_chess_server_list_is_exist(){
    if(access(SERVER_LIST_FILE_DIR,F_OK)==0&&access(SERVER_LIST_FILE_PATH,F_OK)==0){
        return true;

    }

    return false;

}

void shibeta_chess_server_list_create(){
    if(access(SERVER_LIST_FILE_DIR,F_OK)!=0){
        mkdir(SERVER_LIST_FILE_DIR,S_IRWXU|S_IRWXG|S_IRWXO);

    }

    if(access(SERVER_LIST_FILE_PATH,F_OK)!=0){
        FILE* serverlist=fopen(SERVER_LIST_FILE_PATH,"w");

        //写入默认服务器列表
        char* default_address=DEFAULT_ADDRESS;
        short default_port=DEFAULT_PORT;

        fwrite(default_address,strlen(DEFAULT_ADDRESS),1,serverlist);
        fwrite(&default_port,2,1,serverlist);

        fclose(serverlist);

    }

}
