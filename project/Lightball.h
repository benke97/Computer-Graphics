#ifndef _LIGHTBALL_H_
#define _LIGHTBALL_H_

#include "VectorUtils3.h"

#include "loadobj.h"
#include "LoadTGA.h"
//typedef struct {} Terrain;

typedef struct LightBall {
  GLuint texture; // terrain
  Model *model;
  GLuint shader;
  vec3 position;
  vec3 direction;
  bool active;
} LightBall;


void initLightBall(LightBall* this, mat4 projectionMatrix);
LightBall* createLightBall(mat4 projectionMatrix);
void drawLightBall(LightBall* lightball, mat4 projectionMatrix);
void displayLightBall(LightBall* lightball, mat4 wtvMatrix, mat4 trans, mat4 rot1);
void MoveLightBall(LightBall* lightball);
#endif
