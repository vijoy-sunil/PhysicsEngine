#include "../../Include/Agent/Agent.h"
#include <iostream>
#include <cmath>

AgentClass::AgentClass(void){
    /* agent id initial value
    */
    id = 0;
}

AgentClass::~AgentClass(void){
}

vector4v_t AgentClass::computeBoundingVertices(agentAttribute_t attr){
    float halfWidth = attr.width/2;
    float halfHeight = attr.height/2;

    /* compute 2d rotation matrix
    */
    float alpha = atan2(attr.orientation.y, attr.orientation.x);
    float cosVal = cos(alpha);
    float sineVal = sin(alpha);
    /* matrix multiplication for 4 vertices
     * x' = xcos - ysin
     * y' = xsin + ycos
    */
    vector2f_t vertex0;
    vertex0.x = attr.positionCenterOfMass.x + ((-halfWidth * cosVal) - (-halfHeight * sineVal));
    vertex0.y = attr.positionCenterOfMass.y + ((-halfWidth * sineVal) + (-halfHeight * cosVal));

    vector2f_t vertex1;
    vertex1.x = attr.positionCenterOfMass.x + ((halfWidth * cosVal) - (-halfHeight * sineVal));
    vertex1.y = attr.positionCenterOfMass.y + ((halfWidth * sineVal) + (-halfHeight * cosVal));

    vector2f_t vertex2;
    vertex2.x = attr.positionCenterOfMass.x + ((halfWidth * cosVal) - (halfHeight * sineVal));
    vertex2.y = attr.positionCenterOfMass.y + ((halfWidth * sineVal) + (halfHeight * cosVal));

    vector2f_t vertex3;
    vertex3.x = attr.positionCenterOfMass.x + ((-halfWidth * cosVal) - (halfHeight * sineVal));
    vertex3.y = attr.positionCenterOfMass.y + ((-halfWidth * sineVal) + (halfHeight * cosVal));

#if 0
    std::cout<<"V0:"<<vertex0.x<<","<<vertex0.y<<std::endl;
    std::cout<<"V1:"<<vertex1.x<<","<<vertex1.y<<std::endl;
    std::cout<<"V2:"<<vertex2.x<<","<<vertex2.y<<std::endl;
    std::cout<<"V3:"<<vertex3.x<<","<<vertex3.y<<std::endl;
#endif

    return {vertex0, vertex1, vertex2, vertex3};
}

void AgentClass::dumpAgentMap(int id){
    std::cout<<"[DEBUG] Dumping agent map"<<std::endl;
    agentAttribute_t attr = agentMap[id];

    std::cout<<"[DEBUG] id: "<<attr.id<<std::endl;
    std::cout<<"[DEBUG] mass: "<<attr.mass<<std::endl;
    std::cout<<"[DEBUG] width: "<<attr.width<<std::endl;
    std::cout<<"[DEBUG] height: "<<attr.height<<std::endl;
    std::cout<<"[DEBUG] coefficientOfRestitution: "<<attr.coefficientOfRestitution<<std::endl;
    std::cout<<"[DEBUG] momentOfInertia: "<<attr.momentOfInertia<<std::endl;

    std::cout<<"[DEBUG] positionCenterOfMass: "
    <<attr.positionCenterOfMass.x<<","<<attr.positionCenterOfMass.y<<std::endl;

    std::cout<<"[DEBUG] velocityCenterOfMass: "
    <<attr.velocityCenterOfMass.x<<","<<attr.velocityCenterOfMass.y<<std::endl;

    std::cout<<"[DEBUG] orientation: "
    <<attr.orientation.x<<","<<attr.orientation.y<<std::endl;
    std::cout<<"[DEBUG] angularVelocity: "
    <<attr.angularVelocity.x<<","<<attr.angularVelocity.y<<std::endl;

    std::cout<<"[DEBUG] vertices: "
    <<attr.vertices.v0.x<<","<<attr.vertices.v0.y<<" "
    <<attr.vertices.v1.x<<","<<attr.vertices.v1.y<<" "
    <<attr.vertices.v2.x<<","<<attr.vertices.v2.y<<" "
    <<attr.vertices.v3.x<<","<<attr.vertices.v3.y<<std::endl;
}

int AgentClass::createAgent(float mass, 
                            float width, 
                            float height,
                            float epsilon, 
                            vector2f_t comPos,
                            vector2f_t comVel,
                            vector2f_t alpha,
                            vector2f_t omega
                            ){
    /* fill up attr
    */
    agentAttribute_t attr;
    attr.id = id++;
    attr.mass = mass;
    attr.width = width;
    attr.height = height;
    attr.coefficientOfRestitution = epsilon;
    /* For a rectangle with the axis of rotation going through its center of mass (z axis), the 
     * moment of inertia is given by the following equation:
     * I = (mass / 12.0f) * (width * width + height * height)
    */
    attr.momentOfInertia = (attr.mass/12.0f) * 
    (attr.width * attr.width + attr.height * attr.height);

    attr.positionCenterOfMass = comPos;
    attr.velocityCenterOfMass = comVel;
    attr.orientation = alpha;
    attr.angularVelocity = omega;

    attr.vertices = computeBoundingVertices(attr);
    /* add to map
    */
    agentMap[attr.id] = attr;
    std::cout<<"[INFO] Created agent "<<attr.id<<std::endl;
    return attr.id;
}

bool AgentClass::removeAgent(int id){
    if(agentMap.find(id) != agentMap.end()){
        agentMap.erase(id);
        std::cout<<"[INFO] Removed agent "<<id<<std::endl;
        return true;
    }
    else{
        std::cout<<"[ERROR] Unable to remove agent "<<id<<std::endl;
        return false;
    }
}