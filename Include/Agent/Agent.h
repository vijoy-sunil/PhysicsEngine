#ifndef AGENT_H
#define AGENT_H

#include "../../Include/Utils/CommonUtils.h"
#include <map>

/* agent body shape
*/
typedef enum{
    RECTANGLE,
    TRIANGLE,
    ELLIPSE
}agentShape_t;

/* attributes of the agent core
 * id                   : each agent will have an unique id
 * shape                : shape of agent
 * numParticles         : number of particles that make up this agent {1, 9, 25 etc.}
 * dim                  : x, y dimensions ({1, 1} x sqrt(numParticles))
 * com                  : center of mass (x, y) point
 * mass                 : mass in kg
 * surfaceSmoothness    : defines smoothness of agent surface [0(smooth) to 1(rough)]
 * velocity             : [velocity in x, velocity in y] (m/s)
*/
typedef struct{
    int id;
    agentShape_t shape;
    int numParticles;
    std::pair<int, int> dim;
    std::pair<int, int> com;
    float mass;
    float surfaceSmoothness;
    vector_t velocity;
}agentAttribute_t;

class AgentClass{
    private:
        int id;
 
    protected:
        std::map<int, agentAttribute_t> agentMap;
        
    public:
        AgentClass(void);
        ~AgentClass(void);

        void dumpActiveAgents(void);
        int createAgent_test(std::pair<int, int> pos);
        bool removeAgent(int id);
};
#endif /* AGENT_H
*/