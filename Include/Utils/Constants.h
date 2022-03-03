#ifndef UTILS_CONSTANTS_H
#define UTILS_CONSTANTS_H

/* _N       : grid dimension NxN
 * _scale   : screen scale factor
 * _dt      : simulation timestep (s)
 * _g       : gravity (m/s2)
 * _d       : atmosphere density (kg/m3)
*/
const int _N = 700;
const int _scale = 1;
const float _dt = 0.01;
const float _g = -9.8;
const float _d = 1.2;

/* macros
 * ENV_SCENE_?  : environment obstacle scenario
 * DEBUG_PRINT  : enable/disable debug print
*/
#define ENV_SCENE_0     1
#define DEBUG_PRINT     1

#endif /* UTILS_CONSTANTS_H
*/