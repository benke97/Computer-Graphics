#include "Enemy.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initEnemy(Enemy* this, mat4 projectionMatrix) {

  this->shader = loadShaders("shaders/enemy.vert", "shaders/enemy.frag");
  glUseProgram(this->shader);
  printError("init enemy shader");
  this->position = SetVector(0,0,0);
  this->active = true;
  this->flying = true;
  this->model = LoadModelPlus("models/FatTerrorist.obj");
  this->intensity = 1.0;
  this->lifeTimer = 20;
  LoadTGATextureSimple("textures/FatTerrorist.tga", &this->texture);
  //LoadTGATextureSimple("textures/conc.tga", &this->texture);


  glUniformMatrix4fv(glGetUniformLocation(this->shader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

  printError("init enemy");

  drawEnemy(this, projectionMatrix);
};


Enemy* createEnemy(mat4 projectionMatrix) {
  Enemy* result = (Enemy*) malloc(sizeof(Enemy));
  initEnemy(result, projectionMatrix);
  return result;
};


void drawEnemy(Enemy* enemy, mat4 projectionMatrix){
  glUseProgram(enemy->shader);

	glUniformMatrix4fv(glGetUniformLocation(enemy->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glEnableVertexAttribArray(glGetAttribLocation(enemy->shader, "projectionMatrix"));

};



void displayEnemy(Enemy* enemy, mat4 * wtvMatrixp, mat4 trans, mat4 rot1) {
  glUseProgram(enemy->shader);
  //LoadTGATextureSimple("textures/FatTerrorist.tga", &enemy->texture);
  glBindTexture(GL_TEXTURE_2D, enemy->texture);
  mat4 wtvMatrix = *wtvMatrixp;
  glUniformMatrix4fv(glGetUniformLocation(enemy->shader, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);
  mat4 total;
  total = Mult(trans, rot1);
  glUniformMatrix4fv(glGetUniformLocation(enemy->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  glUniform1f(glGetUniformLocation(enemy->shader, "intensity"), enemy->intensity);
  DrawModel(enemy->model, enemy->shader, "in_Position", "in_Normal", "inTexCoord");
};


void MoveEnemy(Enemy* enemy) {
    //enemy->position.x = enemy->position.x + enemy->direction.x / 4;
    //enemy->position.z = enemy->position.z + enemy->direction.z / 4;
    //enemy->position.y = 7.0 + 30*enemy->direction.y * enemy->lifeTimer - 9.82 * pow(enemy->lifeTimer, 2) / 2;
    enemy->lifeTimer += 0.015;
    enemy->intensity -= 0.005;
}
