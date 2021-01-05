#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "user_data.h"

UserData* user_data=NULL;

/*
 * 判断用户文件是否存在
 * 用于判定用户是否第一次打开游戏
 */
bool shibeta_chess_user_data_is_exist(){
    //判断data文件夹和Userdata.scd文件是否存在
    if(access(USER_DATA_FILE_DIR,F_OK)==0&&access(USER_DATA_FILE_PATH,F_OK)==0){
        return true;

    }

    return false;

}

/*
 * 创建用户文件
 */
void shibeta_chess_user_data_create(){
    if(access(USER_DATA_FILE_DIR,F_OK)!=0){
        mkdir(USER_DATA_FILE_DIR,S_IRWXU|S_IRWXG|S_IRWXO);

    }

    if(access(USER_DATA_FILE_PATH,F_OK)!=0){
        FILE* userdata=fopen(USER_DATA_FILE_PATH,"w");

        fclose(userdata);

    }
    
}

/*
 * 初始化
 * 用于读取用户文件内容，比如加载用户名什么之类的
 */
void shibeta_chess_user_data_init(){
    //初始化用户数据结构体
    user_data=calloc(1,sizeof(UserData));

    //打开文件
    FILE* userdata=fopen(USER_DATA_FILE_PATH,"rb");

    //读取玩家名字字节长度
    byte player_name_byte_size[1];
    fread(player_name_byte_size,sizeof(player_name_byte_size),1,userdata);
    int player_name_size=*player_name_byte_size;

    //读取玩家名字的字节信息
    byte player_name_byte[player_name_size];
    fread(player_name_byte,sizeof(player_name_byte),1,userdata);

    //设置玩家名称
    free(user_data->player_name);                                           //管他指针有没有指向，释放就完了-_-

    user_data->player_name=calloc(1,player_name_size+1);                    //申请一个比名字字节长度大1的内存，最后一个字节放终止符
    memcpy(user_data->player_name,player_name_byte,player_name_size);       //复制名字字节给指针（注：之前没有注意，直接把局部变量返回出去，导致局部变量释放以后字节信息变成一堆乱码-_-）

    //关闭流
    fclose(userdata);

}

char* shibeta_chess_user_data_get_player_name(){
    return user_data->player_name;

}

void shibeta_chess_user_data_save(UserData user_data){
    //打开文件
    FILE* user_data_file=fopen(USER_DATA_FILE_PATH,"wb");

    byte len[1];

    len[0]=(byte)strlen(user_data.player_name);

    fwrite(len,1,1,user_data_file);
    fwrite(user_data.player_name,strlen(user_data.player_name),1,user_data_file);

    fclose(user_data_file);

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

