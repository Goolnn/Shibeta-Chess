#ifndef SHIBETA_CHESS_WINDOW_WINDOW
#define SHIBETA_CHESS_WINDOW_WINDOW

#include <gtk/gtk.h>

typedef enum InterfaceName InterfaceName;

enum InterfaceName{
    INTERFACE_NAME_INPUT
    
};

//新建窗口
GtkWidget* shibeta_chess_window_new(GtkApplication*);

void shibeta_chess_window_set_interface(GtkWidget*,InterfaceName);

#endif
