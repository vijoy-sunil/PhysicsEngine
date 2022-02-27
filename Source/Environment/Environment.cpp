#include "../../Include/Environment/Environment.h"
#include <iostream>

EnvironmentClass::EnvironmentClass(float _g, float _d, 
                                   int _N, int _scale, bool noStroke):
AgentClass(),
GridClass(_N, _scale, noStroke)
{
    /* save env attribute
    */
    envAttribute.gravity = _g;
    envAttribute.density = _d;
}

EnvironmentClass::~EnvironmentClass(void){
}

/* |-----------------------------------------------------|
 * |         OVERRIDE BASE CLASS FUNCTIONS               |
 * |-----------------------------------------------------|
*/
void EnvironmentClass::setInitialCells(void){
    
}

void EnvironmentClass::simulationStep(void){

}