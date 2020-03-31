#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "terrain.c"

#include <stdio.h>
mat4 projectionMatrix;
float heightFinder(float xPos, float zPos, int texwith);






// vertex array object
Model *m, *m2;
// Reference to shader program
Terrain* terrain;
GLuint tex2;

mat4 Rot, Trans;
GLfloat t;

void init(void)
{
	// GL inits
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 5000.0);
	glClearColor(0.859,0.957,0.998,1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");
	printf("%d", 5);
	terrain = createTerrain(projectionMatrix);


	// Load and compile shader

}


vec3 cam = {25,10, 100};
vec3 lookAtPoint = {200, 20, 200};
vec3 upVector = {0, 1, 0};
vec3 side_movement = {0,0,0};
float angle = 0;
float yangle = 0;
void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 total, modelView, camMatrix;

	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				upVector.x, upVector.y, upVector.z);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);

	// ###### Skybox ###############
	printError("pre display");

	// ###### TERRAIN #################
	glUseProgram(terrain->shader);
	// Build matrix
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(terrain->shader, "mdlMatrix"), 1, GL_TRUE, total.m);

	//glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(terrain->tm, terrain->shader, "inPosition", "inNormal", "inTexCoord");
	printError("display 2");
	glutSwapBuffers();
}


//########################################
// ######## Declarations################
// ########################################
//########################################
// ########  ################
// ########################################



//########################################
// ######## FUNKCTIONS ################
// ########################################

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

float heightFinder(float xPos, float zPos, int texwidth)
{
  int x = (int)xPos;
	int z = (int)zPos;

	//GLfloat * vertexArray = terrain->vertexArray;
	//int texwidth = terrain->texwidth;

	xPos = fmod(xPos, 1.0f);
	zPos = fmod(zPos, 1.0f);

	vec3 v1 = {0.0, 0.0, 0.0};
	vec3 v2 = {0.0, 0.0, 0.0};
	vec3 v3 = {0.0, 0.0, 0.0};

	if (xPos + zPos > 1)
	{

		v1.x = 1;
		v1.y = vertexArray[((x+1) + z * texwidth)*3 + 1];
		v1.z = 0;

		v2.x = 1;
		v2.y = vertexArray[((x+1) + (z+1) * texwidth)*3 + 1];
		v2.z = 1;

		v3.x = 0;
		v3.y = vertexArray[(x + (z+1) * texwidth)*3 + 1];
		v3.z = 1;
	}
	else
	{

		v1.x = 1;
		v1.y = vertexArray[((x+1) + z * texwidth)*3 + 1];
		v1.z = 0;

		v2.x = 0;
		v2.y = vertexArray[(x + z * texwidth)*3 + 1];
		v2.z = 0;

		v3.x = 0;
		v3.y = vertexArray[(x + (z+1) * texwidth)*3 + 1];
		v3.z = 1;
	}

	// Barycentric
	float w1 = ((v2.z - v3.z)*(xPos - v3.x) + (v3.x - v2.x)*(zPos - v3.z))
			/ ((v2.z - v3.z)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.z - v3.z));

	float w2 = ((v3.z - v1.z)*(xPos - v3.x) + (v1.x - v3.x)*(zPos - v3.z))
			/ ((v2.z - v3.z)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.z - v3.z));

	float w3 = 1.0 - w2 - w1;

	return (w1*v1.y + w2*v2.y + w3*v3.y);
}

//########################################
// ########  ################
// ########################################

void timer(int i)
{
	side_movement = ScalarMult(Normalize(CrossProduct(VectorSub(cam, lookAtPoint),upVector)), 0.1);
	if (glutKeyIsDown('s'))
	{
		vec3 a = ScalarMult(Normalize(VectorSub(cam, lookAtPoint)), 0.1);
		cam = VectorAdd(cam, a);
	}
	if (glutKeyIsDown('w'))
	{
		if (glutKeyIsDown(GLUT_KEY_LEFT_SHIFT))
		{
			vec3 a = ScalarMult(Normalize(VectorSub(cam, lookAtPoint)), 0.5);
			cam = VectorSub(cam, a);
		}
		else
		{
			vec3 a = ScalarMult(Normalize(VectorSub(cam, lookAtPoint)), 0.1);
			cam = VectorSub(cam, a);
		}
	}
	if (glutKeyIsDown('a'))
	{
		lookAtPoint = VectorAdd(lookAtPoint ,side_movement);
		cam = VectorAdd(cam, side_movement);
	}
	if (glutKeyIsDown('d'))
	{
		lookAtPoint = VectorSub(lookAtPoint,side_movement);
		cam = VectorSub(cam, side_movement);
	}

	cam.y = heightFinder(cam.x, cam.z, terrain->texwidth) + 2;

	lookAtPoint.x = cam.x + cos(angle)*cos(yangle);
	lookAtPoint.z = cam.z + sin(angle)*cos(yangle);
	lookAtPoint.y = cam.y + sin(yangle);


	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/ 2,glutGet(GLUT_WINDOW_HEIGHT)/ 2);

	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1200, 800);
	glutCreateWindow ("Laser DOOM");
	//glutFullScreen();
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
