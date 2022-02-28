#include "../Include/Utils/Constants.h"
#include "../Include/Environment/Environment.h"

int main(void){
    EnvironmentClass env(_dt, _g, _d, _N, _scale, true);
    env.runRender();
    return 0;
}