#ifndef SHIBETA_CHESS_WINDOW_WINDOW
#define SHIBETA_CHESS_WINDOW_WINDOW

#include <gtk/gtk.h>

#define RESIZE_BORDER 10

typedef struct Window Window;

//新建窗口
Window* shibeta_chess_window_new(GtkApplication*);

//设置窗口标题文本
void shibeta_chess_window_set_title(Window*,const char*);
//设置窗口副标题文本
void shibeta_chess_window_set_subtitle(Window*,const char*);

//设置窗口主面板
void shibeta_chess_window_set_root(Window*,GtkWidget*);

//显示窗口
void shibeta_chess_window_show(Window*);

#endif
