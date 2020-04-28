#include "Laser.h"
#include "GL_utilities.h"
#include "Gun.h"
#include <math.h>

void initLaser(Laser* laser, Gun* gun) {
  Laser__setDirection(laser, gun);
  Laser__setStartPosition(laser, gun);
  laser->shader = loadShaders("shaders/laser.vert", "shaders/laser.frag");
  laser->model = LoadModelPlus("models/lasershot.obj");
  laser->speed = 4;
  laser->flying = true;
  laser->active = true;
  mat4 rot, rot1, rot2;
  rot1 = Ry(-angle+M_PI/2);
  rot2 = Rx(-yangle+M_PI/2);
  rot = Mult(rot1,rot2);
  laser->orientation = rot;
}

Laser * createLaser(Gun* gun){
  Laser* result = (Laser*) malloc(sizeof(Laser));
  initLaser(result, gun);
  return result;
}

void Laser__setDirection(Laser* laser, Gun* gun)
{
  laser->direction = gun->direction;
}

void Laser__setStartPosition(Laser* laser, Gun* gun)
{
  laser->position = VectorSub(gun->position,gun->direction);
}

void Laser__updatePosition(Laser* laser)
{
  laser->position = VectorAdd(laser->position, ScalarMult(laser->direction,laser->speed));
}

void displayLaser(Laser* laser, mat4 * wtvMatrixp, mat4 trans, mat4 rot1, mat4 projectionMatrix)
{
  glUseProgram(laser->shader);
  glUniformMatrix4fv(glGetUniformLocation(laser->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glEnableVertexAttribArray(glGetAttribLocation(laser->shader, "projectionMatrix"));
  mat4 wtvMatrix = *wtvMatrixp;
  glUniformMatrix4fv(glGetUniformLocation(laser->shader, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);
  mat4 total;
  total = Mult(trans, rot1);
  glUniformMatrix4fv(glGetUniformLocation(laser->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(laser->model, laser->shader, "in_Position", "in_Normal", "inTexCoord");
}
