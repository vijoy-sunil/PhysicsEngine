#include "../../Include/Environment/Environment.h"
#include <cmath>
#include <limits.h>
#include <iostream>

/* set/unset highlight for collision points - draw line from CM to collision points
*/
void EnvironmentClass::highlightCollisionPoints(bool set, agentAttribute_t attr, 
std::vector<vector2f_t> collisionPoints){
    for(int i = 0; i < collisionPoints.size(); i++){
        std::pair<int, int> posStart = {attr.positionCenterOfMass.x, 
                                        attr.positionCenterOfMass.y};
        std::pair<int, int> posEnd = {collisionPoints[i].x,
                                      collisionPoints[i].y};
        if(set)
            setCellStreamColor(posStart, posEnd, blueVal);
        else
            setCellStreamColor(posStart, posEnd, whiteVal);
    }
}

int EnvironmentClass::getCollidingAgent(agentAttribute_t primaryAttr,
std::pair<int, int> point){
    /* iterate over all agents; skip primary agent
    */
    for(int i = 0; i < agentIDList.size(); i++){
        /* skip primary attribute
        */
        if(i == primaryAttr.id)
            continue;

        int agentID = agentIDList[i];
        agentAttribute_t secondaryAttr = agentMap[agentID];
        /* edges of secondary agent
        */
        std::vector<vector2f_t> secondaryAgentVertices = {
            secondaryAttr.vertices.v0,
            secondaryAttr.vertices.v1,
            secondaryAttr.vertices.v2,
            secondaryAttr.vertices.v3,
            secondaryAttr.vertices.v0
        };

        /* compute all points belonging to secondary agent
        */
        std::vector<std::pair<int, int>> secondaryPoints;
        for(int j = 0; j < 4; j++){
            std::vector<std::pair<int, int>> points = connectTwoCells(
                {secondaryAgentVertices[j].x, secondaryAgentVertices[j].y},
                {secondaryAgentVertices[j + 1].x, secondaryAgentVertices[j + 1].y}
            );
            secondaryPoints.insert(secondaryPoints.end(),
                                   points.begin(),
                                   points.end());
        }
        /* check if "point" belongs to secondaryPoints
        */
        for(int j = 0; j < secondaryPoints.size(); j++){
            int px = secondaryPoints[j].first;
            int py = secondaryPoints[j].second;

            if(px == point.first && py == point.second)
                return secondaryAttr.id;
        }
    }
    return -1;
}

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
collisionInfo_t EnvironmentClass::detectCollision(agentAttribute_t attr){
    collisionInfo_t info;
    std::vector<std::pair<int, int> > points;
    int px, py;

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
                info.isCollision = true;
                /* get agent from {px, py}
                */
                info.agentIDs.push_back(getCollidingAgent(attr, {px, py}));
                info.points.push_back({(float)px, (float)py});
                /* calculate normal = slope - 90 degrees in radians
                */
                info.normals.push_back(slope - 1.570);
            }
        }
    }

#if 1
    if(info.isCollision){
        std::cout<<"[DEBUG] Collision info for agent id "<<attr.id<<std::endl;
        for(int i = 0; i < info.points.size(); i++){
            std::cout<<info.agentIDs[i]<<" "
                    <<info.points[i].x<<","<<info.points[i].y<<" "
                    <<info.normals[i]<<std::endl;
        }
    }
#endif
    return info;
}

/* NOTE: The obvious thing to do for collision response is to apply a force to both objects, but 
 * that doesn’t actually do the job for rigid bodies. A force won’t stop the bodies from 
 * interpenetrating because a force can’t instantaneously change a velocity. That is, a force 
 * takes time to change a velocity — it can only do so via integration over time.
 * 
 * In the case of a rigid body collision, however, we must change the velocities instantaneously. 
 * That calls for a new quantity: the “impulse.” You can think of an impulse as a really huge 
 * force integrated over a really short period of time. We can calculate and apply impulses at 
 * the point and instant of collision, and these impulses will change the bodies’ velocities and 
 * prevent them from interpenetrating.
 * 
 * The collision model we’ll use is called “Newton’s Law of Restitution for Instantaneous 
 * Collisions with No Friction.” The model assumes the collision process takes no time. Since 
 * “no time” is a very small amount of time, all of our regular noncollision forces go away 
 * during the collision, and only the collision impulses are calculated. Thus, noncollision forces
 * such as gravity are not taken into account during the collision, although they’re in effect as 
 * usual before and after the collision. The model also assumes that there is no friction at the 
 * point of collision. Thus, the impulse generated by the collision is entirely in the normal 
 * direction n (there’s no tangential impulse at all).
*/
vector2f_t EnvironmentClass::collisionResponse(agentAttribute_t attr){

}