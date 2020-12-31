#include <gtk/gtk.h>

#include "window/window.h"

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

    //未定义参数
    }else{
        printf("传入了未定义的参数！\n");

        return -1;

    }

    //运行程序
    int status=g_application_run(G_APPLICATION(application),0,NULL);

    //释放资源
    g_object_unref(application);

    return status;

}

void client_activate(GtkApplication* application,gpointer data){
    gtk_window_set_interactive_debugging(TRUE);

    //创建窗口
    Window* window=shibeta_chess_window_new(application);

    //设置窗口标题
    shibeta_chess_window_set_title(window,"助屋棋");

    //显示窗口
    shibeta_chess_window_show(window);

    // //读取程序界面文件
    // GtkBuilder* builder=gtk_builder_new();
    // gtk_builder_add_from_file(builder,"./ui/name_interface.ui",NULL);

    // shibeta_chess_window_set_root(window,GTK_WIDGET(gtk_builder_get_object(builder,"name-interface")));

    // if(shibeta_chess_user_data_is_exist()){
        

    // }else{
        

    // }

}

void server_activate(GtkApplication* application,gpointer data){
    // Window* window=shibeta_chess_window_new(application);

    // shibeta_chess_window_set_title(window,"助屋棋 - 服务端");
    // shibeta_chess_window_show(window);

}