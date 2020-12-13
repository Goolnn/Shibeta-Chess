#ifndef SHIBETA_CHESS_WINDOW
#define SHIBETA_CHESS_WINDOW

#include <gtk/gtk.h>

// 初始化窗口
void Window_InitWindow();

// 鼠标进入或离开按钮时触发的事件按钮时触发的事件
gboolean EntryButton(GtkWidget*, GdkEventButton*, gpointer);
gboolean LeaveButton(GtkWidget*, GdkEventButton*, gpointer);

// 窗口移动事件和窗口大小调整事件
gboolean WindowMove(GtkWidget*, GdkEventButton*, gpointer);
gboolean WindowResize(GtkWidget*, GdkEventButton*, gpointer);

gboolean WindowEvent(GtkWidget*, GdkEventWindowState*, gpointer);

// 窗口按钮点击事件
gboolean LeftButtonPress(GtkWidget*, GdkEventButton*, gpointer);
gboolean MiddleButtonPress(GtkWidget*, GdkEventButton*, gpointer);
gboolean RightButtonPress(GtkWidget*, GdkEventButton*, gpointer);

#endif
