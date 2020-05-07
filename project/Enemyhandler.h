#ifndef _ENEMYHANDLER_H_
#define _ENEMYHANDLER_H_

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
void diaplayEnemies (EnemyHandler*, mat4 *camMatrix, User *);
void CheckForNewEnemies (EnemyHandler*, User *,Terrain *, mat4);
void MoveAllEnemies(EnemyHandler*, User *, Terrain * );
void RemoveEnemies(EnemyHandler*);
void CheckEnemiesCollisions (EnemyHandler*, Terrain *, Terrain *);
void displayEnemiesLight (EnemyHandler*, Terrain * terrain);

#endif
