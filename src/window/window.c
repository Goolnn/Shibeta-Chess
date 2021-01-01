#include <gtk/gtk.h>

#include "../data/user_data.h"

#include "window.h"

GtkWidget* shibeta_chess_window_new(GtkApplication* application){
    //实例化窗口
    GtkWidget* window=gtk_application_window_new(application);

    //设置窗口标题
    gtk_window_set_title(GTK_WINDOW(window),"助屋棋");
    gtk_window_set_default_size(GTK_WINDOW(window),640,480);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    return window;

}

gboolean name_accept(GtkButton* button,gpointer data){
    const char* name=gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(data)));

    printf("%s %d\n",name,strlen(name));

    shibeta_chess_user_data_write_player_name(name);

    return FALSE;

}

void shibeta_chess_window_set_interface(GtkWidget* window,InterfaceName name){
    if(name==INTERFACE_NAME_INPUT){
        GtkBuilder* builder=gtk_builder_new_from_file("./ui/NameInput.ui");
        GObject* root=gtk_builder_get_object(builder,"root");

        gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(root));

        GObject* name_input=gtk_builder_get_object(builder,"name-input");
        GObject* accept=gtk_builder_get_object(builder,"accept");

        g_signal_connect(accept,"clicked",G_CALLBACK(name_accept),name_input);

    }

}
