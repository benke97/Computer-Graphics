#ifndef _COLLISIONHANDLER_H_
#define _COLLISIONHANDLER_H_

#include "VectorUtils3.h"
#include "Enemyhandler.h"
#include "Laserhandler.h"
#include "Lightballhandler.h"
#include "loadobj.h"
#include "LoadTGA.h"



typedef struct CollisionHandler {
  vec3 * enemiesPositions[100];
  vec3 * lightballsPositions[100];
  vec3 * laserPositions[100];
} CollisionHandler;

CollisionHandler* createCollisionHandler();
void checkCollisionHandler(LightBallHandler* lightballhandler, EnemyHandler* enemyhandler, LaserHandler* laserhandler, User * user);

#endif
