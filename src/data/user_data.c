#include <unistd.h>
#include <stdio.h>

#include "user_data.h"

bool shibeta_chess_user_data_is_exist(){
    if(access(USER_DATA_PATH,F_OK)==0){
        return true;

    }

    return false;

}
