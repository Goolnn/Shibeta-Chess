#include <gtk/gtk.h>
#include <pthread.h>

#include "window/Window.h"
#include "thread/MainThread.h"

void client_activate(GtkApplication*,gpointer);

/*
 * 助屋棋，一个以作品系列《西伯太的救助屋》为背景的联网多人棋类小游戏
 * 原作：あやか
 * 设计：Longyou_dwd
 * 制作：谷林
 */
int main(int argc,char* argv[]){
    //运行程序
    GtkApplication* application=gtk_application_new("com.goolnn.shibetachess",G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application,"activate",G_CALLBACK(client_activate),NULL);
    int status=g_application_run(G_APPLICATION(application),argc,argv);
    g_object_unref(application);

    return status;

}

void client_activate(GtkApplication* application,gpointer data){
    //初始化程序窗口
    window_init(application);

}
