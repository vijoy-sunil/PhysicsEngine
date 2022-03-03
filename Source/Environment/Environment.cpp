#include "../../Include/Environment/Environment.h"
#include "../../Include/Utils/Constants.h"
#include <iostream>
#include <algorithm>

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
    vector4v_t agentVertices = agentMap[agentID].vertices;
    float agentX1 = std::min(agentVertices.v0.x, 
                    std::min(agentVertices.v1.x, 
                    std::min(agentVertices.v2.x, agentVertices.v3.x)));

    float agentX2 = std::max(agentVertices.v0.x, 
                    std::max(agentVertices.v1.x, 
                    std::max(agentVertices.v2.x, agentVertices.v3.x)));

    float agentY1 = std::min(agentVertices.v0.y, 
                    std::min(agentVertices.v1.y, 
                    std::min(agentVertices.v2.y, agentVertices.v3.y)));

    float agentY2 = std::max(agentVertices.v0.y, 
                    std::max(agentVertices.v1.y, 
                    std::max(agentVertices.v2.y, agentVertices.v3.y)));

    /* check for collision with  walls before spawning in
    */
    for(int i = 0; i < envAttribute.walls.size(); i++){
        vector4v_t wallVertices = envAttribute.walls[i];
        float wallX1 = wallVertices.v0.x;
        float wallX2 = wallVertices.v1.x;
        float wallY1 = wallVertices.v0.y;
        float wallY2 = wallVertices.v3.y;

        /* check inerval overlap
        */
        if(((wallX1 <= agentX2) && (agentX1 <= wallX2)) &&
           ((wallY1 <= agentY2) && (agentY1 <= wallY2)))
            return false;
    }
    /* TODO : check spawn agent overlap with other agent
    */

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
    attr.orientation = 0.0;
    attr.positionCenterOfMass = {attr.width/2, attr.height/2};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);

    /* wall #2
    */
    attr.width = N - 1;
    attr.height = borderWidth;
    attr.orientation = 0.0;
    attr.positionCenterOfMass = {attr.width/2, N - 1 - attr.height/2};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);

    /* wall #3
    */
    attr.width = borderWidth;
    attr.height = (N - 1) - (2 * borderWidth);
    attr.orientation = 0.0;
    attr.positionCenterOfMass = {N - 1 - attr.width/2, (N - 1)/2.0f};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);  

    /* wall #4
    */
    attr.width = borderWidth;
    attr.height = (N - 1) - (2 * borderWidth);
    attr.orientation = 0.0;
    attr.positionCenterOfMass = {attr.width/2, (N - 1)/2.0f};
    attr.vertices = computeBoundingVertices(attr);
    /* add vertices to env attribute
    */
    envAttribute.walls.push_back(attr.vertices);  

    /* wall #5
    */
    attr.width = (N - 1)/2 - borderWidth;
    attr.height = borderWidth;
    attr.orientation = 0.0;
    attr.positionCenterOfMass = {((3 * (N - 1))/4.0f) - borderWidth/2, (N - 1)/2.0f};
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
    /* calculate all forces for all agents and update its velocity attribute
    */
    for(int i = 0; i < agentIDList.size(); i++){
        int agentID = agentIDList[i];
        agentAttribute_t attr = agentMap[agentID];

    }

    /* create agent on mouse click
    */
    if(mouseClicked){
        mouseClicked = false;
        std::pair<float, float> cellPos = mouseAction(xPos, yPos);
        int agentID = createAgent(1.0, 
                                 101, 51, 
                                 0.0, 
                                 0.0,
                                 0.0,
                                 {cellPos.first, cellPos.second},
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
        spawnAgent(attr);
    }

#if DEBUG_PRINT == 1
    if(agentIDList.size() != 0)
        dumpAgentMap(0);
#endif
}