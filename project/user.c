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

	user->cam.y = heightFinder(user->cam.x, user->cam.z, floor) + 2;

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
	vec3 upVector = {0.0, 1.0, 0.0};
	vec3 normel = {0.0, 1.0, 0.0};
	GLfloat slope = 0.0;
	float slopethreshold = 0.4;
	switch(move)
	{
		case(1): //framåt "w"
			a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
			tmp = VectorSub(user->cam, a);
			if(slidedown(tmp.x,tmp.z,user->cam.x, user->cam.z, floor))
			{
			 return true;
			}
			normel = getNormal(tmp.x,tmp.z, floor);
			slope = DotProduct(normel,upVector);
			printf("slope %f\n", slope);
			//vec3 tmp2 = VectorSub(user->cam, ScalarMult(a,10));
			//&& slope(user->cam.x,user->cam.y,tmp2.x,tmp2.y,floor->texwidth, roof) < 2
			if (heightdiff(tmp.x, tmp.z, roof, floor) > 3.0 && slope > slopethreshold)
			{
				return true;
			}
			else
			{
				return false;
			}
		break;

		case(2): //bakåt "s"
			a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.1);
			tmp = VectorAdd(user->cam, a);
			if(slidedown(tmp.x,tmp.z,user->cam.x, user->cam.z, floor))
			{
			 return true;
			}
			normel = getNormal(tmp.x,tmp.z,floor);
			slope = DotProduct(normel,upVector);
			if (heightdiff(tmp.x, tmp.z, roof, floor) > 3.0 && slope > slopethreshold)
			{
				return true;
			}
			else
			{
				return false;
			}
		break;

		case(3): //höger "d"
			tmp = VectorSub(user->cam, user->side_movement);
			if(slidedown(tmp.x,tmp.z,user->cam.x, user->cam.z, floor))
			{
			 return true;
			}
			normel = getNormal(tmp.x,tmp.z,floor);
			slope = DotProduct(normel,upVector);
			if (heightdiff(tmp.x, tmp.z, roof, floor) > 3.0 && slope > slopethreshold)
			{
				return true;
			}
			else
			{
				return false;
			}
		break;

		case(4): //vänster "a"
			tmp = VectorAdd(user->cam, user->side_movement);
			if(slidedown(tmp.x,tmp.z,user->cam.x, user->cam.z, floor))
			{
			 return true;
			}
			normel = getNormal(tmp.x,tmp.z,floor);
			slope = DotProduct(normel,upVector);
			if (heightdiff(tmp.x, tmp.z, roof, floor) > 3.0 && slope > slopethreshold)
			{
				return true;
			}
			else
			{
				return false;
			}
		break;

		case(5): //snabb framåt "shift + w"
			a = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.5);
			tmp = VectorSub(user->cam, a);
			if(slidedown(tmp.x,tmp.z,user->cam.x, user->cam.z, floor))
			{
			 return true;
			}
			normel = getNormal(tmp.x,tmp.z, floor);
			slope = DotProduct(normel,upVector);
			if (heightdiff(tmp.x, tmp.z, roof, floor) > 3.0 && slope > slopethreshold)
			{
				return true;
			}
			else
			{
				return false;
			}
		break;
	}

	return false;
}
