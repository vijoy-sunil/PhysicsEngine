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
    /* draw agent cell/block
    */
    int numCells = attr.numParticles;
    if(attr.shape == RECTANGLE)
        numCells == 1 ? setCellAsAgent(attr.com) : setBlockAsAgent(attr.com, attr.dim);
    return true;
}

bool EnvironmentClass::validPendingAgent(int agentID){
    agentAttribute_t attr = agentMap[agentID];
    /* check if com is in free cell
    */
    if(!isCellFree(attr.com))
        return false;
    /* check all particles
    */
    if(attr.numParticles != 1){
        std::pair<int, int> dimFmt = {attr.dim.first/2, attr.dim.second/2};
        int r, c;
        int i = attr.com.first;
        int j = attr.com.second;

        for(r = -dimFmt.first; r <= dimFmt.first; r++){
            for(c = -dimFmt.second; c <= dimFmt.second; c++){
                if(!isCellFree({i + r, j + c}))
                    return false;
            }
        }
    }
    return true;
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
        /* create agent only if mouse clicked in free cell; and if all particles are in free cell
        */
        if(validPendingAgent(agentID))
            agentIDList.push_back(agentID);
        else
            removeAgent(agentID);
    }
    /* draw all agents
    */
    for(int i = 0; i < agentIDList.size(); i++){
        int agentID = agentIDList[i];
        agentAttribute_t attr = agentMap[agentID];
        spawnAgent(attr);
    }
    /* calculate all forces for all agents and update its velocity and acceleration attributes
    */

    /* update all agents by changing its com based on velocity and acceleration
    */

}