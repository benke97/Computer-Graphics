#ifndef _FLARE_H_
#define _FLARE_H_

#include "VectorUtils3.h"

#include "loadobj.h"
#include "LoadTGA.h"
//typedef struct {} Terrain;

typedef struct Flare {
  Model *model;
  GLuint shader;
  vec3 position;
  vec3 direction;
  bool active;
  bool flying;
  float intensity;
  float lifeTimer;
} Flare;


void initFlare(Flare* this, mat4 projectionMatrix);
Flare* createFlare (mat4 projectionMatrix);
void drawFlare(Flare* flare, mat4 projectionMatrix);
void displayFlare(Flare* flare, mat4 * wtvMatrixp, mat4 trans, mat4 rot1);
void MoveFlare(Flare* flare);

#endif
