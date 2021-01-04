#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "user_data.h"

char* player_name;

bool shibeta_chess_user_data_is_exist(){
    if(access(USER_DATA_FILE_DIR,F_OK)==0&&access(USER_DATA_FILE_PATH,F_OK)==0){
        return true;

    }

    return false;

}

void shibeta_chess_user_data_create(){
    mkdir(USER_DATA_FILE_DIR,S_IRWXU|S_IRWXG|S_IRWXO);

    FILE* userdata=fopen(USER_DATA_FILE_PATH,"w");

    fclose(userdata);
    
}

void shibeta_chess_user_data_init(){
    //打开文件
    FILE* userdata=fopen(USER_DATA_FILE_PATH,"rb");

    //读取玩家名字字节长度
    byte player_name_byte_size[1];
    fread(player_name_byte_size,sizeof(player_name_byte_size),1,userdata);

    //读取玩家名字的字节信息
    byte player_name_byte[*player_name_byte_size];
    fread(player_name_byte,sizeof(player_name_byte),1,userdata);

    //设置玩家名称
    player_name=malloc(*player_name_byte_size+1);
    memcpy(player_name,player_name_byte,*player_name_byte_size);
    player_name[*player_name_byte_size]='\0';

    //关闭流
    fclose(userdata);

}

char* shibeta_chess_user_data_get_player_name(){
    return player_name;

}

void shibeta_chess_user_data_write_player_name(const char* name){
    //打开文件
    FILE* userdata=fopen(USER_DATA_FILE_PATH,"wb");

    byte len[1];

    len[0]=(byte)strlen(name);

    fwrite(len,1,1,userdata);
    fwrite(name,strlen(name),1,userdata);

    fclose(userdata);

}

