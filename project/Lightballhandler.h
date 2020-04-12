#ifndef _LIGHTBALLHANDLER_H_
#define _LIGHTBALLHANDLER_H_

#include "VectorUtils3.h"
#include "Lightball.h"
#include "User.h"
#include "loadobj.h"
#include "LoadTGA.h"



typedef struct LightBallHandler {
  vec3 lightBallsColor[100];
  vec3 lightBallsPositions[100];
  LightBall lightballs[100];
  int LightBallsQuantity;
  int timeUntilNextBall;
  int maxDistance;
} LightBallHandler;


void initLightBallHandler(LightBallHandler*);
LightBallHandler* createLightBallHandler();
void RemoveLightBalls(LightBallHandler*);
void MoveAllLightBalls(LightBallHandler*, mat4*);
void CheckForNewLightBalls (LightBallHandler*, User *, mat4);

#endif
