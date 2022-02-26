#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../../Include/Visualization/Grid/Grid.h"

class EnvironmentClass: public GridClass{
    private:

    public:
        EnvironmentClass(int _N, int _scale, bool noStroke);
        ~EnvironmentClass(void);

        /* override grid functions
        */
        void setInitialCells(void);
        void simulationStep(void);
};
#endif /* ENVIRONMENT_H
*/