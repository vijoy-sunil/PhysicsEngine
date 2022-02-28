#include "../../Include/Environment/Environment.h"
#include "../../Include/Utils/Constants.h"
#include "../../Include/Utils/CommonUtils.h"
#include <iostream>

EnvironmentClass::EnvironmentClass(float _dt, float _g, float _d, 
                                   int _N, int _scale, bool noStroke):
AgentClass(),
GridUtilsClass(_N, _scale, noStroke)
{
    /* save env attribute
    */
    envAttribute.dt = _dt;
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

std::pair<int, int> EnvironmentClass::computeForce(agentAttribute_t attr){
    /* gravitational force
    */
    std::pair<float, float> gravitationalForce = {0, attr.mass * envAttribute.gravity};

    /* sum up all forces acting on the agent
    */
    std::pair<float, float> cumulativeForce = {0.0, 0.0};
    cumulativeForce.first += gravitationalForce.first;
    cumulativeForce.second += gravitationalForce.second;

    return cumulativeForce;
}

std::pair<int, int> EnvironmentClass::computeMotion(agentAttribute_t attr){
    int posX = attr.com.first;
    int posY = attr.com.second;

    posX += attr.velocity.first * envAttribute.dt;
    posY += attr.velocity.second * envAttribute.dt;

    return {posX, posY};
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
    envAttribute.surfaceSmoothness.push_back(0.5);
    envAttribute.surfaceSmoothness.push_back(0.5);
    envAttribute.surfaceSmoothness.push_back(0.5);
    envAttribute.surfaceSmoothness.push_back(0.5);
#endif
}

void EnvironmentClass::simulationStep(void){
    /* calculate all forces for all agents and update its velocity attribute
    */
    for(int i = 0; i < agentIDList.size(); i++){
        int agentID = agentIDList[i];
        agentAttribute_t attr = agentMap[agentID];
        /* |-----------------------------------------------------|
         * |            PHYSICS COMPUTE FORCES                   |
         * | calculate velocity and acceleration based on forces |
         * |-----------------------------------------------------|
         */
        std::pair<float, float> force = computeForce(attr);
        /* update acceleration and velocity
        */
        std::pair<float, float> acceleration = {force.first/attr.mass, force.second/attr.mass};
        attr.velocity.first += acceleration.first * envAttribute.dt;
        attr.velocity.second += acceleration.second * envAttribute.dt;
        /* update map
        */
        agentMap[agentID] = attr;
    }

    /* update all agents' com, updated agents will be drawn in next frame. NOTE: save previous 
     * agent.com before overwriting it. If they are the same, we don't need to clear the agent. 
     * If different, clear previous.
    */
    for(int i = 0; i < agentIDList.size(); i++){
        int agentID = agentIDList[i];
        agentAttribute_t attr = agentMap[agentID];

        std::pair<int, int> currCom = attr.com;
        /* |-----------------------------------------------------|
         * |            PHYSICS COMPUTE MOTION                   |
         * |       calculate nextCom based on velocity           |
         * |-----------------------------------------------------|
         */
        std::pair<int, int> nextCom = computeMotion(attr);
        /* clear agent if currCom != nextCom
        */
        if(currCom.first != nextCom.first || currCom.second != nextCom.second){
            if(attr.shape == RECTANGLE)
                attr.numParticles == 1 ? setCellAsFree(attr.com) : 
                setBlockAsFree(attr.com, attr.dim);      

            /* overwrite currCom with nextCom and update agent map
            */
            attr.com = nextCom;   
            agentMap[agentID] = attr;
        }
    }

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

#if DEBUG_PRINT == 1
    if(agentIDList.size() != 0){
        agentAttribute_t attr = agentMap[0];
        std::cout<<"[DEBUG] agent vel: "<<attr.velocity.first<<","
        <<attr.velocity.second<<std::endl;
    }
#endif
}