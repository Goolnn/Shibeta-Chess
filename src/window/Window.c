#include <gtk/gtk.h>

#include "Window.h"

const int RESIZE_BORDER = 5;

void Window_InitWindow(){
    // 实例化GtkBuilder并加载UI文件
    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "./ui/window.ui", NULL);

    // 获取程序窗口
    GObject* window = gtk_builder_get_object(builder, "window");

    // 窗口按钮注册单击事件
    GObject* leftButton = gtk_builder_get_object(builder, "leftButton");
    GObject* middleButton = gtk_builder_get_object(builder, "middleButton");
    GObject* rightButton = gtk_builder_get_object(builder, "rightButton");

    g_signal_connect(leftButton, "enter-notify-event", G_CALLBACK(EntryButton), NULL);
    g_signal_connect(leftButton, "leave-notify-event", G_CALLBACK(LeaveButton), NULL);
    g_signal_connect(leftButton, "button-press-event", G_CALLBACK(LeftButtonPress), window);

    g_signal_connect(middleButton, "enter-notify-event", G_CALLBACK(EntryButton), NULL);
    g_signal_connect(middleButton, "leave-notify-event", G_CALLBACK(LeaveButton), NULL);
    g_signal_connect(middleButton, "button-press-event", G_CALLBACK(MiddleButtonPress), window);

    g_signal_connect(rightButton, "enter-notify-event", G_CALLBACK(EntryButton), rightButton);
    g_signal_connect(rightButton, "leave-notify-event", G_CALLBACK(LeaveButton), rightButton);
    g_signal_connect(rightButton, "button-press-event", G_CALLBACK(RightButtonPress), NULL);

    // 注册窗口移动和大小调整事件
    GObject* root = gtk_builder_get_object(builder,"root");
    g_signal_connect(root, "button-press-event", G_CALLBACK(WindowResize), window);
    
    GObject* titleBar = gtk_builder_get_object(builder,"titleBar");
    g_signal_connect(titleBar, "button-press-event", G_CALLBACK(WindowMove), window);
    
    // 注册窗口状态改变事件
    g_signal_connect(window, "window-state-event", G_CALLBACK(WindowEvent), middleButton);

    // 加载CSS文件
    GtkCssProvider* provider = gtk_css_provider_new();
    
    gtk_css_provider_load_from_path(provider, "./ui/theme.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // 显示窗口以及所有控件
    gtk_widget_show_all(GTK_WIDGET(window));

}

gboolean EntryButton(GtkWidget *eventbox, GdkEventButton *event, gpointer data){
    if(data != NULL){
        gtk_widget_set_name(eventbox, "entry-button-red");

    }else{
        gtk_widget_set_name(eventbox, "entry-button");

    }
  
    return FALSE;

}

gboolean LeaveButton(GtkWidget *eventbox, GdkEventButton *event, gpointer data){
    if(data != NULL){
        gtk_widget_set_name(eventbox, "leave-button-red");

    }else{
        gtk_widget_set_name(eventbox, "leave-button");

    }
  
    return FALSE;

}

gboolean WindowMove(GtkWidget* widget, GdkEventButton* event, gpointer data){
    if(event->y > RESIZE_BORDER || gtk_window_is_maximized(GTK_WINDOW(data))){
        if(event->type == GDK_2BUTTON_PRESS){
            if(gtk_window_is_maximized(GTK_WINDOW(data))){
                gtk_window_unmaximize(GTK_WINDOW(data));
            
            }else{
                gtk_window_maximize(GTK_WINDOW(data));

            }

        }else if(event->type == GDK_BUTTON_PRESS){
            gtk_window_begin_move_drag(GTK_WINDOW(data), event->button, event->x_root, event->y_root, event->time);

        }

    }

    return FALSE;

}

gboolean WindowResize(GtkWidget* widget, GdkEventButton* event, gpointer data){
    if(!gtk_window_is_maximized(GTK_WINDOW(data))){
        int width;
        int height;

        gtk_window_get_size(GTK_WINDOW(data), &width,&height);

        // 上边
        if((event->x > RESIZE_BORDER && event->x < width - RESIZE_BORDER) && (event->y > 0 && event->y < RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_NORTH, event->button, event->x_root, event->y_root, event->time);

        // 下边
        }else if((event->x > RESIZE_BORDER && event->x < width - RESIZE_BORDER) && (event->y > height - RESIZE_BORDER && event->y < height)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_SOUTH, event->button, event->x_root, event->y_root, event->time);

        // 左边
        }else if((event->x > 0 && event->x < RESIZE_BORDER) && (event->y > RESIZE_BORDER && event->y < height - RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_WEST, event->button, event->x_root, event->y_root, event->time);

        // 右边
        }else if((event->x > width - RESIZE_BORDER && event->x < width) && (event->y > RESIZE_BORDER && event->y < height - RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_EAST, event->button, event->x_root, event->y_root, event->time);

        // 左上
        }else if((event->x > 0 && event->x < RESIZE_BORDER) && (event->y > 0 && event->y < RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_NORTH_WEST, event->button, event->x_root, event->y_root, event->time);

        // 右上
        }else if((event->x > width - RESIZE_BORDER && event->x < width) && (event->y > 0 && event->y < RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_NORTH_EAST, event->button, event->x_root, event->y_root, event->time);

        // 左下
        }else if((event->x > 0 && event->x < RESIZE_BORDER) && (event->y > height - RESIZE_BORDER && event->y < height)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_SOUTH_WEST, event->button, event->x_root, event->y_root, event->time);

        // 右下
        }else if((event->x > width - RESIZE_BORDER && event->x < width) && (event->y > height - RESIZE_BORDER && event->y < height)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data), GDK_WINDOW_EDGE_SOUTH_EAST, event->button, event->x_root, event->y_root, event->time);

        }

    }

    return FALSE;

}

gboolean WindowEvent(GtkWidget* widget, GdkEventWindowState* event, gpointer data){
    // 窗口最大化
    if(event->new_window_state == 43908){
        gtk_image_set_from_file(GTK_IMAGE(gtk_container_get_children(GTK_CONTAINER(data))->data), "./res/icon/windowing.svg");

    // 窗口窗口化
    }else if(event->new_window_state == 87168){
        gtk_image_set_from_file(GTK_IMAGE(gtk_container_get_children(GTK_CONTAINER(data))->data), "./res/icon/maximize.svg");

    }

    return FALSE;

}

gboolean LeftButtonPress(GtkWidget* widget, GdkEventButton* event, gpointer data){
    gtk_window_iconify(GTK_WINDOW(data));

}

gboolean MiddleButtonPress(GtkWidget* widget, GdkEventButton* event, gpointer data){
    if(gtk_window_is_maximized(GTK_WINDOW(data))){
        gtk_window_unmaximize(GTK_WINDOW(data));

    }else{
        gtk_window_maximize(GTK_WINDOW(data));

    }

}

gboolean RightButtonPress(GtkWidget* widget, GdkEventButton* event, gpointer data){
    //退出程序
    gtk_main_quit();

}
