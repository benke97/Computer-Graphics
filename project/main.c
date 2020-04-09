#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "VectorUtils3.h"
#include <stdio.h>

#include "Terrain.h"
#include "User.h"

#include "FlashLight.h"

#include "Lightball.h"

mat4 projectionMatrix, trans, rot1;
Terrain* terrain;
Terrain* roof;
User * user;
FlashLight* flashlight;
LightBall * lightball;
vec3 ballposition;
vec3 lightSourcesColorsArr[] = { {0.0f, 0.0f, 0.0f},

                                 {1.0f, 1.0f, 1.0f} }; // White light

GLint isDirectional[] = {1,0};

vec3 lightSourcesDirectionsPositions[] = { {0.58, 0.58, 0.58}, // Sunlight

                                       {0.0f, 0.0f, 0.0f} }; // Ball

GLfloat specularExponent = 100;


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
  //lightball = createLightBall(projectionMatrix);

	// Place flashlight on user position with direction of lookAtPoint
	flashlight = createFlashLight(&user->cam, &user->lookAtPoint);

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


  if (user->lightball_shooting_activated) {
    user->lightball_shooting_activated = false;
    lightball = createLightBall(projectionMatrix);
    lightball->position = user->cam;
    lightball->direction = Normalize(user->lookAtPoint);
  }

	glUniform3fv(glGetUniformLocation(terrain->shader, "lightSourcesDirPosArr"), 2, &lightSourcesDirectionsPositions[0].x);

	glUniform3fv(glGetUniformLocation(terrain->shader, "lightSourcesColorArr"), 2, &lightSourcesColorsArr[0].x);

	glUniform1iv(glGetUniformLocation(terrain->shader, "isDirectional"), 2, isDirectional);

	glUniform1f(glGetUniformLocation(terrain->shader, "specularExponent"), specularExponent);

	glUniform3f(glGetUniformLocation(terrain->shader, "camPos"), user->cam.x, user->cam.y, user->cam.z);


	//before ball:
	glUniformMatrix4fv(glGetUniformLocation(terrain->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(terrain->tm, terrain->shader, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(roof->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof->tm, roof->shader, "inPosition", "inNormal", "inTexCoord");


	//Draw LightBall

  if (lightball) {
    MoveLightBall(lightball);
    lightSourcesDirectionsPositions[1] = lightball->position;
    trans = T(lightball->position.x, lightball->position.y, lightball->position.z);
    rot1 = Rx(0);
    displayLightBall(lightball, camMatrix, trans, rot1);
  }


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
