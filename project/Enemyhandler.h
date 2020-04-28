#ifndef _FLAREHANDLER_H_
#define _FLAREHANDLER_H_

#include "VectorUtils3.h"
#include "Enemy.h"
#include "User.h"
#include "Terrain.h"
#include "loadobj.h"
#include "LoadTGA.h"



typedef struct EnemyHandler {
  vec3 enemiesColor[100];
  vec3 enemiesPositions[100];
  float enemiesIntensities[100];
  Enemy enemies[100];
  int EnemiesQuantity;
  int timeUntilNextEnemy;
  int maxDistance;
} EnemyHandler;


void initEnemyHandler(EnemyHandler*);
EnemyHandler* createEnemyHandler();
void diaplayEnemies (EnemyHandler*, mat4 *camMatrix);
void CheckForNewEnemies (EnemyHandler*, User *, mat4);
void MoveAllEnemies(EnemyHandler*);
void RemoveEnemies(EnemyHandler*);
void CheckEnemiesCollisions (EnemyHandler*, Terrain *, Terrain *);

void displayEnemiesLight (EnemyHandler* lightballhandler, Terrain * terrain);

#endif
