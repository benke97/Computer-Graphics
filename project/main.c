#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "VectorUtils3.h"
#include <stdio.h>

#include "Lightballhandler.h"
#include "Terrain.h"
#include "User.h"
#include "FlashLight.h"

mat4 projectionMatrix;
Terrain* terrain;
Terrain* roof;
User * user;
FlashLight* flashlight;
LightBallHandler* lightballhandler;

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

	lightballhandler = createLightBallHandler();

	// Place flashlight on user position with direction of lookAtPoint
  //vec3 dir = VectorSub(user->lookAtPoint, user->cam);
	flashlight = createFlashLight(user);
  LoadTGATextureSimple("textures/stoneee.tga", &terrain->dirttex);
	LoadTGATextureSimple("textures/stoneee.tga", &roof->dirttex);
	LoadTGATextureSimple("textures/fonarik_low_Albedo.tga", &flashlight->texture);
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
	CheckForNewLightBalls(lightballhandler, user, projectionMatrix);
  glUniform1i(glGetUniformLocation(terrain->shader, "LightBallsQuantity"), lightballhandler->LightBallsQuantity);
	glUniform3fv(glGetUniformLocation(terrain->shader, "lightBallsPositions"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsPositions[0].x);
	glUniform1fv(glGetUniformLocation(terrain->shader, "lightBallsIntensities"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsIntensities[0]);
	glUniform3fv(glGetUniformLocation(terrain->shader, "lightBallsColor"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsColor[0].x);
	glUniform1f(glGetUniformLocation(terrain->shader, "specularExponent"), specularExponent);
	glUniform3f(glGetUniformLocation(terrain->shader, "camPos"), user->cam.x, user->cam.y, user->cam.z);
	glUniformMatrix4fv(glGetUniformLocation(terrain->shader, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, terrain->dirttex);
	DrawModel(terrain->tm, terrain->shader, "inPosition", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(roof->shader, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, roof->dirttex);
	DrawModel(roof->tm, roof->shader, "inPosition", "inNormal", "inTexCoord");


//LightBalls
	CheckLighballsCollisions (lightballhandler, terrain, roof);
 	MoveAllLightBalls(lightballhandler, &camMatrix);
 	RemoveLightBalls(lightballhandler);

  // FlashLight
  glUseProgram(terrain->shader);

  FlashLight__updateDirection(flashlight, user);
	FlashLight__updatePosition(flashlight, user);

  glUniform3f(glGetUniformLocation(terrain->shader, "flashlightPosition"), flashlight->position.x, flashlight->position.y, flashlight->position.z);
  glUniform3f(glGetUniformLocation(terrain->shader, "flashlightDirection"), flashlight->direction.x, flashlight->direction.y, flashlight->direction.z);
  glUniform1f(glGetUniformLocation(terrain->shader, "flashlightCutOff"), flashlight->cutOffAngle);
	glUniform1f(glGetUniformLocation(terrain->shader, "flashlightOuterCutOff"), flashlight->outerCutOff);

	mat4 rot1 = Ry(-angle+ M_PI/2);
	mat4 rot2 = Rx(-yangle);
	mat4 rot = Mult(rot1,rot2);
	printf("angle %f\n", angle);
	glUseProgram(flashlight->shader);
	mat4 trans = T(flashlight->position.x + flashlight->direction.x/1.5, flashlight->position.y + flashlight->direction.y/1.5, flashlight->position.z + flashlight->direction.z/1.5);
	mat4 scale = S(0.007,0.007,0.007);
	mat4 tot = Mult(trans,scale);
	glBindTexture(GL_TEXTURE_2D, flashlight->texture);
	drawFlashlight(flashlight, projectionMatrix);
	displayFlashlight(flashlight, &camMatrix,tot,rot);
	//printf("innercutoff %f\n", flashlight->cutOffAngle);
  //printf("outercutoff %f\n", flashlight->outerCutOff);
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
