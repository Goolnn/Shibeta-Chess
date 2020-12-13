#include <gtk/gtk.h>

#include "window/Window.h"

/*
 * 助屋棋，一个以作品系列《西伯太的救助屋》为背景的联网多人棋类小游戏
 * 原作：あやか
 * 设计：Longyou_dwd
 * 制作：谷林
 */
int main(int argc, char* argv[]){
    // 初始化GTK
    gtk_init(&argc, &argv);
    // 初始化窗口
    Window_InitWindow();
    // 进入GTK主循环
    gtk_main();

    return 0;

}
