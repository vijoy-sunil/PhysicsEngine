#ifndef UTILS_COMMONUTILS_H
#define UTILS_COMMONUTILS_H

typedef struct{
    float x;
    float y;

    int getDirection(float attr){
        return attr < 0 ? -1 : 1;
    }
}vector_t;

extern double xPos, yPos;
extern volatile bool mouseClicked;

int getRandomAmount(int start, int end);
#endif /* UTILS_COMMONUTILS_H
*/