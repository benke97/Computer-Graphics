#include "Gun.h"
#include "GL_utilities.h"
#include <math.h>

void initGun(Gun* gun, User* user) {
  Gun__updatePosition(gun, user);
  Gun__updateDirection(gun, user);
  gun->shader = loadShaders("shaders/gun.vert", "shaders/gun.frag");
  gun->model = LoadModelPlus("models/XCom.obj");
}

Gun * createGun(User* user){
  Gun* result = (Gun*) malloc(sizeof(Gun));
  initGun(result, user);
  return result;
}

void Gun__updateDirection(Gun* gun, User* user)
{
  gun->direction = VectorSub(user->lookAtPoint, user->cam);
}

void Gun__updatePosition(Gun* gun, User* user)
{
  vec3 sideOffset = ScalarMult(Normalize(user->side_movement), -0.3);
  vec3 yOffset = {0, 0.3, 0};
	vec3 posOffset = VectorSub(VectorSub(user->cam, yOffset), sideOffset);
  gun->position = posOffset;
}

void drawGun(Gun* gun, mat4 projectionMatrix){
  glUseProgram(gun->shader);
	glUniformMatrix4fv(glGetUniformLocation(gun->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glEnableVertexAttribArray(glGetAttribLocation(gun->shader, "projectionMatrix"));
};

void displayGun(Gun* gun, mat4 * wtvMatrixp, mat4 trans, mat4 rot1) {
  glUseProgram(gun->shader);
  mat4 wtvMatrix = *wtvMatrixp;
  glUniformMatrix4fv(glGetUniformLocation(gun->shader, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);
  mat4 total;
  total = Mult(trans, rot1);
  glUniformMatrix4fv(glGetUniformLocation(gun->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(gun->model, gun->shader, "in_Position", "in_Normal", "inTexCoord");
};
