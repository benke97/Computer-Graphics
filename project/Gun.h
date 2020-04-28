#ifndef _GUN_H_
#define _GUN_H_

#include "VectorUtils3.h"
#include "User.h"

typedef struct Gun
{
  vec3 position;
  vec3 direction;
  Model *model;
  GLuint shader;
  GLuint texture;
} Gun;

void initGun(Gun* gun, User* user);
Gun * createGun(User* user);
void Gun__updateDirection(Gun* gun, User* user);
void Gun__updatePosition(Gun* gun, User* user);
void drawGun(Gun* gun, mat4 projectionMatrix);
void displayGun(Gun* gun, mat4 * wtvMatrixp, float trans, float rot1);



#endif
