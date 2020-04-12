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

vec3 ballposition;
vec3 lightBallsColor[100] = {};
vec3 lightBallsPositions[100] = {};
LightBall lightballs[100] = {};
int LightBallsQuantity;
GLfloat specularExponent = 100;

GLint isDirectional[] = {0,0,0,0};
int timeUntilNextBall;

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
  LightBallsQuantity = 0;
  timeUntilNextBall = 0;
	// Place flashlight on user position with direction of lookAtPoint
  vec3 dir = VectorSub(user->lookAtPoint, user->cam);
	flashlight = createFlashLight(&user->cam, &dir);

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

  if (user->lightball_shooting_activated && timeUntilNextBall < 0) {
    LightBall * lightball;

    lightball = createLightBall(projectionMatrix);
    lightBallsColor[LightBallsQuantity] = SetVector(1.0f, 1.0f, 1.0f);
    lightball->position = user->cam;
    lightBallsPositions[LightBallsQuantity] = lightball->position;
    lightball->direction = Normalize(VectorSub(user->lookAtPoint, user->cam));
    lightballs[LightBallsQuantity] = *lightball;
    LightBallsQuantity += 1;
    timeUntilNextBall = 10;
  }
	else {
		timeUntilNextBall--;
		user->lightball_shooting_activated = false;
	}


  glUniform1i(glGetUniformLocation(terrain->shader, "LightBallsQuantity"), LightBallsQuantity);

	glUniform3fv(glGetUniformLocation(terrain->shader, "lightSourcesDirPosArr"), LightBallsQuantity, &lightBallsPositions[0].x);

	glUniform3fv(glGetUniformLocation(terrain->shader, "lightSourcesColorArr"), LightBallsQuantity, &lightBallsColor[0].x);

  glUniform1iv(glGetUniformLocation(terrain->shader, "isDirectional"), 4, isDirectional);

	glUniform1f(glGetUniformLocation(terrain->shader, "specularExponent"), specularExponent);

	glUniform3f(glGetUniformLocation(terrain->shader, "camPos"), user->cam.x, user->cam.y, user->cam.z);


	//before ball:
	glUniformMatrix4fv(glGetUniformLocation(terrain->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(terrain->tm, terrain->shader, "inPosition", "inNormal", "inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(roof->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof->tm, roof->shader, "inPosition", "inNormal", "inTexCoord");


	//Draw LightBall
  for (int ball=0; ball < LightBallsQuantity; ball++){
    LightBall * lightball = &lightballs[ball];
    MoveLightBall(lightball);
    lightBallsPositions[ball] = lightball->position;
    trans = T(lightball->position.x, lightball->position.y, lightball->position.z);
    rot1 = Rx(0);
    displayLightBall(lightball, camMatrix, trans, rot1);

		if (Norm(lightball->position) > 200) {
			lightball->active = false;
		}
  }

	LightBall new_lightballs[100];
	vec3 new_lightBallsPositions[100];
	vec3 new_lightBallsColor[100];
	int new_ball = 0;

  for (int ball=0; ball < LightBallsQuantity; ball++){
    if (lightballs[ball].active) {
        new_lightballs[new_ball] = lightballs[ball];
        new_lightBallsPositions[new_ball] = lightBallsPositions[ball];
				new_lightBallsColor[new_ball] = lightBallsColor[ball];
				new_ball++;
    }
  }

	LightBallsQuantity = new_ball;

	for (int ball=0; ball < LightBallsQuantity; ball++) {
		lightballs[ball] = new_lightballs[ball];
		lightBallsPositions[ball] = new_lightBallsPositions[ball];
		lightBallsColor[ball] = new_lightBallsColor[ball];
	}


  // FlashLight
  glUseProgram(terrain->shader);

  vec3 dir = VectorSub(user->lookAtPoint, user->cam);
  FlashLight__setDirection(flashlight, &dir);

  glUniform3f(glGetUniformLocation(terrain->shader, "flashlightPosition"), flashlight->position->x, flashlight->position->y, flashlight->position->z);
  glUniform3f(glGetUniformLocation(terrain->shader, "flashlightDirection"), flashlight->direction->x, flashlight->direction->y, flashlight->direction->z);
  glUniform1f(glGetUniformLocation(terrain->shader, "flashlightCutOff"), flashlight->cutOffAngle);


	printError("display 2");
	glutSwapBuffers();
}

void timer(int i)
{
	userInput(user, roof, terrain);
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
