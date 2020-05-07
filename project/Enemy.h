#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "VectorUtils3.h"
#include "Terrain.h"
#include "loadobj.h"
#include "LoadTGA.h"
//typedef struct {} Terrain;

typedef struct Enemy {
  Model *model;
  GLuint shader;
  vec3 position;
  vec3 direction;
  bool active;
  bool flying;
  float intensity;
  float lifeTimer;
  GLuint texture;
} Enemy;


void initEnemy(Enemy* this, mat4 projectionMatrix, GLuint enemyShader);
Enemy* createEnemy (mat4 projectionMatrix, GLuint enemyShader);
void drawEnemy(Enemy* enemy, mat4 projectionMatrix);
void displayEnemy(Enemy* enemy, mat4 * wtvMatrixp, mat4 trans, mat4 rot1);
void MoveEnemy(Enemy* enemy, vec3 userPos, Terrain *);

#endif
