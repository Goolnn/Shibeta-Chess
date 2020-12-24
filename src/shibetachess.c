#include <gtk/gtk.h>

#include "window/window.h"

void client_activate(GtkApplication*,gpointer);

int main(int argc,char* argv[]){
    GtkApplication* application=gtk_application_new("com.goolnn.shibetachess",G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application,"activate",G_CALLBACK(client_activate),NULL);
    int status=g_application_run(G_APPLICATION(application),argc,argv);
    g_object_unref(application);

}

void client_activate(GtkApplication* application,gpointer data){
    Window* window=shibetachess_window_new(application);

    shibetachess_window_set_title(window,"助屋棋");
    shibetachess_window_show(window);

}

// #include <gtk/gtk.h>
     
// void show_image(GtkWidget *widget,cairo_t *cr,cairo_surface_t *img)
// {
//   double xc = 128.0;
// double yc = 128.0;
// double radius = 100.0;
// double angle1 = 45.0  * (G_PI/180.0);  /* angles are specified */
// double angle2 = 180.0 * (G_PI/180.0);  /* in radians           */

// cairo_set_line_width (cr, 10.0);
// cairo_arc (cr, xc, yc, radius, angle1, angle2);
// cairo_stroke (cr);

// /* draw helping lines */
// cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
// cairo_set_line_width (cr, 6.0);

// cairo_arc (cr, xc, yc, 10.0, 0, 2*G_PI);
// cairo_fill (cr);

// cairo_arc (cr, xc, yc, radius, angle1, angle1);
// cairo_line_to (cr, xc, yc);
// cairo_arc (cr, xc, yc, radius, angle2, angle2);
// cairo_line_to (cr, xc, yc);
// cairo_stroke (cr);
//     // cairo_region_t *mask;
    
//     // cairo_set_source_surface(cr,img,0,0);
//     // cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE); 
//     // cairo_paint(cr);
    
//     // mask=gdk_cairo_region_create_from_surface(img);

//     // gdk_window_shape_combine_region(gtk_widget_get_window(widget) ,mask,0,0);
//     // gtk_widget_shape_combine_region(widget,mask);
    
//     // cairo_region_destroy(mask);
// }
    
// int main(int argc,char **argv)
// {
//     GtkWidget *win;
//     cairo_surface_t *img;

// //     GdkScreen *screen;
// //    GdkVisual *colormap;
    
//     gtk_init(&argc,&argv);
    
//     win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
//     gtk_window_set_decorated(GTK_WINDOW(win),FALSE);
//     gtk_widget_set_app_paintable(win,TRUE);
//     gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
    
//     // img=cairo_image_surface_create_from_png("./res/1.png");
//     // gtk_widget_set_size_request(win,
//     // cairo_image_surface_get_width(img),
//     // cairo_image_surface_get_height(img));

//     gtk_widget_set_size_request(win,
//     1280,
//     720);
//     g_signal_connect(G_OBJECT(win),"draw",
//     G_CALLBACK(show_image),NULL);

//     GdkScreen *screen = gtk_widget_get_screen (win);
//   GdkVisual *visual = gdk_screen_get_rgba_visual (screen);

//   if (visual == NULL)
//     visual = gdk_screen_get_system_visual (screen);

//   gtk_widget_set_visual (win, visual);


    
//     gtk_widget_show_all(win);
     
//     gtk_main();
//     cairo_surface_destroy(img);
    
//     return 0;
// }