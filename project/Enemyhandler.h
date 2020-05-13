#ifndef _ENEMYHANDLER_H_
#define _ENEMYHANDLER_H_

#include "VectorUtils3.h"
#include "Enemy.h"
#include "User.h"
#include "Terrain.h"
#include "loadobj.h"
#include "LoadTGA.h"



typedef struct EnemyHandler {
  vec3 enemiesPositions[100];
  Enemy enemies[100];

  int EnemiesQuantity;
  int timeUntilNextEnemy;
  
} EnemyHandler;


void initEnemyHandler(EnemyHandler*);
EnemyHandler* createEnemyHandler();
void diaplayEnemies (EnemyHandler*, mat4 *camMatrix, User *);
void CheckForNewEnemies (EnemyHandler*, User *,Terrain *, mat4, GLuint enemyShader);
void MoveAllEnemies(EnemyHandler*, User *, Terrain * );
void RemoveEnemies(EnemyHandler*);

#endif
