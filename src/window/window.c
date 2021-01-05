#include <gtk/gtk.h>

#include "../data/user_data.h"

#include "window.h"

void clear_window_interface(GtkWidget*);

gboolean name_input(GtkButton*,gpointer);

GtkWidget* shibeta_chess_window_new(GtkApplication* application){
    //实例化窗口
    GtkWidget* window=gtk_application_window_new(application);

    //设置窗口标题
    gtk_window_set_title(GTK_WINDOW(window),"助屋棋");
    gtk_window_set_default_size(GTK_WINDOW(window),800,600);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    return window;

}

void shibeta_chess_window_set_interface(GtkWidget* window,InterfaceName name){
    //清空窗口界面
    clear_window_interface(window);

    if(name==INTERFACE_NAME_INPUT){
        GtkBuilder* builder=gtk_builder_new_from_file("./ui/NameInput.ui");
        GObject* root=gtk_builder_get_object(builder,"root");

        gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(root));

        GObject* name_input=gtk_builder_get_object(builder,"name-input");
        GObject* accept=gtk_builder_get_object(builder,"accept");

        g_signal_connect(accept,"clicked",G_CALLBACK(name_input),builder);

    }

}

/*
 * 清空窗口界面
 * 清空窗口的界面布局
 */
void clear_window_interface(GtkWidget* window){
    GList* list=gtk_container_get_children(GTK_CONTAINER(window));

    if(list!=NULL){
        void* data=list->data;

        while(true){
            if(data!=NULL){
                gtk_container_remove(GTK_CONTAINER(window),GTK_WIDGET(data));

            }else{
                break;

            }

            data=list->next;

        }

    }

}

gboolean name_input(GtkButton* button,gpointer data){
    GtkBuilder* builder=GTK_BUILDER(data);

    GObject* name_input=gtk_builder_get_object(builder,"name-input");
    GObject* warning=gtk_builder_get_object(builder,"warning");

    const char* get_name=gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(name_input)));

    if(strlen(get_name)<=255){
        if(strlen(get_name)>0){
            gtk_label_set_text(GTK_LABEL(warning),"");
            
            shibeta_chess_user_data_create();

            UserData user_data;
            user_data.player_name=calloc(1,strlen(get_name)+1);
            memcpy(user_data.player_name,get_name,strlen(get_name));
            
            shibeta_chess_user_data_save(user_data);

        }else{
            gtk_label_set_text(GTK_LABEL(warning),"至少打个空格什么的作为名字吧……-_-");

        }

    }else{
        char buffer[128];
        sprintf(buffer,"请将名字长度控制在255字节以下，当前占用%d字节！",strlen(get_name));

        gtk_label_set_text(GTK_LABEL(warning),buffer);

    }


    return FALSE;

}
