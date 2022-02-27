#ifndef AGENT_H
#define AGENT_H

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
 * dim                  : x, y dimensions
 * com                  : center of mass (x, y) point
 * numParticles         : number of particels that make up this agent
 * mass                 : mass in kg
 * surfaceSmoothness    : defines smoothness of agent surface [0(smooth) to 1(rough)]
 * velocity             : [angle, magnitude] pair (m/s)
 * acceleration         : [angle, magniture] pair (m/s2)
*/
typedef struct{
    int id;
    agentShape_t shape;
    std::pair<int, int> dim;
    std::pair<int, int> com;
    int numParticles;
    float mass;
    float surfaceSmoothness;
    std::pair<float, float> velocity;
    std::pair<float, float> acceleration;
}agentAttribute_t;

class AgentClass{
    private:
        int id;
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