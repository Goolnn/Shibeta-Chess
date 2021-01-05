#include <gtk/gtk.h>

#include "window/window.h"

#include "data/server_list.h"
#include "data/user_data.h"

void client_activate(GtkApplication*,gpointer);
void server_activate(GtkApplication*,gpointer);

/*
 * 原作：あやか
 * 设计：Longyou_dwd
 * 制作：谷林
 */
int main(int argc,char* argv[]){
    //创建程序
    GtkApplication* application=gtk_application_new("com.goolnn.shibetachess",G_APPLICATION_FLAGS_NONE);
    
    //判断启动客户端或是服务端
    //客户端
    if(argc==1||(argc==2&&strcmp(argv[1],"--client")==0)){
        g_signal_connect(application,"activate",G_CALLBACK(client_activate),NULL);

    //服务端
    }else if(argc==2&&strcmp(argv[1],"--server")==0){
        g_signal_connect(application,"activate",G_CALLBACK(server_activate),NULL);

    }

    //运行程序
    int status=g_application_run(G_APPLICATION(application),0,NULL);

    //释放资源
    g_object_unref(application);

    return status;

}

void client_activate(GtkApplication* application,gpointer data){
    //启动窗口DEBUG界面
    gtk_window_set_interactive_debugging(TRUE);

    //初始化窗口参数
    //实例化窗口
    GtkWidget* window=shibeta_chess_window_new(application);

    //判断是否第一次游玩游戏
    if(shibeta_chess_user_data_is_exist()){
        shibeta_chess_user_data_init();

        //判断服务器列表是否存在
        if(shibeta_chess_server_list_is_exist()){


        }else{
            shibeta_chess_server_list_create();

        }

    }else{
        shibeta_chess_window_set_interface(window,INTERFACE_NAME_INPUT);

    }

    //显示窗口
    gtk_widget_show_all(window);

}

void server_activate(GtkApplication* application,gpointer data){
    // Window* window=shibeta_chess_window_new(application);

    // shibeta_chess_window_set_title(window,"助屋棋 - 服务端");
    // shibeta_chess_window_show(window);

}
