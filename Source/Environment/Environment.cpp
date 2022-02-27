#include "../../Include/Environment/Environment.h"
#include <iostream>

EnvironmentClass::EnvironmentClass(float _g, float _d, 
                                   int _N, int _scale, bool noStroke):
AgentClass(),
GridUtilsClass(_N, _scale, noStroke)
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
    int borderWidth = 0.03 * N;
    /* surfaces
    */
    setCellAsObstacleStream({0, 0}, {N-1, 0}, borderWidth, BOTTOM);
    setCellAsObstacleStream({N-1, 0}, {N-1, N-1}, borderWidth, RIGHT);
    setCellAsObstacleStream({0, 0}, {0, N-1}, borderWidth, LEFT);
    /* friction
    */
    envAttribute.frictionList.push_back(0.5);
    envAttribute.frictionList.push_back(0.5);
    envAttribute.frictionList.push_back(0.5);
}

void EnvironmentClass::simulationStep(void){

}