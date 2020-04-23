#include "FlashLight.h"
#include "GL_utilities.h"
#include <math.h>

void initFlashLight (FlashLight * FlashLight, User* user) {
  FlashLight__updatePosition(FlashLight, user);
  FlashLight__updateDirection(FlashLight, user);
  FlashLight->cutOffAngle = cos(M_PI/13);
  FlashLight->outerCutOff = cos(M_PI/9);
  FlashLight->shader = loadShaders("shaders/flashlight.vert", "shaders/flashlight.frag");
  FlashLight->model = LoadModelPlus("models/flashlight.obj");
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

void drawFlashlight(FlashLight* flashlight, mat4 projectionMatrix){
  glUseProgram(flashlight->shader);
	glUniformMatrix4fv(glGetUniformLocation(flashlight->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glEnableVertexAttribArray(glGetAttribLocation(flashlight->shader, "projectionMatrix"));
};

void displayFlashlight(FlashLight* flashlight, mat4 * wtvMatrixp, mat4 trans, mat4 rot1) {
  glUseProgram(flashlight->shader);
  mat4 wtvMatrix = *wtvMatrixp;
  glUniformMatrix4fv(glGetUniformLocation(flashlight->shader, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);
  mat4 total;
  total = Mult(trans, rot1);
  glUniformMatrix4fv(glGetUniformLocation(flashlight->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(flashlight->model, flashlight->shader, "in_Position", "in_Normal", "inTexCoord");
};
