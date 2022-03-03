#ifndef UTILS_COMMONUTILS_H
#define UTILS_COMMONUTILS_H

typedef struct vector2{
    float x;
    float y;
}vector2f_t;

typedef struct vector4{
    vector2f_t v0;
    vector2f_t v1;
    vector2f_t v2;
    vector2f_t v3;
}vector4v_t;

extern double xPos, yPos;
extern volatile bool mouseClicked;

int getRandomAmount(int start, int end);
#endif /* UTILS_COMMONUTILS_H
*/