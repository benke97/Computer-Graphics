#include "FlashLight.h"
#include "GL_utilities.h"
#include <math.h>

void initFlashLight (FlashLight * FlashLight, User* user) {
  FlashLight__updatePosition(FlashLight, user);
  FlashLight__updateDirection(FlashLight, user);
  FlashLight->cutOffAngle = cos(M_PI/13);
  FlashLight->outerCutOff = cos(M_PI/9);
}

FlashLight * createFlashLight(User* user){
  FlashLight* result = (FlashLight*) malloc(sizeof(FlashLight));
  initFlashLight(result, user);

  return result;
}

void FlashLight__updateDirection(FlashLight* flashlight, User* user)
{
  flashlight->direction = VectorSub(user->lookAtPoint, user->cam);
}

void FlashLight__updatePosition(FlashLight* flashlight, User* user)
{
  vec3 sideOffset = ScalarMult(Normalize(user->side_movement), 0.3);
  vec3 yOffset = {0, 0.3, 0};
	vec3 posOffset = VectorSub(VectorSub(user->cam, yOffset), sideOffset);
  flashlight->position = posOffset;
}
