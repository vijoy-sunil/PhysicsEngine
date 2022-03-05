#ifndef UTILS_GRIDUTILS_H
#define UTILS_GRIDUTILS_H

#include "../../Include/Visualization/Grid/Grid.h"
#include <vector>

/* cell state
*/
typedef enum{
    FREE,
    AGENT,
    OBSTACLE
}cellState_t;

class GridUtilsClass: public GridClass{
    private:
        /* NxN grid that stores the cell states
        */
        int *cellState;
        
    protected:
        /* grid size
        */
        int N;
        
        int getIdx(std::pair<int, int> pos);
        bool isCellFree(std::pair<int, int> pos);
        bool isCellAgent(std::pair<int, int> pos);
        bool isCellObstacle(std::pair<int, int> pos);

        void setCellAsFree(std::pair<int, int> pos);
        void setCellAsAgent(std::pair<int, int> pos);
        void setCellAsObstacle(std::pair<int, int> pos);

        void setCellStreamAsState(std::pair<int, int> posStart, 
        std::pair<int, int> posEnd, cellState_t state);

        void setCellStreamColor(std::pair<int, int> posStart, 
        std::pair<int, int> posEnd, colorVal cVal, float alpha=1.0);
        void setCellColorFromState(std::pair<int, int> pos, cellState_t state, float alpha=1.0);

        std::vector<std::pair<int, int> > connectTwoCells(std::pair<int, int> posStart, 
        std::pair<int, int> posEnd);

    public:
        GridUtilsClass(int _N, int _scale, bool noStroke);
        ~GridUtilsClass(void);
};
#endif /* UTILS_GRIDUTILS_H
*/
