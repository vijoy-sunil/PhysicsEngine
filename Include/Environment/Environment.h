#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../../Include/Utils/GridUtils.h"
#include "../../Include/Agent/Agent.h"
#include <vector>

/* attributes of the environment core
 * dt                   : simulation timestep
 * gravity              : gravitational force on the atmosphere operates uniformly (m/s2)
 * density              : drag experienced by a falling object depends on the air density (kg/m3)
 * surfaceSmoothness    : surface smoothness for different types of surfaces
*/
typedef struct{
    float dt;
    float gravity;
    float density;
    std::vector<float> surfaceSmoothness;
}envAttribute_t;

class EnvironmentClass: public AgentClass, public GridUtilsClass{
    private:
        /* env attribute
        */
        envAttribute_t envAttribute;
        /* maintained list for agent ids
        */
        std::vector<int> agentIDList;

    public:
        EnvironmentClass(float _dt, float _g, float _d, int _N, int _scale, bool noStroke);
        ~EnvironmentClass(void);
        /* agent fns
        */
        bool spawnAgent(agentAttribute_t attr);
        bool validPendingAgent(int agentID);
        /* physics
        */
        std::pair<int, int> computeForce(agentAttribute_t attr);
        std::pair<int, int> computeMotion(agentAttribute_t attr);
        bool addForce(std::pair<int, int> pos, float magnitude, float direction);
        /* override grid functions
        */
        void setInitialCells(void);
        void simulationStep(void);
};
#endif /* ENVIRONMENT_H
*/