#include "../../Include/Environment/Environment.h"
#include "../../Include/Utils/Constants.h"
#include "../../Include/Utils/CommonUtils.h"
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

bool EnvironmentClass::spawnAgent(agentAttribute_t attr){
    /* we need to extract, number of cells to draw, shape, dim and position
    */
    int numCells = attr.numParticles;
    for(int i = 0; i < numCells; i++){
        setCellAsAgent(attr.com);
    }
}

/* |-----------------------------------------------------|
 * |         OVERRIDE BASE CLASS FUNCTIONS               |
 * |-----------------------------------------------------|
*/
void EnvironmentClass::setInitialCells(void){
#if ENV_SCENE_0 == 1
    int borderWidth = 0.03 * N;
    /* surfaces
    */
    setCellAsObstacleStream({0, 0}, {N-1, 0}, borderWidth, BOTTOM);
    setCellAsObstacleStream({0, 0}, {0, N-1}, borderWidth, LEFT);
    setCellAsObstacleStream({N-1, 0}, {N-1, N-1}, borderWidth, RIGHT);
    setCellAsObstacleStream({N/2, N/2}, {N-1, N/2}, borderWidth, BOTTOM);
    /* friction
    */
    envAttribute.frictionList.push_back(0.5);
    envAttribute.frictionList.push_back(0.5);
    envAttribute.frictionList.push_back(0.5);
    envAttribute.frictionList.push_back(0.5);
#endif
}

void EnvironmentClass::simulationStep(void){
    /* create agent on mouse click
    */
    if(mouseClicked){
        mouseClicked = false;
        std::pair<int, int> cellPos = mouseAction(xPos, yPos);
        int agentID = createAgent_test(cellPos);
        /* add agent id to list
        */
        agentIDList.push_back(agentID);
    }
    /* draw all agents
    */
    for(int i = 0; i < agentIDList.size(); i++){
        int agentID = agentIDList[i];
        agentAttribute_t attr = agentMap[agentID];
        spawnAgent(attr);
    }

    /* update agent
    */

}