#ifndef AGENT_H
#define AGENT_H

#include "../../Include/Utils/CommonUtils.h"
#include <map>

/* attributes of the agent core
 * id                       : each agent will have an unique id
 * mass                     : mass in kg
 * width                    : width in cells (min resolution 1) [odd number]
 * height                   : height in cells (min resolition 1) [odd number]
 * orientation              : angle wrt x axis, + in counter clockwise (radians)
 * angularVelocity          : angular velocity in (radiands/s)
 * coefficientOfRestitution : ratio of the final to initial relative speed between two objects 
 *                            after they collide. It normally ranges from 0 to 1 where 1 would be 
 *                            a perfectly elastic collision
 * positionCenterOfMass     : cell position of com
 * velocityCenterOfMass     : com velocity in m/s
 * vertices                 : 4 pairs of corner vertices
*/
typedef struct{
    int id;
    float mass;
    float width;
    float height;
    float orientation;
    float angularVelocity;
    float coefficientOfRestitution;
    vector2f_t positionCenterOfMass;
    vector2f_t velocityCenterOfMass;
    vector4v_t vertices;
}agentAttribute_t;

class AgentClass{
    private:
        int id;
 
    protected:
        std::map<int, agentAttribute_t> agentMap;
        vector4v_t computeBoundingVertices(agentAttribute_t attr);
        void dumpAgentMap(int id);
        
    public:
        AgentClass(void);
        ~AgentClass(void);

        int createAgent(float mass, 
                        float width, float height,
                        float alpha, 
                        float omega,
                        float epsilon, 
                        vector2f_t comPos,
                        vector2f_t comVel
                        );
        bool removeAgent(int id);
};
#endif /* AGENT_H
*/