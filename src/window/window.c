#include <gtk/gtk.h>

#include "window.h"

//初始化窗口
void window_init(Window*);

//鼠标进入或离开按钮时触发的事件按钮时触发的事件
gboolean mouse_entry_button(GtkWidget*,GdkEventButton*,gpointer);
gboolean mouse_leave_button(GtkWidget*,GdkEventButton*,gpointer);

//窗口移动事件和窗口大小调整事件
gboolean window_move(GtkWidget*,GdkEventButton*,gpointer);
gboolean window_resize(GtkWidget*,GdkEventButton*,gpointer);

gboolean window_event(GtkWidget*,GdkEventWindowState*,gpointer);

//窗口按钮点击事件
gboolean left_button(GtkWidget*,GdkEventButton*,gpointer);
gboolean middle_button(GtkWidget*,GdkEventButton*,gpointer);
gboolean right_button(GtkWidget*,GdkEventButton*,gpointer);

gboolean title_button(GtkWidget*,GdkEventButton*,gpointer);

Window* shibetachess_window_new(GtkApplication* application){
    //创建源窗口
    GtkWidget* source=gtk_application_window_new(application);

    //创建窗口
    Window* window=calloc(1,sizeof(Window));
    //初始化源窗口
    window->source=source;

    //初始化源窗口参数
    window_init(window);

    return window;

}

void shibetachess_window_set_title(Window* window,const char* title){
    gtk_window_set_title(GTK_WINDOW(window->source),title);
    gtk_label_set_label(GTK_LABEL(window->title),title);

}

void shibetachess_window_set_subtitle(Window* window,const char* subtitle){
    gtk_label_set_label(GTK_LABEL(window->subtitle),subtitle);

}

void shibetachess_window_show(Window* window){
    gtk_widget_show_all(window->source);

}

void window_init(Window* window){
    //初始化窗口透明
    GdkScreen* screen=gtk_widget_get_screen(GTK_WIDGET(window->source));
    GdkVisual* visual=gdk_screen_get_rgba_visual(screen);

    if(visual==NULL){
        visual=gdk_screen_get_system_visual(screen);

    }

    gtk_widget_set_visual(window->source,visual);

    //设置程序窗口参数
    gtk_window_set_default_size(GTK_WINDOW(window->source),640,480);        //设置窗口大小
    gtk_window_set_decorated(GTK_WINDOW(window->source),FALSE);             //隐藏窗口装饰

    gtk_widget_set_app_paintable(window->source,TRUE);

    //读取程序界面文件
    GtkBuilder* builder=gtk_builder_new();
    gtk_builder_add_from_file(builder,"./ui/window.ui",NULL);

    //获取并添加主界面
    GObject* root=gtk_builder_get_object(builder,"root");
    gtk_container_add(GTK_CONTAINER(window->source),GTK_WIDGET(root));

    //窗口按钮
    GObject* leftButton=gtk_builder_get_object(builder,"leftButton");
    GObject* middleButton=gtk_builder_get_object(builder,"middleButton");
    GObject* rightButton=gtk_builder_get_object(builder,"rightButton");

    //窗口标题
    GObject* titleBox=gtk_builder_get_object(builder,"titleBox");

    //标题面板
    GObject* titleBar=gtk_builder_get_object(builder,"titleBar");

    //窗口按钮事件
    g_signal_connect(leftButton,"enter-notify-event",G_CALLBACK(mouse_entry_button),NULL);
    g_signal_connect(leftButton,"leave-notify-event",G_CALLBACK(mouse_leave_button),NULL);
    g_signal_connect(leftButton,"button-press-event",G_CALLBACK(left_button),window->source);

    g_signal_connect(middleButton,"enter-notify-event",G_CALLBACK(mouse_entry_button),NULL);
    g_signal_connect(middleButton,"leave-notify-event",G_CALLBACK(mouse_leave_button),NULL);
    g_signal_connect(middleButton,"button-press-event",G_CALLBACK(middle_button),window->source);

    g_signal_connect(rightButton,"enter-notify-event",G_CALLBACK(mouse_entry_button),rightButton);
    g_signal_connect(rightButton,"leave-notify-event",G_CALLBACK(mouse_leave_button),rightButton);
    g_signal_connect(rightButton,"button-press-event",G_CALLBACK(right_button),NULL);

    //窗口最大化事件
    g_signal_connect(window->source,"window-state-event",G_CALLBACK(window_event),middleButton);

    //窗口按钮动画
    g_signal_connect(titleBox,"enter-notify-event",G_CALLBACK(mouse_entry_button),NULL);
    g_signal_connect(titleBox,"leave-notify-event",G_CALLBACK(mouse_leave_button),NULL);
    g_signal_connect(titleBox,"button-press-event",G_CALLBACK(title_button),NULL);
    
    //窗口调整事件
    g_signal_connect(root,"button-press-event",G_CALLBACK(window_resize),window->source);

    //窗口移动事件
    g_signal_connect(titleBar,"button-press-event",G_CALLBACK(window_move),window->source);

    //加载CSS文件
    GtkCssProvider* provider=gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider,"./ui/theme.css",NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);

    //初始化窗口结构体内容
    window->title=GTK_WIDGET(gtk_container_get_children(GTK_CONTAINER(titleBox))->data);
    window->subtitle=GTK_WIDGET(gtk_builder_get_object(builder,"subtitle"));

    gtk_widget_set_app_paintable(window->source,TRUE);

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

        gtk_widget_set_app_paintable(widget,FALSE);

    //窗口窗口化
    }else if(event->new_window_state==87168){
        gtk_image_set_from_file(GTK_IMAGE(gtk_container_get_children(GTK_CONTAINER(data))->data),"./res/icon/maximize.svg");

        gtk_widget_set_app_paintable(widget,TRUE);

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
