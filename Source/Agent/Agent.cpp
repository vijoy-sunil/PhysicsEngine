#include "../../Include/Agent/Agent.h"
#include <iostream>
#include <cassert>
#include <cmath>

AgentClass::AgentClass(void){
    /* agent id initial value
    */
    id = 0;
}

AgentClass::~AgentClass(void){
}

void AgentClass::dumpActiveAgents(void){
    std::cout<<"[DEBUG] Dumping agent map"<<std::endl;

    for(auto it = agentMap.begin(); it != agentMap.end(); it++){
        std::cout<<"agent id: "<<it->first<<std::endl;
        std::cout<<"shape: "<<it->second.shape<<std::endl;
        std::cout<<"num particles: "<<it->second.numParticles<<std::endl;
        std::cout<<"dim: "<<it->second.dim.first<<","<<it->second.dim.second<<std::endl;
        std::cout<<"com: "<<it->second.com.first<<","<<it->second.com.second<<std::endl;
        std::cout<<"mass: "<<it->second.mass<<std::endl;
        std::cout<<"surface smoothness: "<<it->second.surfaceSmoothness<<std::endl;

        std::cout<<"velocity: "<<it->second.velocity.x<<","
        <<it->second.velocity.y<<std::endl;

        std::cout<<"collisionFace: "<<it->second.collisionFace.first<<","
        <<it->second.collisionFace.second<<std::endl;
    }
}

int AgentClass::createAgent_test(std::pair<int, int> pos){
    /* fill up attr
    */
    agentAttribute_t attr;
    attr.id = id++;
    attr.shape = RECTANGLE;

    attr.numParticles = 1;
    int dimVal = int(sqrt(attr.numParticles));
    attr.dim = {dimVal, dimVal};

    attr.com = pos;
    attr.mass = 10;
    attr.surfaceSmoothness = 0.5;
    attr.velocity = {0.0, 0.0};
    attr.collisionFace = {0, 0};
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