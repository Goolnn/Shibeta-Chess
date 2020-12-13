#include <stdlib.h>

#include "Point.h"

Point* Point_NewInstance(float x, float y){
    Point* instance = (Point*)calloc(1, sizeof(Point));

    instance->x = x;
    instance->y = y;

    return instance;

}

void Point_SetX(Point* point, float x){
    point->x = x;

}

void Point_SetY(Point* point, float y){
    point->y = y;

}

float Point_GetX(Point* point){
    return point->x;

}

float Point_GetY(Point* point){
    return point->y;

}

void Point_Free(Point* point){
    free(point);

}
