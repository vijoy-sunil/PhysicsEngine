#ifndef AGENT_H
#define AGENT_H

#include "../../Include/Utils/CommonUtils.h"
#include <map>

/* attributes of the agent core
 * id                       : each agent will have an unique id
 * mass                     : mass in kg
 * width                    : width in cells (min resolution 1) [odd number]
 * height                   : height in cells (min resolition 1) [odd number]
 * coefficientOfRestitution : ratio of the final to initial relative speed between two objects 
 *                            after they collide. It normally ranges from 0 to 1 where 1 would be 
 *                            a perfectly elastic collision
 * momentOfInertia          : the moment of inertia measures the extent to which an object resists
 *                            rotational acceleration about a particular axis. 
 * positionCenterOfMass     : cell position of com
 * velocityCenterOfMass     : com velocity in m/s
 * orientation              : x, y vector; get angle from sqrt(x2 + y2) (radians)
 * angularVelocity          : x, y (radians/s)
 * vertices                 : 4 pairs of corner vertices
*/
typedef struct{
    int id;
    float mass;
    float width;
    float height;
    float coefficientOfRestitution;
    float momentOfInertia;
    vector2f_t positionCenterOfMass;
    vector2f_t velocityCenterOfMass;
    vector2f_t orientation;
    vector2f_t angularVelocity;
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
                        float width, 
                        float height,
                        float epsilon, 
                        vector2f_t comPos,
                        vector2f_t comVel,
                        vector2f_t alpha,
                        vector2f_t omega
                        );
        bool removeAgent(int id);
};
#endif /* AGENT_H
*/