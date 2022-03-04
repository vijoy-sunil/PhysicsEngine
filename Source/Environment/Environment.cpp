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
    /* check spawn agent collision with wall/other agents
    */
    std::pair<std::vector<vector2f_t>, std::vector<float>> collisionPair = 
    detectCollision(agentMap[agentID]);
    if(collisionPair.first.size() != 0)
        return false;
    return true;
}

void EnvironmentClass::setInitialCells(void){
#if ENV_SCENE_0 == 1
    float borderWidth = 0.03 * N;
    /* consider walls as static agents, we use the attributes to calculate bounding vertices
    */ 
    agentAttribute_t attr;
    /* wall #1
    */
    attr.width = N - 1;
    attr.height = borderWidth;
    attr.positionCenterOfMass = {attr.width/2, attr.height/2};
    attr.orientation = {0.0, 0.0};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);

    /* wall #2
    */
    attr.width = N - 1;
    attr.height = borderWidth;
    attr.positionCenterOfMass = {attr.width/2, N - 1 - attr.height/2};
    attr.orientation = {0.0, 0.0};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);

    /* wall #3
    */
    attr.width = borderWidth;
    attr.height = (N - 1) - (2 * borderWidth);
    attr.positionCenterOfMass = {N - 1 - attr.width/2, (N - 1)/2.0f};
    attr.orientation = {0.0, 0.0};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);  

    /* wall #4
    */
    attr.width = borderWidth;
    attr.height = (N - 1) - (2 * borderWidth);
    attr.positionCenterOfMass = {attr.width/2, (N - 1)/2.0f};
    attr.orientation = {0.0, 0.0};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);  

    /* wall #5
    */
    attr.width = (N - 1)/2 - borderWidth;
    attr.height = borderWidth;
    attr.positionCenterOfMass = {((3 * (N - 1))/4.0f) - borderWidth/2, (N - 1)/2.0f};
    attr.orientation = {0.0, 0.0};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices); 

    /* surfaces
    */
    for(int i = 0; i < envAttribute.walls.size(); i++)
        setMeshAsState(envAttribute.walls[i], OBSTACLE);
#endif
}

void EnvironmentClass::simulationStep(void){
    /* iterate over all agents
    */
    for(int i = 0; i < agentIDList.size(); i++){
        int agentID = agentIDList[i];
        agentAttribute_t attr = agentMap[agentID];
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
        /* integrate, update agent map
        */
        killAgent(attr);
        integrateAndUpdate(linearAccel, angularAccel, attr);
    }

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
        /* update agent spawn
        */
        spawnAgent(attr);
    }

#if 0
    if(agentIDList.size() != 0)
        dumpAgentMap(0);
#endif
}