#ifndef _FLARE_H_
#define _FLARE_H_

#include "VectorUtils3.h"

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
} Enemy;


void initEnemy(Enemy* this, mat4 projectionMatrix);
Enemy* createEnemy (mat4 projectionMatrix);
void drawEnemy(Enemy* enemy, mat4 projectionMatrix);
void displayEnemy(Enemy* enemy, mat4 * wtvMatrixp, mat4 trans, mat4 rot1);
void MoveEnemy(Enemy* enemy);

#endif
