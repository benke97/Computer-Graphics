#ifndef _FLASHLIGHT_H_
#define _FLASHLIGHT_H_

#include "VectorUtils3.h"
#include "User.h"

typedef struct FlashLight
{
  vec3 position;
  vec3 direction;
  Model *model;
  GLuint shader;
  GLfloat cutOffAngle;
  GLfloat outerCutOff;
  GLuint texture;
} FlashLight;

void initFlashLight (FlashLight * FlashLight, User* user);
FlashLight * createFlashLight(User* user);
void FlashLight__updateDirection(FlashLight* flashlight, User* user);
void FlashLight__updatePosition(FlashLight* flashlight, User* user);
void drawFlashlight(FlashLight* flashlight, mat4 projectionMatrix);
void displayFlashlight(FlashLight* flashlight, mat4 * wtvMatrixp, mat4 trans, mat4 rot1);



#endif
