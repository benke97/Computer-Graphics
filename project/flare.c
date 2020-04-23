#include "Flare.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initFlare(Flare* this, mat4 projectionMatrix) {

  this->shader = loadShaders("shaders/flare.vert", "shaders/flare.frag");
  glUseProgram(this->shader);
  printError("init flare shader");
  this->position = SetVector(0,0,0);
  this->active = true;
  this->flying = true;
  this->model = LoadModelPlus("models/octagon.obj");
  this->intensity = 1.0;
  this->lifeTimer = 0;

  glUniformMatrix4fv(glGetUniformLocation(this->shader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

  printError("init flare");

  drawFlare(this, projectionMatrix);
};


Flare* createFlare(mat4 projectionMatrix) {
  Flare* result = (Flare*) malloc(sizeof(Flare));
  initFlare(result, projectionMatrix);
  return result;
};


void drawFlare(Flare* flare, mat4 projectionMatrix){
  glUseProgram(flare->shader);
	glUniformMatrix4fv(glGetUniformLocation(flare->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glEnableVertexAttribArray(glGetAttribLocation(flare->shader, "projectionMatrix"));
};



void displayFlare(Flare* flare, mat4 * wtvMatrixp, mat4 trans, mat4 rot1) {
  glUseProgram(flare->shader);
  mat4 wtvMatrix = *wtvMatrixp;
  glUniformMatrix4fv(glGetUniformLocation(flare->shader, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);
  mat4 total;
  total = Mult(trans, rot1);
  glUniformMatrix4fv(glGetUniformLocation(flare->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  glUniform1f(glGetUniformLocation(flare->shader, "intensity"), flare->intensity);
  DrawModel(flare->model, flare->shader, "in_Position", "in_Normal", "inTexCoord");
};


void MoveFlare(Flare* flare) {
    flare->position.x = flare->position.x + flare->direction.x / 4;
    flare->position.z = flare->position.z + flare->direction.z / 4;
    flare->position.y = 7.0 + 30*flare->direction.y * flare->lifeTimer - 9.82 * pow(flare->lifeTimer, 2) / 2;
    flare->lifeTimer += 0.015;
    flare->intensity -= 0.005;
}
