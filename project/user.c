#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "User.h"
#include "MicroGlut.h"
#include "GL_utilities.h"

#include "Terrain.h"

float angle = 0;
float yangle = 0;

User * createUser(){
  User* result = (User*) malloc(sizeof(User));
  initUser(result);

  return result;
}


void initUser (User * user) {

  user->cam = SetVector(25,10, 100);
  user->lookAtPoint = SetVector (200, 20, 200);
  user->upVector = SetVector (0, 1, 0);
  user->side_movement = SetVector (0,0,0);
	user->lightball_shooting_activated = false;
}


void mouse(int x, int y)
{
	//printf("%d %d\n", x, y);
	float diffx = x - (glutGet(GLUT_WINDOW_WIDTH)/2);
	float diffy = y - (glutGet(GLUT_WINDOW_HEIGHT)/2);
	yangle += -diffy/700;

	if (yangle < -M_PI/3)
	{
			yangle = -M_PI/3;
	}
	else if (yangle > M_PI/3)
	{
			yangle = M_PI/3;
	}
	angle += diffx/700;
}


void userInput(User * user, Terrain * terrain)
{
  user->side_movement = ScalarMult(Normalize(CrossProduct(VectorSub(user->cam, user->lookAtPoint),user->upVector)), 0.1);
	if (glutKeyIsDown('s'))
	{
		vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
		user->cam = VectorAdd(user->cam, a);
	}
	if (glutKeyIsDown('w'))
	{
		/*if (glutKeyIsDown(GLUT_KEY_LEFT_SHIFT))
		{
			vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.5);
			user->cam = VectorSub(user->cam, a);
		}
		else
		{
			vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
			user->cam = VectorSub(user->cam, a);
		}*/
			vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.5);
			user->cam = VectorSub(user->cam, a);
	}
	if (glutKeyIsDown('a'))
	{
		user->lookAtPoint = VectorAdd(user->lookAtPoint ,user->side_movement);
		user->cam = VectorAdd(user->cam, user->side_movement);
	}
	if (glutKeyIsDown('d'))
	{
		user->lookAtPoint = VectorSub(user->lookAtPoint,user->side_movement);
		user->cam = VectorSub(user->cam, user->side_movement);
	}

	user->cam.y = heightFinder(user->cam.x, user->cam.z, terrain->texwidth, terrain) + 2;

	user->lookAtPoint.x = user->cam.x + cos(angle)*cos(yangle);
	user->lookAtPoint.z = user->cam.z + sin(angle)*cos(yangle);
	user->lookAtPoint.y = user->cam.y + sin(yangle);

	//For LightBall:

	if (glutKeyIsDown('f'))
	{
		user->lightball_shooting_activated = true;
	}
}
