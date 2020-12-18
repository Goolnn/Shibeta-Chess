#include <gtk/gtk.h>

#include "MainThread.h"

void MainThread_Run(int* a){
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    static int i=0;

    while(1){
        printf("%d\n",i);

        i++;

        sleep(1);

    }

}