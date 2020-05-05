#include "Gun.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"
#include <math.h>

void initGun(Gun* gun, User* user) {
  Gun__updatePosition(gun, user);
  Gun__updateDirection(gun, user);
  gun->shader = loadShaders("shaders/gun.vert", "shaders/gun.frag");
  gun->model = LoadModelPlus("models/XCom.obj");
  gun->overheated = false;
  gun->heat = 0.0;
  //glBindTexture(GL_TEXTURE_2D, gun->texture);
  LoadTGATextureSimple("textures/Laser_Rifle_Diffuse.tga", &gun->texture);
  //LoadTGATextureSimple("textures/conc.tga", &gun->texture);
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

void displayGun(Gun* gun, mat4 * wtvMatrixp, float angle, float yangle) {
  glUseProgram(gun->shader);

  mat4 trans = T(gun->position.x + gun->direction.x/1.5, gun->position.y + gun->direction.y/1.5, gun->position.z + gun->direction.z/1.5);
  mat4 scale = S(0.015,0.015,0.015);
  mat4 tot = Mult(trans,scale);
  mat4 rot1 = Ry(-angle+M_PI/2);
  mat4 rot2 = Rx(-yangle);
  mat4 rot = Mult(rot1,rot2);

  glBindTexture(GL_TEXTURE_2D, gun->texture);
  mat4 wtvMatrix = *wtvMatrixp;
  glUniformMatrix4fv(glGetUniformLocation(gun->shader, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);
  mat4 total;
  total = Mult(tot, rot);
  glUniformMatrix4fv(glGetUniformLocation(gun->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(gun->model, gun->shader, "in_Position", "in_Normal", "inTexCoord");
};
