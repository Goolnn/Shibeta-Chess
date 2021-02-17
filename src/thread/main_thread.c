#include <sys/time.h>

#include <gtk/gtk.h>

#include <stdbool.h>

#include "main_thread.h"

void main_thread_run(){
    struct timeval time;

    while(true){
        printf("a\n");
        // for(int i=0;i<3;i++){
        //     Animation* animation=ANIMATIONS[i];

        //     if(animation->start){
        //         if(animation->process==0){
        //             animation->start_time=now_time();
        //             animation->end_time=animation->start_time+animation->animation_time*1000000;

        //         }else if(animation->process<1&&!animation->forward){
        //             long old_start_time=animation->start_time;
        //             long old_end_time=animation->end_time;

        //             animation->start_time=now_time()-(old_end_time-now_time());
        //             animation->end_time=now_time()+(now_time()-old_start_time);

        //         }

        //         if(animation->end_time-now_time()>0){
        //             animation->process=1-(double)(animation->end_time-now_time())/(animation->animation_time*1000000);

        //             if(window!=NULL)gtk_widget_queue_draw(window);

        //         }else{
        //             animation->process=1;

        //             if(window!=NULL)gtk_widget_queue_draw(window);

        //         }

        //         animation->forward=true;
            
        //     }else{
        //         if(animation->process==1){
        //             animation->start_time=now_time();
        //             animation->end_time=animation->start_time+animation->animation_time*1000000;

        //         }else if(animation->process>0&&animation->forward){
        //             long old_start_time=animation->start_time;
        //             long old_end_time=animation->end_time;

        //             animation->start_time=now_time()-(old_end_time-now_time());
        //             animation->end_time=now_time()+(now_time()-old_start_time);

        //         }

        //         if(animation->end_time-now_time()>0){
        //             animation->process=(double)(animation->end_time-now_time())/(animation->animation_time*1000000);

        //             if(window!=NULL)gtk_widget_queue_draw(window);

        //         }else{
        //             animation->process=0;

        //             if(window!=NULL)gtk_widget_queue_draw(window);

        //         }

        //         animation->forward=false;

        //     }

        // }

        // pthread_mutex_lock(&mutex);

        // linked_list_remove_head(animation_list);

        // pthread_mutex_unlock(&mutex);

        //线程礼让
        g_usleep(1);

    }

}
