#include "FlashLight.h"
#include "GL_utilities.h"

void initFlashLight (FlashLight * FlashLight, vec3* pos, vec3* dir) {
  FlashLight->position = pos;
  FlashLight->direction = dir;
  FlashLight->cutOffAngle = cos(12.5f);
}

FlashLight * createFlashLight(vec3* pos, vec3* dir){
  FlashLight* result = (FlashLight*) malloc(sizeof(FlashLight));
  initFlashLight(result, pos, dir);

  return result;
}
