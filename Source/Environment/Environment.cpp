#include "../../Include/Environment/Environment.h"
#include "../../Include/Utils/Constants.h"
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
    envAttribute.numWalls = 0;
}

EnvironmentClass::~EnvironmentClass(void){
}

void EnvironmentClass::setMeshAsState(vector4v_t vertices, cellState_t state){
    setCellStreamAsState({vertices.v0.x, vertices.v0.y}, 
                         {vertices.v1.x, vertices.v1.y}, state);
    setCellStreamAsState({vertices.v1.x, vertices.v1.y}, 
                         {vertices.v2.x, vertices.v2.y}, state);
    setCellStreamAsState({vertices.v2.x, vertices.v2.y}, 
                         {vertices.v3.x, vertices.v3.y}, state);
    setCellStreamAsState({vertices.v3.x, vertices.v3.y}, 
                         {vertices.v0.x, vertices.v0.y}, state);
}

bool EnvironmentClass::spawnAgent(agentAttribute_t attr){
    /* draw agent
    */
    setMeshAsState(attr.vertices, AGENT);
    /* draw com
    */
    setCellAsAgent({attr.positionCenterOfMass.x, attr.positionCenterOfMass.y});
    return true;
}

bool EnvironmentClass::killAgent(agentAttribute_t attr){
    /* clear agent
    */
    setMeshAsState(attr.vertices, FREE);
    /* clear com
    */
    setCellAsFree({attr.positionCenterOfMass.x, attr.positionCenterOfMass.y});
    return true;
}

bool EnvironmentClass::validPendingAgent(int agentID){
    /* check spawn agent collision
    */
    std::pair<std::vector<vector2f_t>, std::vector<float>> collisionPair = 
    detectCollision(agentMap[agentID]);

    if(collisionPair.first.size() != 0)
        return false;
    return true;
}

void EnvironmentClass::setInitialCells(void){
#if ENV_SCENE_0 == 1
    /* consider walls as agents with infinite mass
    */ 
    float borderWidth = 0.03 * N,
          wallMass = 1000.0;
    int agentID;

    /* wall #1
    */
    agentID = createAgent(wallMass, 
                          N - 1, 
                          borderWidth, 
                          0.0, 
                          {(N - 1)/2.0f, borderWidth/2.0f},
                          {0.0, 0.0},
                          {0.0, 0.0},
                          {0.0, 0.0});
    agentIDList.push_back(agentID);
    envAttribute.numWalls++;

    /* wall #2
    */
    agentID = createAgent(wallMass, 
                          N - 1, 
                          borderWidth, 
                          0.0, 
                          {(N - 1)/2.0f, N - 1 - borderWidth/2.0f},
                          {0.0, 0.0},
                          {0.0, 0.0},
                          {0.0, 0.0});
    agentIDList.push_back(agentID);
    envAttribute.numWalls++;

    /* wall #3
    */
    agentID = createAgent(wallMass, 
                          borderWidth, 
                          (N - 1) - (2 * borderWidth), 
                          0.0, 
                          {N - 1 - borderWidth/2.0f, (N - 1)/2.0f},
                          {0.0, 0.0},
                          {0.0, 0.0},
                          {0.0, 0.0});
    agentIDList.push_back(agentID);
    envAttribute.numWalls++;

    /* wall #4
    */
    agentID = createAgent(wallMass, 
                          borderWidth, 
                          (N - 1) - (2 * borderWidth), 
                          0.0, 
                          {borderWidth/2.0f, (N - 1)/2.0f},
                          {0.0, 0.0},
                          {0.0, 0.0},
                          {0.0, 0.0});
    agentIDList.push_back(agentID);
    envAttribute.numWalls++;

    /* wall #5
    */
    agentID = createAgent(wallMass, 
                          (N - 1)/2 - borderWidth, 
                          borderWidth, 
                          0.0, 
                          {((3 * (N - 1))/4.0f) - borderWidth/2, (N - 1)/2.0f},
                          {0.0, 0.0},
                          {0.0, 0.0},
                          {0.0, 0.0});
    agentIDList.push_back(agentID);
    envAttribute.numWalls++;

    /* draw wall agents
    */
    for(int i = 0; i < envAttribute.numWalls; i++)
        setMeshAsState(agentMap[i].vertices, OBSTACLE);
#endif
}

void EnvironmentClass::simulationStep(void){
    /* create agent on mouse click
    */
    if(mouseClicked){
        mouseClicked = false;
        std::pair<float, float> cellPos = mouseAction(xPos, yPos);
        int agentID = createAgent(1.0, 
                                  101, 
                                  51, 
                                  0.0, 
                                  {cellPos.first, cellPos.second},
                                  {0.0, 0.0},
                                  {0.0, 0.0},
                                  {0.0, 0.0});
        /* create agent that are not interpenetrated
        */
        if(validPendingAgent(agentID))
            agentIDList.push_back(agentID);
        else
            removeAgent(agentID);
        /* draw agent
        */
        spawnAgent(agentMap[agentID]);
    }

    else{
        /* iterate over all agents [excluding wall agents]
        */
        for(int i = envAttribute.numWalls; i < agentIDList.size(); i++){
            int agentID = agentIDList[i];
            agentAttribute_t attr = agentMap[agentID];
            /* erase agent
            */
            killAgent(attr);
            /* compute net force at CM
            */
            vector2f_t netForce = computeNetForce(attr);
            /* compute linear acceleration of CM
            */
            vector2f_t linearAccel = {netForce.x/attr.mass, 
                                    netForce.y/attr.mass};
            /* compute net torque at CM
            */
            vector2f_t netTorque = computeNetTorque(attr);
            /* compute angular acceleration of CM
            */
            vector2f_t angularAccel = {netTorque.x/attr.momentOfInertia, 
                                    netTorque.y/attr.momentOfInertia};
            /* integrate, update and draw agent map
            */
            integrateAndUpdate(linearAccel, angularAccel, attr);
            spawnAgent(agentMap[agentID]);
        }
    }

#if 0
    if(agentIDList.size() != 0)
        dumpAgentMap(0);
#endif
}