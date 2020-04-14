#ifndef _FLASHLIGHT_H_
#define _FLASHLIGHT_H_

#include "VectorUtils3.h"
#include "User.h"

typedef struct FlashLight
{
  vec3 position;
  vec3 direction;
  GLfloat cutOffAngle;
  GLfloat outerCutOff;
} FlashLight;

void initFlashLight (FlashLight * FlashLight, User* user);
FlashLight * createFlashLight(User* user);
void FlashLight__updateDirection(FlashLight* flashlight, User* user);
void FlashLight__updatePosition(FlashLight* flashlight, User* user);




#endif
