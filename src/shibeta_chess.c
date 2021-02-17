#include <sys/time.h>

#include <gtk/gtk.h>

#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#if WIN32
    #include <windows.h>

#endif

#include "tool/linked_list.h"

struct Animation;

typedef struct Animation Animation;

struct Animation{
    bool start;

    double animation_time;

    long start_time;
    long end_time;

    bool forward;

    double process;

};

//函数声明
void window_activate(GtkApplication*,gpointer);

gboolean window_clicked(GtkWidget*,GdkEventButton*,gpointer);
gboolean window_draw(GtkWidget*,cairo_t*,gpointer);

gboolean window_mouse_motion(GtkWidget*,GdkEventMotion*,gpointer);
gboolean window_mouse_leave(GtkWidget*,GdkEventButton*,gpointer);

gboolean entry_clicked(GtkWidget*,GdkEventButton*,gpointer);

void shibetachess_window_set_subtext(const char*);

void shibetachess_cairo_arc_rectangle(cairo_t*,double,double,double,double,double);

void animation_thread();

//DEBUG模式
const bool DEBUG=TRUE;

//窗口参数
const char* WINDOW_TITLE="助屋棋";                                                          //窗口标题

const float WINDOW_SHADOW_SIZE=20;                                                          //窗口阴影大小
const float WINDOW_CORNOR_SIZE=10;                                                          //窗口圆角大小

const float WINDOW_TITLEBAR_SIZE=36;                                                        //窗口标题栏大小

//棋盘参数
const float CHESSBOARD_PADDING=50;                                                          //棋盘边界大小

const float PIECE_PADDING=8;                                                                //棋子边界
const float PIECE_SIZE=64;                                                                  //棋子边界

//窗口阴影参数
const double BORDER_SHADOW_OPACITY=0.0625;                                                  //边缘阴影浓度
const double CORNOR_SHADOW_OPACITY=0.075;                                                   //边角阴影浓度

//动画参数
LinkedList* animation_list;                                                                 //动画列表

const float ANIMATION_TIME=0.25;                                                            //动画时长

Animation menu_button_animation;                                                            //菜单按钮动画
Animation iconify_button_animation;                                                         //最小化按钮动画
Animation close_button_animation;                                                           //关闭按钮动画

Animation* ANIMATIONS[3+6*8]={&menu_button_animation,&iconify_button_animation,&close_button_animation};

//标记
bool menu_button_entry;
bool iconify_button_entry;
bool close_button_entry;

//计算参数
const float CELL_SIZE=PIECE_SIZE+PIECE_PADDING*2;                                           //棋格大小

const float WINDOW_OFFSET_X=WINDOW_SHADOW_SIZE;                                             //窗口偏移X
const float WINDOW_OFFSET_Y=WINDOW_SHADOW_SIZE;                                             //窗口偏移Y

const float CANVAS_OFFSET_X=WINDOW_OFFSET_X;                                                //画布偏移X
const float CANVAS_OFFSET_Y=WINDOW_OFFSET_Y+WINDOW_TITLEBAR_SIZE;                           //画布偏移Y

const float CHESSBOARD_OFFSET_X=CANVAS_OFFSET_X+CHESSBOARD_PADDING;                         //棋盘偏移X
const float CHESSBOARD_OFFSET_Y=CANVAS_OFFSET_Y+CHESSBOARD_PADDING;                         //棋盘偏移Y

const float WINDOW_ICON_SIZE=8*(WINDOW_TITLEBAR_SIZE/32);                                   //窗口图标大小

const float ALL_WINDOW_WIDTH=CHESSBOARD_PADDING*2+CELL_SIZE*6+WINDOW_SHADOW_SIZE*2;                                 //全窗口宽度
const float ALL_WINDOW_HEIGHT=CHESSBOARD_PADDING*2+CELL_SIZE*8+WINDOW_TITLEBAR_SIZE*2+WINDOW_SHADOW_SIZE*2;         //全窗口高度

const float WINDOW_WIDTH=CHESSBOARD_PADDING*2+CELL_SIZE*6;                                  //窗口宽度
const float WINDOW_HEIGHT=CHESSBOARD_PADDING*2+CELL_SIZE*8+WINDOW_TITLEBAR_SIZE*2;          //窗口高度

const float CANVAS_WIDTH=CHESSBOARD_PADDING*2+CELL_SIZE*6;                                  //画布宽度
const float CANVAS_HEIGHT=CHESSBOARD_PADDING*2+CELL_SIZE*8;                                 //画布高度

//游戏配置文件参数
const char* USERDATA_PATH="./res/data/Userdata";
const char* SERVERLIST_PATH="./res/data/Serverlist";

//Windows平台下窗口移动所需参数
bool pressed=false;

double pressed_x;
double pressed_y;

int pressed_window_x;
int pressed_window_y;

//线程锁
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//程序控件
GtkWidget* window;
GtkWidget* pane;
GtkWidget* menu;

char* subtext;

bool mouse_leave=true;

double mouse_x;
double mouse_y;

int chessboard_x=-1;
int chessboard_y=-1;

int chessboard_focus_x=-1;
int chessboard_focus_y=-1;

bool can_close_menu=true;
bool menu_open=false;
bool choose_menu=false;

bool game_exit=false;

int main(int argc,char* argv[]){
    //初始化动画列表
    animation_list=linked_list_new(0,NULL);

    //动画参数初始化
    menu_button_animation.animation_time=0.25;
    iconify_button_animation.animation_time=0.25;
    close_button_animation.animation_time=0.25;

    //动画参数初始化
    for(int y=0;y<8;y++){
        for(int x=0;x<6;x++){
            Animation* piece_animation=calloc(1,sizeof(Animation));

            piece_animation->animation_time=0.25;

            ANIMATIONS[3+(y*6+x)]=piece_animation;

        }

    }

    //启动游戏窗口
    GtkApplication* application=gtk_application_new("com.goolnn.shibetachess",G_APPLICATION_FLAGS_NONE);

    g_signal_connect(application,"activate",G_CALLBACK(window_activate),NULL);

    int status=g_application_run(G_APPLICATION(application),0,NULL);

    g_object_unref(application);

    return status;

}

long now_time(){
    struct timeval time;

    gettimeofday(&time,NULL);

    return time.tv_sec*1000000+time.tv_usec;

}

void window_activate(GtkApplication* application,gpointer data){
    //仅Linux系统可以启动该DEBUG功能
    //在Windows系统上显示该窗口时，该窗口的界面样式竟然会受到游戏窗口加载的CSS界面样式给修改。无语……
    #if linux
        gtk_window_set_interactive_debugging(DEBUG);

    #endif

    window=gtk_application_window_new(application);

    //设置游戏窗口参数
    gtk_window_set_default_size(GTK_WINDOW(window),ALL_WINDOW_WIDTH,ALL_WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    gtk_window_set_decorated(GTK_WINDOW(window),FALSE);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window),WINDOW_TITLE);

    //设置游戏窗口可绘制
    gtk_widget_set_app_paintable(window,TRUE);

    //初始化窗口透明
    GdkScreen* screen=gtk_widget_get_screen(window);
    GdkVisual* visual=gdk_screen_get_rgba_visual(screen);

    if(visual==NULL){
        visual=gdk_screen_get_system_visual(screen);

    }

    gtk_widget_set_visual(window,visual);

    //添加窗口面板
    pane=gtk_fixed_new();

    gtk_container_add(GTK_CONTAINER(window),pane);

    //添加游戏窗口事件
    gtk_widget_add_events(window,GDK_POINTER_MOTION_MASK);

    g_signal_connect(window,"button-press-event",G_CALLBACK(window_clicked),window);
    g_signal_connect(window,"button-release-event",G_CALLBACK(window_clicked),window);

    g_signal_connect(window,"motion-notify-event",G_CALLBACK(window_mouse_motion),window);
    g_signal_connect(window,"leave-notify-event",G_CALLBACK(window_mouse_leave),window);

    g_signal_connect(window,"draw",G_CALLBACK(window_draw),NULL);

    //加载CSS文件
    GtkCssProvider* provider=gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider,"./res/css/Theme.css",NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);

    //判断玩家是否第一次游玩
    if(access(USERDATA_PATH,F_OK)==-1){
        //创建名字输入界面
        menu=gtk_viewport_new(NULL,NULL);

        gtk_viewport_set_shadow_type(GTK_VIEWPORT(menu),GTK_SHADOW_NONE);

        GtkWidget* name_input=gtk_grid_new();

        gtk_widget_set_halign(name_input,GTK_ALIGN_CENTER);
        gtk_widget_set_valign(name_input,GTK_ALIGN_CENTER);

        GtkWidget* label=gtk_label_new("初次见面，请问你叫什么呢？");

        gtk_widget_set_margin_bottom(label,5);

        GtkWidget* name_field=gtk_entry_new();

        gtk_widget_set_size_request(name_field,250,-1);

        g_signal_connect(name_field,"button-press-event",G_CALLBACK(entry_clicked),NULL);

        gtk_grid_attach(GTK_GRID(name_input),label,0,0,1,1);
        gtk_grid_attach(GTK_GRID(name_input),name_field,0,1,1,1);

        gtk_container_add(GTK_CONTAINER(menu),name_input);

        gtk_widget_set_size_request(menu,WINDOW_WIDTH,CANVAS_HEIGHT);

        gtk_fixed_put(GTK_FIXED(pane),menu,WINDOW_OFFSET_X,CANVAS_OFFSET_Y);

        menu_open=true;
        // can_close_menu=false;

    }

    // //名字输入
    // menu=gtk_grid_new();

    // GtkWidget* label=gtk_label_new("初次见面，请问你叫什么呢？");

    // gtk_widget_set_hexpand(label,TRUE);
    // gtk_widget_set_vexpand(label,TRUE);

    // gtk_widget_set_halign(label,GTK_ALIGN_CENTER);
    // gtk_widget_set_valign(label,GTK_ALIGN_END);

    // GtkWidget* name_field=gtk_entry_new();

    // gtk_widget_set_hexpand(name_field,TRUE);
    // gtk_widget_set_vexpand(name_field,TRUE);

    // gtk_widget_set_halign(name_field,GTK_ALIGN_CENTER);
    // gtk_widget_set_valign(name_field,GTK_ALIGN_START);

    // gtk_widget_set_size_request(name_field,250,-1);

    // g_signal_connect(name_field,"button-press-event",G_CALLBACK(entry_clicked),NULL);

    // gtk_grid_attach(GTK_GRID(menu),label,0,0,1,1);
    // gtk_grid_attach(GTK_GRID(menu),name_field,0,1,1,1);

    // gtk_widget_set_size_request(menu,ALL_WINDOW_WIDTH,ALL_WINDOW_HEIGHT-WINDOW_TITLEBAR_SIZE*2);

    // gtk_fixed_put(GTK_FIXED(pane),menu,0,WINDOW_TITLEBAR_SIZE);

    //启动动画线程
    pthread_t animation;
    pthread_create(&animation,NULL,(void*)animation_thread,NULL);
    pthread_detach(animation);

    //显示窗口
    gtk_widget_show_all(window);

}

gboolean window_draw(GtkWidget* widget,cairo_t* cr,gpointer data){
    //绘制窗口边缘阴影
    cairo_pattern_t* pattern;

    //上边缘阴影
    pattern=cairo_pattern_create_linear(ALL_WINDOW_WIDTH/2,WINDOW_SHADOW_SIZE,ALL_WINDOW_WIDTH/2,0);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,BORDER_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_rectangle(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,0,ALL_WINDOW_WIDTH-WINDOW_SHADOW_SIZE*2-WINDOW_CORNOR_SIZE*2,WINDOW_SHADOW_SIZE);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //下边缘阴影
    pattern=cairo_pattern_create_linear(ALL_WINDOW_WIDTH/2,ALL_WINDOW_HEIGHT-WINDOW_SHADOW_SIZE,ALL_WINDOW_WIDTH/2,ALL_WINDOW_HEIGHT);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,BORDER_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_rectangle(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,ALL_WINDOW_HEIGHT-WINDOW_SHADOW_SIZE,ALL_WINDOW_WIDTH-WINDOW_SHADOW_SIZE*2-WINDOW_CORNOR_SIZE*2,WINDOW_SHADOW_SIZE);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //左边缘阴影
    pattern=cairo_pattern_create_linear(WINDOW_SHADOW_SIZE,ALL_WINDOW_HEIGHT/2,0,ALL_WINDOW_HEIGHT/2);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,BORDER_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_rectangle(cr,0,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE,ALL_WINDOW_HEIGHT-WINDOW_SHADOW_SIZE*2-WINDOW_CORNOR_SIZE*2);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //右边缘阴影候
    pattern=cairo_pattern_create_linear(ALL_WINDOW_WIDTH-WINDOW_SHADOW_SIZE,ALL_WINDOW_HEIGHT/2,ALL_WINDOW_WIDTH,ALL_WINDOW_HEIGHT/2);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,BORDER_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_rectangle(cr,ALL_WINDOW_WIDTH-WINDOW_SHADOW_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE,ALL_WINDOW_HEIGHT-WINDOW_SHADOW_SIZE*2-WINDOW_CORNOR_SIZE*2);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //左上角阴影
    pattern=cairo_pattern_create_radial(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,0,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,CORNOR_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_arc(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,G_PI,G_PI/2*3);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_move_to(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);
    cairo_rel_line_to(cr,0,-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE));
    cairo_rel_line_to(cr,-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);

    cairo_close_path(cr);

    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //右上角阴影
    pattern=cairo_pattern_create_radial(ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,0,ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,CORNOR_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_arc(cr,ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,G_PI/2*3,G_PI*2);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_move_to(cr,ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);
    cairo_rel_line_to(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,0);
    cairo_rel_line_to(cr,-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE));

    cairo_close_path(cr);

    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //左下角阴影
    pattern=cairo_pattern_create_radial(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),0,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,CORNOR_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_arc(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,G_PI/2,G_PI);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_move_to(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE));
    cairo_rel_line_to(cr,-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),0);
    cairo_rel_line_to(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);

    cairo_close_path(cr);

    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //右下角阴影
    pattern=cairo_pattern_create_radial(ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),0,ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);

    cairo_pattern_add_color_stop_rgba(pattern,0,0,0,0,CORNOR_SHADOW_OPACITY);
    cairo_pattern_add_color_stop_rgba(pattern,1,0,0,0,0);

    cairo_arc(cr,ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,0,G_PI/2);
    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_move_to(cr,ALL_WINDOW_WIDTH-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),ALL_WINDOW_HEIGHT-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE));
    cairo_rel_line_to(cr,WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE,0);
    cairo_rel_line_to(cr,-(WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE),WINDOW_SHADOW_SIZE+WINDOW_CORNOR_SIZE);

    cairo_close_path(cr);

    cairo_set_source(cr,pattern);
    cairo_fill(cr);

    cairo_pattern_destroy(pattern);
    pattern=NULL;

    //窗口点击限制蒙板
    //仅在Linux系统下有作用。在Windows下不仅没有作用，还会产生奇怪的BUG
    #if linux
        cairo_surface_t* surface=gdk_window_create_similar_surface(gtk_widget_get_window(widget),CAIRO_CONTENT_COLOR_ALPHA,ALL_WINDOW_WIDTH,ALL_WINDOW_HEIGHT);
        cairo_t* surface_cr=cairo_create(surface);

        cairo_set_source_rgb(surface_cr,1,1,1);

        shibetachess_cairo_arc_rectangle(surface_cr,WINDOW_OFFSET_X,WINDOW_OFFSET_Y,WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_CORNOR_SIZE);
        cairo_fill(surface_cr);

        cairo_region_t* mask=gdk_cairo_region_create_from_surface(surface);

        gdk_window_input_shape_combine_region(gtk_widget_get_window(widget),mask,0,0);

        cairo_surface_destroy(surface);
        cairo_destroy(surface_cr);
        cairo_region_destroy(mask);

        surface=NULL;
        surface_cr=NULL;
        mask=NULL;

    #endif

    //圆角窗口蒙板
    cairo_set_source_rgb(cr,1,1,1);

    shibetachess_cairo_arc_rectangle(cr,WINDOW_OFFSET_X,WINDOW_OFFSET_Y,WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_CORNOR_SIZE);

    cairo_clip(cr);
    cairo_new_path(cr);

    //绘制窗口
    cairo_set_source_rgb(cr,(double)32/255,(double)32/255,(double)32/255);

    cairo_rectangle(cr,WINDOW_OFFSET_X,WINDOW_OFFSET_Y,WINDOW_WIDTH,WINDOW_TITLEBAR_SIZE);
    cairo_fill(cr);
    cairo_rectangle(cr,WINDOW_OFFSET_X,WINDOW_OFFSET_Y+WINDOW_HEIGHT-WINDOW_TITLEBAR_SIZE,WINDOW_WIDTH,WINDOW_TITLEBAR_SIZE);
    cairo_fill(cr);

    cairo_set_source_rgb(cr,1,1,1);

    cairo_rectangle(cr,WINDOW_OFFSET_X,WINDOW_OFFSET_Y+WINDOW_TITLEBAR_SIZE,WINDOW_WIDTH,WINDOW_HEIGHT-WINDOW_TITLEBAR_SIZE*2);
    cairo_fill(cr);

    //窗口标题
    cairo_set_source_rgb(cr,1,1,1);

    cairo_text_extents_t extents;

    double x;
    double y;

    x=WINDOW_OFFSET_X+WINDOW_WIDTH/2;
    y=WINDOW_OFFSET_Y+WINDOW_TITLEBAR_SIZE/2;

    cairo_select_font_face(cr,"黑体",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,12*(WINDOW_TITLEBAR_SIZE/30));
    cairo_text_extents(cr,WINDOW_TITLE,&extents);

    x=x-(extents.width/2+extents.x_bearing);
    y=y-(extents.height/2+extents.y_bearing);

    cairo_move_to(cr,x,y);
    cairo_show_text(cr,WINDOW_TITLE);

    //绘制菜单按钮
    if(can_close_menu){
        cairo_set_source_rgba(cr,0,0,0,0.5*menu_button_animation.process);

        cairo_rectangle(cr,WINDOW_OFFSET_X,WINDOW_OFFSET_Y,WINDOW_TITLEBAR_SIZE,WINDOW_TITLEBAR_SIZE);
        cairo_fill(cr);

    }

    if(can_close_menu){
        cairo_set_source_rgb(cr,1,1,1);

    }else{
        cairo_set_source_rgb(cr,0.5,0.5,0.5);

    }

    cairo_set_line_width(cr,1.5);

    cairo_move_to(cr,WINDOW_OFFSET_X+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2-1,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_line_to(cr,WINDOW_OFFSET_Y+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2+1,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_stroke(cr);

    cairo_move_to(cr,WINDOW_OFFSET_X+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2-1,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2+WINDOW_ICON_SIZE/2);
    cairo_line_to(cr,WINDOW_OFFSET_Y+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2+1,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2+WINDOW_ICON_SIZE/2);
    cairo_stroke(cr);

    cairo_move_to(cr,WINDOW_OFFSET_X+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2-1,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2+WINDOW_ICON_SIZE);
    cairo_line_to(cr,WINDOW_OFFSET_Y+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2+1,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2+WINDOW_ICON_SIZE);
    cairo_stroke(cr);

    //绘制最小化按钮
    cairo_set_source_rgba(cr,0,0,0,0.5*iconify_button_animation.process);

    cairo_rectangle(cr,WINDOW_SHADOW_SIZE+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE*2,WINDOW_SHADOW_SIZE,WINDOW_TITLEBAR_SIZE,WINDOW_TITLEBAR_SIZE);
    cairo_fill(cr);

    cairo_set_source_rgb(cr,1,1,1);

    cairo_set_line_width(cr,1.5);

    cairo_move_to(cr,WINDOW_OFFSET_X+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE*2+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2,WINDOW_SHADOW_SIZE+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_line_to(cr,WINDOW_OFFSET_Y+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE*2+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2,WINDOW_SHADOW_SIZE+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_stroke(cr);

    //绘制关闭按钮
    cairo_set_source_rgba(cr,0,0,0,0.5*close_button_animation.process);

    cairo_rectangle(cr,WINDOW_OFFSET_X+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE,WINDOW_OFFSET_Y,WINDOW_TITLEBAR_SIZE,WINDOW_TITLEBAR_SIZE);
    cairo_fill(cr);

    cairo_set_source_rgb(cr,1,1,1);

    cairo_set_line_width(cr,1.5);

    cairo_move_to(cr,WINDOW_OFFSET_X+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_line_to(cr,WINDOW_OFFSET_Y+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2,WINDOW_SHADOW_SIZE+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_stroke(cr);

    cairo_move_to(cr,WINDOW_OFFSET_X+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2,WINDOW_SHADOW_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_line_to(cr,WINDOW_OFFSET_Y+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE+(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2,WINDOW_SHADOW_SIZE+WINDOW_TITLEBAR_SIZE-(WINDOW_TITLEBAR_SIZE-WINDOW_ICON_SIZE)/2);
    cairo_stroke(cr);

    //绘制副文本
    cairo_set_source_rgb(cr,1,1,1);

    x=WINDOW_OFFSET_X+WINDOW_WIDTH/2;
    y=WINDOW_OFFSET_Y+WINDOW_HEIGHT-WINDOW_TITLEBAR_SIZE+WINDOW_TITLEBAR_SIZE/2;

    cairo_select_font_face(cr,"黑体",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,12*(WINDOW_TITLEBAR_SIZE/30));
    cairo_text_extents(cr,subtext,&extents);

    x=x-(extents.width/2+extents.x_bearing);
    y=y-(extents.height/2+extents.y_bearing);

    cairo_move_to(cr,x,y);
    cairo_show_text(cr,subtext);

    //高亮鼠标棋格
    if(!menu_open&&!choose_menu){
        if(!(chessboard_x==chessboard_focus_x&&chessboard_y==chessboard_focus_y)&&(chessboard_x!=-1&&chessboard_y!=-1)){
            if(!(((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==0)&&((chessboard_x==2||chessboard_x==3)&&chessboard_y==0))&&!(((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==7)&&((chessboard_x==2||chessboard_x==3)&&chessboard_y==7))){
                cairo_set_source_rgba(cr,0,0,0,0.125);

                if((chessboard_x==2||chessboard_x==3)&&chessboard_y==0){
                    cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*2,CHESSBOARD_OFFSET_Y,CELL_SIZE*2,CELL_SIZE);

                }else if((chessboard_x==2||chessboard_x==3)&&chessboard_y==7){
                    cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*2,CHESSBOARD_OFFSET_Y+CELL_SIZE*7,CELL_SIZE*2,CELL_SIZE);

                }else{
                    cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*chessboard_x,CHESSBOARD_OFFSET_Y+CELL_SIZE*chessboard_y,CELL_SIZE,CELL_SIZE);

                }

                cairo_fill(cr);

            }

        }

    }

    //高亮焦点棋格
    if(chessboard_focus_x!=-1&&chessboard_focus_y!=-1){
        cairo_set_source_rgba(cr,0,0,0,0.25);

        if((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==0){
            cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*2,CHESSBOARD_OFFSET_Y,CELL_SIZE*2,CELL_SIZE);

        }else if((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==7){
            cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*2,CHESSBOARD_OFFSET_Y+CELL_SIZE*7,CELL_SIZE*2,CELL_SIZE);

        }else{
            cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*chessboard_focus_x,CHESSBOARD_OFFSET_Y+CELL_SIZE*chessboard_focus_y,CELL_SIZE,CELL_SIZE);

        }

        cairo_fill(cr);

    }

    //高亮敌组攻击范围
    if((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==0){
        cairo_set_source_rgba(cr,0,0,0,0.125);

        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*1,CHESSBOARD_OFFSET_Y,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);
        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*4,CHESSBOARD_OFFSET_Y,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);

        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*2,CHESSBOARD_OFFSET_Y+CELL_SIZE,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);
        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*3,CHESSBOARD_OFFSET_Y+CELL_SIZE,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);

    }

    //高亮助屋攻击范围
    if((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==7){
        cairo_set_source_rgba(cr,0,0,0,0.125);

        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*1,CHESSBOARD_OFFSET_Y+CELL_SIZE*7,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);
        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*4,CHESSBOARD_OFFSET_Y+CELL_SIZE*7,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);

        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*1,CHESSBOARD_OFFSET_Y+CELL_SIZE*6,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);
        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*2,CHESSBOARD_OFFSET_Y+CELL_SIZE*6,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);
        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*3,CHESSBOARD_OFFSET_Y+CELL_SIZE*6,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);
        cairo_rectangle(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*4,CHESSBOARD_OFFSET_Y+CELL_SIZE*6,CELL_SIZE,CELL_SIZE);
        cairo_fill(cr);

    }

    //绘制棋盘
    cairo_set_source_rgb(cr,0,0,0);

    for(int i=0;i<=8;i++){
        cairo_move_to(cr,CHESSBOARD_OFFSET_X,CHESSBOARD_OFFSET_Y+CELL_SIZE*i);
        cairo_line_to(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*6,CHESSBOARD_OFFSET_Y+CELL_SIZE*i);

        cairo_stroke(cr);

    }

    for(int i=0;i<=6;i++){
        if(i!=3){
            cairo_move_to(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*i,CHESSBOARD_OFFSET_Y);
            cairo_line_to(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*i,CHESSBOARD_OFFSET_Y+CELL_SIZE*8);

            cairo_stroke(cr);

        }else{
            cairo_move_to(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*i,CHESSBOARD_OFFSET_Y+CELL_SIZE);
            cairo_line_to(cr,CHESSBOARD_OFFSET_X+CELL_SIZE*i,CHESSBOARD_OFFSET_Y+CELL_SIZE*7);

            cairo_stroke(cr);

        }

    }

    //绘制敌组文字
    const char* DIZU_TEXT="敌组";
    const char* ZHUWU_TEXT="助屋";

    x=CHESSBOARD_OFFSET_X+CELL_SIZE*3;
    y=CHESSBOARD_OFFSET_Y+CELL_SIZE/2;

    cairo_select_font_face(cr,"黑体",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,32*(CELL_SIZE/75));
    cairo_text_extents(cr,DIZU_TEXT,&extents);

    x=x-(extents.width/2+extents.x_bearing);
    y=y-(extents.height/2+extents.y_bearing);

    cairo_move_to(cr,x,y);
    cairo_show_text(cr,DIZU_TEXT);

    //绘制助屋文字
    x=CHESSBOARD_OFFSET_X+CELL_SIZE*3;
    y=CHESSBOARD_OFFSET_Y+CELL_SIZE/2+CELL_SIZE*7;

    cairo_select_font_face(cr,"黑体",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,32*(CELL_SIZE/75));
    cairo_text_extents(cr,ZHUWU_TEXT,&extents);

    x=x-(extents.width/2+extents.x_bearing);
    y=y-(extents.height/2+extents.y_bearing);

    cairo_move_to(cr,x,y);
    cairo_show_text(cr,ZHUWU_TEXT);

    //绘制选择菜单
    if(choose_menu){
        cairo_set_source_rgba(cr,0,0,0,0.25);

        shibetachess_cairo_arc_rectangle(cr,ALL_WINDOW_WIDTH/2-(CELL_SIZE*5)/2,ALL_WINDOW_HEIGHT/2-CELL_SIZE/2,CELL_SIZE*5,CELL_SIZE,WINDOW_CORNOR_SIZE);
        cairo_fill(cr);

    }

    //绘制菜单
    if(menu_open){
        cairo_set_source_rgba(cr,0,0,0,0.5);

        cairo_rectangle(cr,CANVAS_OFFSET_X,CANVAS_OFFSET_Y,CANVAS_WIDTH,CANVAS_HEIGHT);
        cairo_fill(cr);

    }

    //DEBUG信息
    if(DEBUG){
        cairo_set_source_rgb(cr,0,0,0);
        char text[32];

        cairo_select_font_face(cr,"黑体",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr,14*(CELL_SIZE/75));

        const double DEBUG_X=CANVAS_OFFSET_X+10;
        const double DEBUG_Y=CANVAS_OFFSET_Y+20;

        //绘制鼠标位置
        if(!mouse_leave&&mouse_x>0&&mouse_y>0){
            sprintf(text,"MOUSE X:%f",mouse_x);

            x=DEBUG_X;
            y=DEBUG_Y+20*0;

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

            sprintf(text,"MOUSE Y:%f",mouse_y);

            x=DEBUG_X;
            y=DEBUG_Y+20*1;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

        }else{
            sprintf(text,"MOUSE X:NaN");

            x=DEBUG_X;
            y=DEBUG_Y+20*0;

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

            sprintf(text,"MOUSE Y:NaN");

            x=DEBUG_X;
            y=DEBUG_Y+20*1;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

        }

        //绘制棋盘位置
        if(chessboard_x!=-1&&chessboard_y!=-1){
            sprintf(text,"CHESSBOARD X:%d",chessboard_x);

            x=DEBUG_X;
            y=DEBUG_Y+20*2;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

            sprintf(text,"CHESSBOARD Y:%d",chessboard_y);

            x=DEBUG_X;
            y=DEBUG_Y+20*3;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

        }else{
            sprintf(text,"CHESSBOARD X:NaN");

            x=DEBUG_X;
            y=DEBUG_Y+20*2;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

            sprintf(text,"CHESSBOARD Y:NaN");

            x=DEBUG_X;
            y=DEBUG_Y+20*3;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

        }

        //绘制焦点位置
        if(chessboard_focus_x!=-1&&chessboard_focus_y!=-1){
            sprintf(text,"CHESSBOARD FOCUS X:%d",chessboard_focus_x);

            x=DEBUG_X;
            y=DEBUG_Y+20*4;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

            sprintf(text,"CHESSBOARD FOCUS Y:%d",chessboard_focus_y);

            x=DEBUG_X;
            y=DEBUG_Y+20*5;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

        }else{
            sprintf(text,"CHESSBOARD FOCUS X:NaN");

            x=DEBUG_X;
            y=DEBUG_Y+20*4;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

            sprintf(text,"CHESSBOARD FOCUS Y:NaN");

            x=DEBUG_X;
            y=DEBUG_Y+20*5;

            cairo_text_extents(cr,text,&extents);

            cairo_move_to(cr,x,y);
            cairo_show_text(cr,text);

        }

        //绘制菜单开启信息
        if(menu_open){
            sprintf(text,"MENU OPEN:TRUE");

        }else{
            sprintf(text,"MENU OPEN:FALSE");

        }

        x=DEBUG_X;
        y=DEBUG_Y+20*6;

        cairo_text_extents(cr,text,&extents);

        cairo_move_to(cr,x,y);
        cairo_show_text(cr,text);

        if(choose_menu){
            sprintf(text,"CHOOSE MENU OPEN:TRUE");

        }else{
            sprintf(text,"CHOOSE MENU OPEN:FALSE");

        }

        x=DEBUG_X;
        y=DEBUG_Y+20*7;

        cairo_text_extents(cr,text,&extents);

        cairo_move_to(cr,x,y);
        cairo_show_text(cr,text);

    }

    return FALSE;

}

gboolean window_clicked(GtkWidget* widget,GdkEventButton* event,gpointer data){
    //窗口移动
    //Windows平台上也可以使用该方法移动窗口，但是当窗口移动到屏幕边缘时就会产生最大化的趋势，即便窗口大小为不可调整状态
    #if linux
        if(event->type==GDK_BUTTON_PRESS){
            if(mouse_x>WINDOW_TITLEBAR_SIZE&&mouse_x<WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE*2&&mouse_y>0&&mouse_y<WINDOW_TITLEBAR_SIZE){
                gtk_window_begin_move_drag(GTK_WINDOW(data),event->button,event->x_root,event->y_root,event->time);

            }

        }

    #endif

    #if WIN32
        if(event->x>WINDOW_SHADOW_SIZE+WINDOW_TITLEBAR_SIZE&&event->x<WINDOW_SHADOW_SIZE+WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE*2&&event->y>WINDOW_SHADOW_SIZE&&mouse_y<WINDOW_SHADOW_SIZE+WINDOW_TITLEBAR_SIZE){
            if(event->button==GDK_BUTTON_PRIMARY&&event->type==GDK_BUTTON_PRESS){
                pressed=true;

                pressed_x=event->x_root;
                pressed_y=event->y_root;

                mouse_x=-1;
                mouse_y=-1;

                gtk_window_get_position(GTK_WINDOW(window),&pressed_window_x,&pressed_window_y);
                gtk_widget_queue_draw(window);

                //限制鼠标移动区域
                //用于放置窗口被拖动到任务栏后面导致窗口被挡住
                RECT rect;
                SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
                ClipCursor(&rect);

            }else if(event->button==GDK_BUTTON_PRIMARY&&event->type==GDK_BUTTON_RELEASE){
                pressed=false;

                ClipCursor(NULL);

            }

        }

    #endif

    if(event->button==GDK_BUTTON_PRIMARY&&event->type==GDK_BUTTON_PRESS){
        //窗口菜单
        if(mouse_x>0&&mouse_x<WINDOW_TITLEBAR_SIZE&&mouse_y>0&&mouse_y<WINDOW_TITLEBAR_SIZE&&can_close_menu){
            menu_open=!menu_open;

            gtk_widget_set_child_visible(menu,menu_open);

            gtk_widget_queue_draw(window);

        }

        //窗口最小化
        if((mouse_x>WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE*2&&mouse_x<WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE)&&mouse_y<WINDOW_TITLEBAR_SIZE){
            gtk_window_iconify(GTK_WINDOW(data));

        }

        //窗口关闭
        if((mouse_x>WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE&&mouse_x<WINDOW_WIDTH)&&mouse_y<WINDOW_TITLEBAR_SIZE){
            gtk_window_close(GTK_WINDOW(data));

            game_exit=true;

        }

        if(!menu_open){
            int chessboard_x=(mouse_x-CHESSBOARD_PADDING)/CELL_SIZE;
            int chessboard_y=(mouse_y-WINDOW_TITLEBAR_SIZE-CHESSBOARD_PADDING)/CELL_SIZE;

            if((chessboard_x>=0&&chessboard_y>=0)&&(chessboard_x<=5&&chessboard_y<=7)&&(mouse_x-CHESSBOARD_PADDING>=0&&mouse_y-WINDOW_TITLEBAR_SIZE-CHESSBOARD_PADDING>=0)){
                if((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==0){
                    if((chessboard_x==1&&chessboard_y==0)||(chessboard_x==4&&chessboard_y==0)||(chessboard_x==2&&chessboard_y==1)||(chessboard_x==3&&chessboard_y==1)){
                        chessboard_focus_x=chessboard_x;
                        chessboard_focus_y=chessboard_y;

                        gtk_widget_queue_draw(window);

                        choose_menu=true;

                        gtk_widget_queue_draw(window);

                    }else{
                        chessboard_focus_x=chessboard_x;
                        chessboard_focus_y=chessboard_y;

                        gtk_widget_queue_draw(window);

                    }

                }else if((chessboard_focus_x==2||chessboard_focus_x==3)&&chessboard_focus_y==7){
                    if((chessboard_x==1&&chessboard_y==7)||(chessboard_x==4&&chessboard_y==7)||(chessboard_x==1&&chessboard_y==6)||(chessboard_x==2&&chessboard_y==6)||(chessboard_x==3&&chessboard_y==6)||(chessboard_x==4&&chessboard_y==6)){
                        chessboard_focus_x=chessboard_x;
                        chessboard_focus_y=chessboard_y;

                        gtk_widget_queue_draw(window);

                        choose_menu=true;

                        gtk_widget_queue_draw(window);

                    }else{
                        chessboard_focus_x=chessboard_x;
                        chessboard_focus_y=chessboard_y;

                        gtk_widget_queue_draw(window);

                    }

                }else if(!choose_menu){
                    chessboard_focus_x=chessboard_x;
                    chessboard_focus_y=chessboard_y;

                    gtk_widget_queue_draw(window);

                }

            }

        }

    }else if(event->button==GDK_BUTTON_SECONDARY&&event->type==GDK_BUTTON_PRESS){
        if(choose_menu&&!menu_open){
            choose_menu=false;

            chessboard_focus_x=-1;
            chessboard_focus_y=-1;

            gtk_widget_queue_draw(window);

        }else{
            if(!menu_open){
                chessboard_focus_x=-1;
                chessboard_focus_y=-1;

                gtk_widget_queue_draw(window);

            }

        }

    }

    return TRUE;

}

gboolean window_mouse_motion(GtkWidget* widget,GdkEventMotion* event,gpointer data){
    #if WIN32
        if(pressed){
            gtk_window_move(GTK_WINDOW(window),pressed_window_x+(event->x_root-pressed_x),pressed_window_y+(event->y_root-pressed_y));

        }

    #endif

    if(event->window==gtk_widget_get_window(widget)&&!pressed){
        int old_chessboard_x=chessboard_x;
        int old_chessboard_y=chessboard_y;

        mouse_x=event->x-WINDOW_SHADOW_SIZE;
        mouse_y=event->y-WINDOW_SHADOW_SIZE;

        int chessboard_x_temp=(mouse_x-CHESSBOARD_PADDING)/CELL_SIZE;
        int chessboard_y_temp=(mouse_y-WINDOW_TITLEBAR_SIZE-CHESSBOARD_PADDING)/CELL_SIZE;

        if((chessboard_x_temp>=0&&chessboard_y_temp>=0)&&(chessboard_x_temp<=5&&chessboard_y_temp<=7)&&(mouse_x-CHESSBOARD_PADDING>=0&&mouse_y-WINDOW_TITLEBAR_SIZE-CHESSBOARD_PADDING>=0)){
            chessboard_x=chessboard_x_temp;
            chessboard_y=chessboard_y_temp;

        }else{
            chessboard_x=-1;
            chessboard_y=-1;

        }

        if(chessboard_x!=old_chessboard_x||chessboard_y!=old_chessboard_y){
            if(old_chessboard_x!=-1&&old_chessboard_y!=-1){
                printf("淡出：(%d,%d)\n",old_chessboard_x,old_chessboard_y);

            }

            if(chessboard_x!=-1&&chessboard_y!=-1){
                printf("淡入：(%d,%d)\n",chessboard_x,chessboard_y);

            }

        }

        mouse_leave=false;

        gtk_widget_queue_draw(window);

    }else{
        chessboard_x=-1;
        chessboard_y=-1;

        mouse_x=-1;
        mouse_y=-1;

        gtk_widget_queue_draw(window);

    }

    //Windows平台下判定鼠标超出窗口范围的代码
    #if WIN32
        if(mouse_x>WINDOW_OFFSET_X+WINDOW_WIDTH||mouse_y>WINDOW_OFFSET_X+WINDOW_HEIGHT){
            mouse_x=-1;
            mouse_y=-1;

            gtk_widget_queue_draw(window);

        }

    #endif

    if((mouse_x>0&&mouse_x<WINDOW_TITLEBAR_SIZE&&mouse_y>0&&mouse_y<WINDOW_TITLEBAR_SIZE)){
        if(!menu_button_entry){
            menu_button_animation.start=true;
            menu_button_entry=true;

        }

    }else if(menu_button_entry){
        menu_button_animation.start=false;
        menu_button_entry=false;

    }

    if((mouse_x>WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE*2&&mouse_x<WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE)&&mouse_y<WINDOW_TITLEBAR_SIZE&&!mouse_leave){
        iconify_button_animation.start=true;

    }else{
        iconify_button_animation.start=false;

    }

    if((mouse_x>WINDOW_WIDTH-WINDOW_TITLEBAR_SIZE&&mouse_x<WINDOW_WIDTH)&&mouse_y<WINDOW_TITLEBAR_SIZE&&!mouse_leave){
        close_button_animation.start=true;

    }else{
        close_button_animation.start=false;

    }

    return FALSE;

}

gboolean window_mouse_leave(GtkWidget* widget,GdkEventButton* event,gpointer data){
    mouse_leave=true;

    menu_button_animation.start=false;
    iconify_button_animation.start=false;
    close_button_animation.start=false;

    mouse_x=-1;
    mouse_y=-1;

    chessboard_x=-1;
    chessboard_y=-1;

    gtk_widget_queue_draw(window);

    return FALSE;

}

gboolean entry_clicked(GtkWidget* widget,GdkEventButton* event,gpointer data){
    if(event->button==GDK_BUTTON_SECONDARY){
        return TRUE;

    }

    return FALSE;

}

void shibetachess_window_set_subtext(const char* text){
    free(subtext);

    subtext=calloc(1,sizeof(text));
    memcpy(subtext,text,sizeof(text));

    gtk_widget_queue_draw(window);

}

void shibetachess_cairo_arc_rectangle(cairo_t* cr,double x,double y,double width,double height,double radius){
    double degrees=G_PI/180;

    cairo_new_sub_path(cr);

    cairo_arc(cr,x+width-radius,y+radius,radius,-90*degrees,0*degrees);
    cairo_arc(cr,x+width-radius,y+height-radius,radius,0*degrees,90*degrees);
    cairo_arc(cr,x+radius,y+height-radius,radius,90*degrees,180*degrees);
    cairo_arc(cr,x+radius,y+radius,radius,180*degrees,270*degrees);

    cairo_close_path(cr);

}

void animation_thread(){
    struct timeval time;

    while(!game_exit){
        for(int i=0;i<3;i++){
            Animation* animation=ANIMATIONS[i];

            if(animation->start){
                if(animation->process==0){
                    animation->start_time=now_time();
                    animation->end_time=animation->start_time+animation->animation_time*1000000;

                }else if(animation->process<1&&!animation->forward){
                    long old_start_time=animation->start_time;
                    long old_end_time=animation->end_time;

                    animation->start_time=now_time()-(old_end_time-now_time());
                    animation->end_time=now_time()+(now_time()-old_start_time);

                }

                if(animation->end_time-now_time()>0){
                    animation->process=1-(double)(animation->end_time-now_time())/(animation->animation_time*1000000);

                }else{
                    animation->process=1;

                }

                animation->forward=true;

            }else{
                if(animation->process==1){
                    animation->start_time=now_time();
                    animation->end_time=animation->start_time+animation->animation_time*1000000;

                }else if(animation->process>0&&animation->forward){
                    long old_start_time=animation->start_time;
                    long old_end_time=animation->end_time;

                    animation->start_time=now_time()-(old_end_time-now_time());
                    animation->end_time=now_time()+(now_time()-old_start_time);

                }

                if(animation->end_time-now_time()>0){
                    animation->process=(double)(animation->end_time-now_time())/(animation->animation_time*1000000);

                }else{
                    animation->process=0;

                }

                animation->forward=false;

            }

        }

        //刷新窗口界面
        if(window!=NULL)gtk_widget_queue_draw(window);

        //线程礼让
        g_usleep(1);

    }

}
