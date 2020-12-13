#ifndef SHIBETA_CHESS_POINT
#define SHIBETA_CHESS_POINT

typedef struct Point Point;

struct Point{
    float x;
    float y;

};

Point* Point_NewInstance(float, float);

void Point_SetX(Point*, float);
void Point_SetY(Point*, float);

float Point_GetX(Point*);
float Point_GetY(Point*);

void Point_Free(Point*);

#endif
