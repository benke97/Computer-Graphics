#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#define GLUT_KEY_LEFT_SHIFT GLUT_KEY_TAB
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
	user->speed = 1;
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


void userInput(User * user, Terrain * roof, Terrain * floor)
{
  user->side_movement = ScalarMult(Normalize(CrossProduct(VectorSub(user->cam, user->lookAtPoint),user->upVector)), 0.1);
	if (glutKeyIsDown('s'))
	{
		if (validMove(user, roof, floor,2))
		{
			vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
			user->cam = VectorAdd(user->cam, a);
		}
	}
	if (glutKeyIsDown('w'))
	{
		if (glutKeyIsDown(GLUT_KEY_LEFT_SHIFT))
		{
			if (validMove(user, roof, floor,5))
			{
				vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.5);
				user->cam = VectorSub(user->cam, a);
			}
			else
			{
				if (validMove(user, roof, floor,1))
				{
					vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
					user->cam = VectorSub(user->cam, a);
				}
			}
		}
		else
		{
			if (validMove(user, roof, floor,1))
			{
				vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
				user->cam = VectorSub(user->cam, a);
			}
		}
		/*	vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.5);
			user->cam = VectorSub(user->cam, a);*/
	}
	if (glutKeyIsDown('a'))
	{
		if (validMove(user, roof, floor,4))
		{
			user->lookAtPoint = VectorAdd(user->lookAtPoint ,user->side_movement);
			user->cam = VectorAdd(user->cam, user->side_movement);
		}
	}
	if (glutKeyIsDown('d'))
	{
		if (validMove(user, roof, floor,3))
		{
			user->lookAtPoint = VectorSub(user->lookAtPoint,user->side_movement);
			user->cam = VectorSub(user->cam, user->side_movement);
		}
	}

	user->cam.y = heightFinder(user->cam.x, user->cam.z, floor->texwidth, floor) + 2;

	user->lookAtPoint.x = user->cam.x + cos(angle)*cos(yangle);
	user->lookAtPoint.z = user->cam.z + sin(angle)*cos(yangle);
	user->lookAtPoint.y = user->cam.y + sin(yangle);

	//For LightBall:

	if (glutKeyIsDown('f'))
	{
		user->lightball_shooting_activated = true;
	}
}

bool validMove(User* user, Terrain* roof, Terrain* floor,int move)
{
	vec3 a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
	vec3 tmp = VectorSub(user->cam, a);
	switch(move)
	{
		case(1): //framåt "w"
			a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
			tmp = VectorSub(user->cam, a);
			if (heightdiff(tmp.x, tmp.z, roof->texwidth, roof, floor) < 3.0)
			{
				return false;
			}
			else
			{
				return true;
			}
		break;

		case(2): //bakåt "s"
			a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
			tmp = VectorAdd(user->cam, a);
			if (heightdiff(tmp.x, tmp.z, roof->texwidth, roof, floor) < 3.0)
			{
				return false;
			}
			else
			{
				return true;
			}
		break;

		case(3): //höger "d"
			tmp = VectorSub(user->cam, user->side_movement);
			if (heightdiff(tmp.x, tmp.z, roof->texwidth, roof, floor) < 3.0)
			{
				return false;
			}
			else
			{
				return true;
			}
		break;

		case(4): //vänster "a"
			tmp = VectorAdd(user->cam, user->side_movement);
			if (heightdiff(tmp.x, tmp.z, roof->texwidth, roof, floor) < 3.0)
			{
				return false;
			}
			else
			{
				return true;
			}
		break;

		case(5): //snabb framåt "shift + w"
			a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.5);
			tmp = VectorSub(user->cam, a);
			if (heightdiff(tmp.x, tmp.z, roof->texwidth, roof, floor) < 3.0)
			{
				return false;
			}
			else
			{
				return true;
			}
		break;
	}
	/*
	//bakåt "s"
		case 2:
			break;
		//vänster "a"
		case 3:
			break;
		//höger "d"
		case 4:
			break;
		//snabb fram "w+shift"
		case 5:
		break;*/
}
