#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../../Include/Agent/Agent.h"
#include "../../Include/Utils/GridUtils.h"
#include "../../Include/Utils/CommonUtils.h"
#include <vector>

/* attributes of the environment core
 * dt                   : simulation timestep (s)
 * gravity              : gravitational force on the atmosphere operates uniformly (m/s2)
 * density              : drag experienced by a falling object depends on the air density (kg/m3)
 * numWalls             : number of agents that are walls, agentID [0 ... numWalls-1]
*/
typedef struct{
    float dt;
    float gravity;
    float density;
    int numWalls;
}envAttribute_t;

/* collision info
*/
typedef struct{
    bool isCollision;
    std::vector<int> agentIDs;
    std::vector<vector2f_t> points;
    std::vector<float> normals;
}collisionInfo_t;

class EnvironmentClass: public AgentClass, public GridUtilsClass{
    private:
        /* env attribute
        */
        envAttribute_t envAttribute;
        /* maintained list for agent ids
        */
        std::vector<int> agentIDList;

    protected:
        /* agent draw fns
        */
        void setMeshAsState(vector4v_t vertices, cellState_t state);
        bool spawnAgent(agentAttribute_t attr);
        bool killAgent(agentAttribute_t attr);
        bool validPendingAgent(int agentID);
        /* collision
        */
        void highlightCollisionPoints(bool set, agentAttribute_t attr, 
        std::vector<vector2f_t> collisionPoints);
        int getCollidingAgent(agentAttribute_t primaryAttr,
        std::pair<int, int> point);
        collisionInfo_t detectCollision(agentAttribute_t attr);
        vector2f_t collisionResponse(agentAttribute_t attr);
        /* physics
        */
        vector2f_t computeDragForce(agentAttribute_t attr);
        vector2f_t computeNormalForce(agentAttribute_t attr);
        vector2f_t computeNetForce(agentAttribute_t attr);
        vector2f_t computeNetTorque(agentAttribute_t attr);
        void integrateAndUpdate(vector2f_t linearAccel, vector2f_t angularAccel, 
        agentAttribute_t attr);

    public:
        EnvironmentClass(float _dt, float _g, float _d, int _N, int _scale, bool noStroke);
        ~EnvironmentClass(void);
        /* override grid functions
        */
        void setInitialCells(void);
        void simulationStep(void);
};
#endif /* ENVIRONMENT_H
*/