#include "../../Include/Environment/Environment.h"
#include <limits.h>
#include <cmath>
#include <iostream>

/* detect collision and return collision normal; THIS IS CALLED BEFORE DRAWING THE AGENT
 * 
 * The collision normal is a vector which is used to calculate impulses after the collision. If 
 * you take a look at the impulse calculations you'll see the collision normal being used. 
 * Basically in a collision (in a simple case), velocities get reflected about the collision 
 * normal.The collision normal is calculated depending on the type of collision that has occured. 
 *
 * If the collision was vertex-vertex this is a degenerate case and there is an infinite choice 
 * of collision normals. To counter this problem you're better of considering this collision as a 
 * vertex-edge collision by choosing an edge on either of the polygons. 
 * 
 * In the vertex-edge collision the collision normal is the perpendicular to the collision edge.
*/
std::vector<vector2f_t> EnvironmentClass::detectCollision(agentAttribute_t attr){
    std::vector<vector2f_t> collisionPoints;
    std::vector<float> collisionNormals;

    std::vector<std::pair<int, int> > points;
    float px, py;

    /* iterate over all 4 edges of the agent
    */
    std::vector<std::pair<float, float>> agentVertices = {
        {attr.vertices.v0.x, attr.vertices.v0.y},
        {attr.vertices.v1.x, attr.vertices.v1.y},
        {attr.vertices.v2.x, attr.vertices.v2.y},
        {attr.vertices.v3.x, attr.vertices.v3.y},
        {attr.vertices.v0.x, attr.vertices.v0.y}
    };

    for(int i = 0; i < 4; i++){
        points = connectTwoCells(agentVertices[i], agentVertices[i + 1]);
        /* slope calculation
        */
        double dy = agentVertices[i + 1].second - agentVertices[i].second;
        double dx = agentVertices[i + 1].first - agentVertices[i].first;
        /* division by zero guard
        */
        double slopeVal = dx == 0 ? INT_MAX : dy/dx;
        float slope = atan(slopeVal);

        for(int i = 0; i < points.size(); i++){
            px = points[i].first;
            py = points[i].second;

            if(!isCellFree({px, py})){
                collisionPoints.push_back({px, py});
                /* calculate normal = slope - 90 degrees in radians
                */
                collisionNormals.push_back(slope - 1.570);
            }
        }
    }

#if 1
    std::cout<<"[DEBUG] Collision points and normals for agent id "<<attr.id<<std::endl;
    for(int i = 0; i < collisionPoints.size(); i++){
        std::cout<<collisionPoints[i].x<<","<<collisionPoints[i].y<<" "
        <<collisionNormals[i]<<std::endl;
    }
#endif
    return collisionPoints;
}

