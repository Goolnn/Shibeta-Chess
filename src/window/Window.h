#ifndef SHIBETA_CHESS_WINDOW
#define SHIBETA_CHESS_WINDOW

#include <gtk/gtk.h>

#define RESIZE_BORDER 10

void window_init(GtkApplication* application);

// 鼠标进入或离开按钮时触发的事件按钮时触发的事件
gboolean mouse_entry_button(GtkWidget*,GdkEventButton*,gpointer);
gboolean mouse_leave_button(GtkWidget*,GdkEventButton*,gpointer);

// 窗口移动事件和窗口大小调整事件
gboolean window_move(GtkWidget*,GdkEventButton*,gpointer);
gboolean window_resize(GtkWidget*,GdkEventButton*,gpointer);

gboolean window_event(GtkWidget*,GdkEventWindowState*,gpointer);

// 窗口按钮点击事件
gboolean left_button(GtkWidget*,GdkEventButton*,gpointer);
gboolean middle_button(GtkWidget*,GdkEventButton*,gpointer);
gboolean right_button(GtkWidget*,GdkEventButton*,gpointer);

gboolean title_button(GtkWidget*,GdkEventButton*,gpointer);

#endif
