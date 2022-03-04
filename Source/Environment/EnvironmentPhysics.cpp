#include "../../Include/Environment/Environment.h"

vector2f_t EnvironmentClass::computeDragForce(agentAttribute_t attr){
    /* drag coefficient
    */
    const float cD = 0.1;
    /* precompute k for ease of calculation
    */
    const float k = 0.5 * cD * envAttribute.density;
    /* aX   : traversal area in x direction
     * aY   : traversal area in y direction
    */
    const float aX = attr.height;
    const float aY = attr.width;
    /* we need velocity to compute density and the magnitude of velocity, since drag acts opposite
     * to velocity vector
    */
    float vX = attr.velocityCenterOfMass.x;
    int vXMag = vX < 0 ? -1 : 1;

    float vY = attr.velocityCenterOfMass.y;
    float vYMag = vY < 0 ? -1 : 1;

    return {k * aX * vX * vX * -1 * vXMag, 
            k * aY * vY * vY * -1 * vYMag};
}

vector2f_t EnvironmentClass::computeNormalForce(agentAttribute_t attr){
    return {0, 0};
}

vector2f_t EnvironmentClass::computeNetForce(agentAttribute_t attr){
    /* accl, vel due to gravity
    */
    vector2f_t fG = {0, attr.mass * envAttribute.gravity};
    /* normal force based on collision
    */
    vector2f_t fN = computeNormalForce(attr);

    /* drag force
     * NOTE: When the object is first released, drag is small as velocity is low, so the 
     * resultant force is down. This means the object accelerates towards earth. As the object 
     * gains speed, drag increases until it equals the magnitude of the force down due to gravity. 
     * At this point, resultant force is zero and so the object stops accelerating, and maintains 
     * the same velocity. This is called terminal velocity.
    */
    vector2f_t fD = {0, 0}; //computeDragForce(attr);

    /* sum up all forces acting on the agent
    */
    vector2f_t cumulativeForce = {0.0, 0.0};
    cumulativeForce.x = fG.x + fN.x + fD.x;
    cumulativeForce.y = fG.y + fN.y + fD.y;

    return cumulativeForce;
}

vector2f_t EnvironmentClass::computeNetTorque(agentAttribute_t attr){
    /* For each force, form the perpdot product from the CM to the point of force application and 
     * add the value into the total torque at the CM. So, we need force magnitude and the point
     * of application
    */

    vector2f_t cumulativeTorque = {100.0, 10.0};
    return cumulativeTorque;
}

void EnvironmentClass::integrateAndUpdate(vector2f_t linearAccel, vector2f_t angularAccel, 
agentAttribute_t attr){
    /* lin. vel = d(lin. accel)/dt
    */
    attr.velocityCenterOfMass.x += linearAccel.x * envAttribute.dt;
    attr.velocityCenterOfMass.y += linearAccel.y * envAttribute.dt;
    /* pos = d(lin. vel)/dt
    */
    attr.positionCenterOfMass.x += attr.velocityCenterOfMass.x * envAttribute.dt;
    attr.positionCenterOfMass.y += attr.velocityCenterOfMass.y * envAttribute.dt;
    /* angl. vel = d(angl. accel)/dt
    */
    attr.angularVelocity.x += angularAccel.x * envAttribute.dt;
    attr.angularVelocity.y += angularAccel.y * envAttribute.dt;
    /* orientation = d(angl. vel)/dt
    */
    attr.orientation.x += attr.angularVelocity.x * envAttribute.dt;
    attr.orientation.y += attr.angularVelocity.y * envAttribute.dt;
    /* update vertices
    */
    attr.vertices = computeBoundingVertices(attr);
    /* update agent map
    */
    agentMap[attr.id] = attr;
}
