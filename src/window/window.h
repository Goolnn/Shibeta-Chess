#ifndef SHIBETA_CHESS_WINDOW
#define SHIBETA_CHESS_WINDOW

#include <gtk/gtk.h>

#define RESIZE_BORDER 10

typedef struct Window{
    GtkWidget* source;

    GtkWidget* title;
    GtkWidget* subtitle;

} Window;

//新建窗口
Window* shibetachess_window_new(GtkApplication*);

//设置窗口标题文本
void shibetachess_window_set_title(Window*,const char*);
//设置窗口副标题文本
void shibetachess_window_set_subtitle(Window*,const char*);

//显示窗口
void shibetachess_window_show(Window*);

#endif
