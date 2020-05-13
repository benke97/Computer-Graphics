#include "Enemy.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initEnemy(Enemy* this, mat4 projectionMatrix, GLuint enemyShader) {

  this->shader = enemyShader;
  glUseProgram(this->shader);
  this->position = SetVector(0,0,0);
  this->active = true;
  this->model = LoadModelPlus("models/FatTerrorist.obj");
  this->lifeTimer = 20;
  LoadTGATextureSimple("textures/FatTerrorist.tga", &this->texture);
  glUniformMatrix4fv(glGetUniformLocation(this->shader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
  drawEnemy(this, projectionMatrix);
};


Enemy* createEnemy(mat4 projectionMatrix, GLuint enemyShader) {
  Enemy* result = (Enemy*) malloc(sizeof(Enemy));
  initEnemy(result, projectionMatrix, enemyShader);
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
  mat4 scale = S(0.02, 0.02, 0.02);
  total = Mult(total,scale);
  glUniformMatrix4fv(glGetUniformLocation(enemy->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  glUniform1f(glGetUniformLocation(enemy->shader, "LifeTimer"), enemy->lifeTimer);
  DrawModel(enemy->model, enemy->shader, "in_Position", "in_Normal", "inTexCoord");
};


void MoveEnemy(Enemy* enemy, vec3 userPos, Terrain * floor) {
    float dx = enemy->position.x - userPos.x;
    float dz = enemy->position.z - userPos.z;
    enemy->position.x = enemy->position.x - dx / 500;
    enemy->position.z = enemy->position.z - dz / 500;
    enemy->position.y = heightFinder(enemy->position.x, enemy->position.z, floor);
}
