#ifndef _FLASHLIGHT_H_
#define _FLASHLIGHT_H_

#include "VectorUtils3.h"

typedef struct FlashLight
{
  vec3* position;
  vec3* direction;
  GLfloat cutOffAngle;
} FlashLight;

void initFlashLight (FlashLight * FlashLight, vec3* pos, vec3* dir);
FlashLight * createFlashLight(vec3* pos, vec3* dir);
void FlashLight__setDirection(FlashLight* flashlight, vec3* dir);




#endif
