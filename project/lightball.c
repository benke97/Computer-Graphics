#include "Lightball.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initLightBall(LightBall* this, mat4 projectionMatrix) {

  this->shader = loadShaders("shaders/lightball.vert", "shaders/lightball.frag");
  glUseProgram(this->shader);
  printError("init lightball shader");
  this->position = SetVector(0,0,0);
  this->active = true;
  this->flying = true;
  this->model = LoadModelPlus("models/groundsphere.obj");
  this->intensity = 1;
  this->lifeTimer = 20;

  glUniformMatrix4fv(glGetUniformLocation(this->shader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

  printError("init lightball");

  drawLightBall(this, projectionMatrix);
};


LightBall* createLightBall(mat4 projectionMatrix) {
  LightBall* result = (LightBall*) malloc(sizeof(LightBall));
  initLightBall(result, projectionMatrix);
  return result;
};

void drawLightBall(LightBall* lightball, mat4 projectionMatrix){
  glUseProgram(lightball->shader);
	glUniformMatrix4fv(glGetUniformLocation(lightball->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glEnableVertexAttribArray(glGetAttribLocation(lightball->shader, "projectionMatrix"));
};

void displayLightBall(LightBall* lightball, mat4 * wtvMatrixp, mat4 trans, mat4 rot1) {
  glUseProgram(lightball->shader);
  mat4 wtvMatrix = *wtvMatrixp;
  glUniformMatrix4fv(glGetUniformLocation(lightball->shader, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);
  mat4 total;
  total = Mult(trans, rot1);
  glUniformMatrix4fv(glGetUniformLocation(lightball->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(lightball->model, lightball->shader, "in_Position", "in_Normal", "inTexCoord");
};


void MoveLightBall(LightBall* lightball) {
    lightball->position = VectorAdd(lightball->position, lightball->direction);
}
