#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "VectorUtils3.h"
#include <stdio.h>

#include "Terrain.h"
#include "User.h"

mat4 projectionMatrix;
Terrain* terrain;
Terrain* roof;
User * user;

void init(void)
{
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 5000.0);
	glClearColor(0.859,0.957,0.998,1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");
	printf("%d", 5);
	int heightmap = 1;
	int height = 0;
	terrain = createTerrain(projectionMatrix,heightmap,height);
	heightmap = 0;
	height = 20;
	roof = createTerrain(projectionMatrix,heightmap,height);
	user = createUser();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 total, modelView, camMatrix;
	camMatrix = lookAt(user->cam.x, user->cam.y, user->cam.z,
				user->lookAtPoint.x, user->lookAtPoint.y, user->lookAtPoint.z,
				user->upVector.x, user->upVector.y, user->upVector.z);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	printError("pre display");
	glUseProgram(terrain->shader);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(terrain->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(terrain->tm, terrain->shader, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(roof->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof->tm, roof->shader, "inPosition", "inNormal", "inTexCoord");
	printError("display 2");
	glutSwapBuffers();
}

void timer(int i)
{
	userInput(user, terrain);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/ 2,glutGet(GLUT_WINDOW_HEIGHT)/ 2);
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}
//dasd
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1200, 800);
	glutCreateWindow ("Laser DOOM");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	exit(0);
}
