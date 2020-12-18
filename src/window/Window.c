#include <gtk/gtk.h>

#include "../thread/MainThread.h"
#include "Window.h"

void window_init(GtkApplication* application){
    //初始化程序窗口
    GtkWidget* window=gtk_application_window_new(application);

    //设置程序窗口参数
    gtk_window_set_title(GTK_WINDOW(window),"助屋棋");              //设置窗口标题
    gtk_window_set_default_size(GTK_WINDOW(window),640,480);        //设置窗口大小
    gtk_window_set_decorated(GTK_WINDOW(window),FALSE);             //隐藏窗口装饰

    //读取程序界面文件
    GtkBuilder* builder=gtk_builder_new();
    gtk_builder_add_from_file(builder,"./ui/window.ui",NULL);

    //获取并添加主界面
    GObject* root=gtk_builder_get_object(builder,"root");
    gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(root));

    //窗口按钮
    GObject* leftButton=gtk_builder_get_object(builder,"leftButton");
    GObject* middleButton=gtk_builder_get_object(builder,"middleButton");
    GObject* rightButton=gtk_builder_get_object(builder,"rightButton");

    //窗口标题
    GObject* titleLabel=gtk_builder_get_object(builder,"titleLabel");

    //标题面板
    GObject* titleBar=gtk_builder_get_object(builder,"titleBar");

    //窗口按钮事件
    g_signal_connect(leftButton,"enter-notify-event",G_CALLBACK(mouse_entry_button),NULL);
    g_signal_connect(leftButton,"leave-notify-event",G_CALLBACK(mouse_leave_button),NULL);
    g_signal_connect(leftButton,"button-press-event",G_CALLBACK(left_button),window);

    g_signal_connect(middleButton,"enter-notify-event",G_CALLBACK(mouse_entry_button),NULL);
    g_signal_connect(middleButton,"leave-notify-event",G_CALLBACK(mouse_leave_button),NULL);
    g_signal_connect(middleButton,"button-press-event",G_CALLBACK(middle_button),window);

    g_signal_connect(rightButton,"enter-notify-event",G_CALLBACK(mouse_entry_button),rightButton);
    g_signal_connect(rightButton,"leave-notify-event",G_CALLBACK(mouse_leave_button),rightButton);
    g_signal_connect(rightButton,"button-press-event",G_CALLBACK(right_button),NULL);

    //窗口最大化事件
    g_signal_connect(window,"window-state-event",G_CALLBACK(window_event),middleButton);

    //窗口按钮动画
    g_signal_connect(titleLabel,"enter-notify-event",G_CALLBACK(mouse_entry_button),NULL);
    g_signal_connect(titleLabel,"leave-notify-event",G_CALLBACK(mouse_leave_button),NULL);
    g_signal_connect(titleLabel,"button-press-event",G_CALLBACK(title_button),NULL);
    
    //窗口调整事件
    g_signal_connect(root,"button-press-event",G_CALLBACK(window_resize),window);

    //窗口移动事件
    g_signal_connect(titleBar,"button-press-event",G_CALLBACK(window_move),window);

    //加载CSS文件
    GtkCssProvider* provider=gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider,"./ui/theme.css",NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);

    //显示窗口
    gtk_widget_show_all(window);

}

gboolean mouse_entry_button(GtkWidget *eventbox,GdkEventButton *event,gpointer data){
    if(data!=NULL){
        gtk_widget_set_name(eventbox,"entry-button-red");

    }else{
        gtk_widget_set_name(eventbox,"entry-button");

    }
  
    return FALSE;

}

gboolean mouse_leave_button(GtkWidget *eventbox,GdkEventButton *event,gpointer data){
    if(data!=NULL){
        gtk_widget_set_name(eventbox,"leave-button-red");

    }else{
        gtk_widget_set_name(eventbox,"leave-button");

    }
  
    return FALSE;

}

gboolean window_move(GtkWidget* widget,GdkEventButton* event,gpointer data){
    if(event->y>RESIZE_BORDER || gtk_window_is_maximized(GTK_WINDOW(data))){
        if(event->type==GDK_2BUTTON_PRESS){
            if(gtk_window_is_maximized(GTK_WINDOW(data))){
                gtk_window_unmaximize(GTK_WINDOW(data));
            
            }else{
                gtk_window_maximize(GTK_WINDOW(data));

            }

        }else if(event->type==GDK_BUTTON_PRESS){
            gtk_window_begin_move_drag(GTK_WINDOW(data),event->button,event->x_root,event->y_root,event->time);

        }

    }

    return FALSE;

}

gboolean window_resize(GtkWidget* widget,GdkEventButton* event,gpointer data){
    if(!gtk_window_is_maximized(GTK_WINDOW(data))){
        int width;
        int height;

        gtk_window_get_size(GTK_WINDOW(data),&width,&height);

        //上边
        if((event->x>RESIZE_BORDER&&event->x<width-RESIZE_BORDER)&&(event->y>0&&event->y<RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_NORTH,event->button,event->x_root,event->y_root,event->time);

        //下边
        }else if((event->x>RESIZE_BORDER&&event->x<width-RESIZE_BORDER)&&(event->y>height-RESIZE_BORDER&&event->y<height)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_SOUTH,event->button,event->x_root,event->y_root,event->time);

        //左边
        }else if((event->x>0&&event->x<RESIZE_BORDER)&&(event->y>RESIZE_BORDER&&event->y<height-RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_WEST,event->button,event->x_root,event->y_root,event->time);

        //右边
        }else if((event->x>width-RESIZE_BORDER&&event->x<width)&&(event->y>RESIZE_BORDER&&event->y<height-RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_EAST,event->button,event->x_root,event->y_root,event->time);

        //左上
        }else if((event->x>0&&event->x<RESIZE_BORDER)&&(event->y>0&&event->y<RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_NORTH_WEST,event->button,event->x_root,event->y_root,event->time);

        //右上
        }else if((event->x>width-RESIZE_BORDER&&event->x<width)&&(event->y>0&&event->y<RESIZE_BORDER)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_NORTH_EAST,event->button,event->x_root,event->y_root,event->time);

        //左下
        }else if((event->x>0&&event->x<RESIZE_BORDER)&&(event->y>height-RESIZE_BORDER&&event->y<height)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_SOUTH_WEST,event->button,event->x_root,event->y_root,event->time);

        //右下
        }else if((event->x>width-RESIZE_BORDER&&event->x<width)&&(event->y>height-RESIZE_BORDER&&event->y<height)){
            gtk_window_begin_resize_drag(GTK_WINDOW(data),GDK_WINDOW_EDGE_SOUTH_EAST,event->button,event->x_root,event->y_root,event->time);

        }

    }

    return FALSE;

}

gboolean window_event(GtkWidget* widget,GdkEventWindowState* event,gpointer data){
    //窗口最大化
    if(event->new_window_state==43908){
        gtk_image_set_from_file(GTK_IMAGE(gtk_container_get_children(GTK_CONTAINER(data))->data),"./res/icon/windowing.svg");

    //窗口窗口化
    }else if(event->new_window_state==87168){
        gtk_image_set_from_file(GTK_IMAGE(gtk_container_get_children(GTK_CONTAINER(data))->data),"./res/icon/maximize.svg");

    }

    return FALSE;

}

gboolean left_button(GtkWidget* widget,GdkEventButton* event,gpointer data){
    gtk_window_iconify(GTK_WINDOW(data));

    return FALSE;

}

gboolean middle_button(GtkWidget* widget,GdkEventButton* event,gpointer data){
    if(gtk_window_is_maximized(GTK_WINDOW(data))){
        gtk_window_unmaximize(GTK_WINDOW(data));

    }else{
        gtk_window_maximize(GTK_WINDOW(data));

    }

    return FALSE;

}

gboolean right_button(GtkWidget* widget,GdkEventButton* event,gpointer data){
    //退出程序
    exit(0);

    return FALSE;

}

gboolean title_button(GtkWidget* widget,GdkEventButton* event,gpointer data){
    printf("原作：あやか\n");
    printf("设计：Longyou_dwd\n");
    printf("制作：谷林\n");

    return FALSE;

}
