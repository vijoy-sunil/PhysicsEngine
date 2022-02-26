#include "../../Include/Environment/Environment.h"

EnvironmentClass::EnvironmentClass(int _N, int _scale, bool noStroke):
GridClass(_N, _scale, noStroke){

}

EnvironmentClass::~EnvironmentClass(void){

}

/*  +-------------------------------------+
 *  | override base class functions below |
 *  +-------------------------------------+
*/
void EnvironmentClass::setInitialCells(void){
    genCellColor(10, 10, greenVal, 1.0);
}

void EnvironmentClass::simulationStep(void){

}