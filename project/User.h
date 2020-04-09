#ifndef _USER_H_
#define _USER_H_
#include "VectorUtils3.h"
#include "Terrain.h"

typedef struct User {
  vec3 cam;
  vec3 lookAtPoint;
  vec3 upVector;
  vec3 side_movement;
  bool lightball_shooting_activated;
} User;

void mouse(int x, int y);
float angle;
float yangle;

void userInput(User *, Terrain * );
User * createUser ();
void initUser (User * user);

#endif
