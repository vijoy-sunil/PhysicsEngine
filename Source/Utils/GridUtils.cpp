#include "../../Include/Utils/GridUtils.h"
#include <stdlib.h>
#include <cmath>
#include <limits.h>

GridUtilsClass::GridUtilsClass(int _N, int _scale, bool noStroke): 
GridClass(_N, _scale, noStroke){
    N = _N;
    cellState = (int*)calloc(N * N, sizeof(int));
}

GridUtilsClass::~GridUtilsClass(void){
    free(cellState);
}

int GridUtilsClass::getIdx(std::pair<int, int> pos){
    return (pos.first + (pos.second * N));
}

bool GridUtilsClass::isCellFree(std::pair<int, int> pos){
    return cellState[getIdx(pos)] == FREE;
}

bool GridUtilsClass::isCellAgent(std::pair<int, int> pos){
    return cellState[getIdx(pos)] == AGENT;
}

bool GridUtilsClass::isCellObstacle(std::pair<int, int> pos){
    return cellState[getIdx(pos)] == OBSTACLE;
}

void GridUtilsClass::setCellAsFree(std::pair<int, int> pos){
    cellState[getIdx(pos)] = FREE;
    /* set color according to cell state
    */
    setCellColorFromState(pos, FREE);
}

void GridUtilsClass::setCellAsAgent(std::pair<int, int> pos){
    cellState[getIdx(pos)] = AGENT;
    /* set color according to cell state
    */
    setCellColorFromState(pos, AGENT);
}

void GridUtilsClass::setCellAsObstacle(std::pair<int, int> pos){
    cellState[getIdx(pos)] = OBSTACLE;
    /* set color according to cell state
    */
    setCellColorFromState(pos, OBSTACLE);
}

/* used to create line obstacles (walls), this also takes in the width parameter
*/
void GridUtilsClass::setCellAsObstacleStream(std::pair<int, int> posStart, 
std::pair<int, int> posEnd, const int width, widthType_t wType){
    std::vector<std::pair<int, int> > points;
    points = connectTwoCells(posStart, posEnd);

    int j, px, py;
    for(int i = 0; i < points.size(); i++){
        /* increment width
        */
        for(j = 0; j < width; j++){
            px = points[i].first;
            py = points[i].second;

            px = wType == BOTTOM ? px : wType == RIGHT ? px - j : wType == LEFT ? px + j : px;
            py = wType == RIGHT ? py : wType == TOP ? py - j : wType == BOTTOM ? py + j : py;
            setCellAsObstacle({px, py});
        }
    }
}

void GridUtilsClass::setCellColorFromState(std::pair<int, int> pos, 
cellState_t state, float alpha){
    colorVal cVal = state == FREE ? whiteVal : state == AGENT ? redVal: blackVal;
    genCellColor(pos.first, pos.second, cVal, alpha);
}

/* return cell coordinates between (i1,j1) and (i2,j2), both end points included
*/
std::vector<std::pair<int, int>> GridUtilsClass::connectTwoCells(std::pair<int, int> posStart, 
std::pair<int, int> posEnd){
    std::vector<std::pair<int, int>> points;
    /* get slope
    */
    double dy = posEnd.second - posStart.second;
    double dx = posEnd.first - posStart.first;
    /* division by zero guard
    */
    double slope = dx == 0 ? INT_MAX : dy/dx;
    /* generate numPoints between the two points
    */
    int numPoints = N;
    double px, py;
    for(double i = 0; i < numPoints; i++){
        py = slope == 0 ? 0 : dy * (i / numPoints);
        if(slope != INT_MAX)
            px = slope == 0 ? dx * (i / numPoints) : py / slope;
        else
            px = 0;

        points.push_back(std::make_pair((int)round(px) + posStart.first, 
        (int)round(py) + posStart.second));
    }
    /* do not forget to add the destination point as well (i2, j2)
    */
    points.push_back(posEnd);
    return points;
}

