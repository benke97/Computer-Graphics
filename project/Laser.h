#ifndef _LASER_H_
#define _LASER_H_

#include "VectorUtils3.h"
#include "User.h"
#include "Gun.h"

typedef struct Laser
{
  vec3 position;
  vec3 direction;
  Model *model;
  GLuint shader;
  float speed;
  bool flying;
} Laser;

void initLaser(Laser* laser, Gun* gun);
Laser * createLaser(Gun* gun);
void Laser__setDirection(Laser* laser, Gun* gun);
void Laser__setStartPosition(Laser* laser, Gun* gun);
void Laser__updatePosition(Laser* laser);
void displayLaser(Laser* laser, mat4 * wtvMatrixp, mat4 trans, mat4 rot1, mat4 projectionMatrix);

#endif
