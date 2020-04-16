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
Terrain* terrain_floor;
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

	bool is_floor = true;
	terrain_floor = createTerrain(projectionMatrix, is_floor);
	roof = createTerrain(projectionMatrix, !is_floor);

	user = createUser();
	lightballhandler = createLightBallHandler();

	// Place flashlight on user position with direction of lookAtPoint
  //vec3 dir = VectorSub(user->lookAtPoint, user->cam);
	flashlight = createFlashLight(user);
  LoadTGATextureSimple("textures/stoneee.tga", &terrain_floor->terrain_texture);
	LoadTGATextureSimple("textures/stoneee.tga", &roof->terrain_texture);
	LoadTGATextureSimple("textures/fonarik_low_Albedo.tga", &flashlight->texture);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 camMatrix;
	camMatrix = lookAt(user->cam.x, user->cam.y, user->cam.z,
				user->lookAtPoint.x, user->lookAtPoint.y, user->lookAtPoint.z,
				user->upVector.x, user->upVector.y, user->upVector.z);

	printError("pre display");

	CheckForNewLightBalls(lightballhandler, user, projectionMatrix);
	displayLightBallsLight (lightballhandler, terrain_floor);
	displayTerrain(terrain_floor, roof, specularExponent, user->cam, &camMatrix);


//LightBalls
	CheckLighballsCollisions (lightballhandler, terrain_floor, roof);
 	MoveAllLightBalls(lightballhandler, &camMatrix);
 	RemoveLightBalls(lightballhandler);

  // FlashLight
  glUseProgram(terrain_floor->shader);

  FlashLight__updateDirection(flashlight, user);
	FlashLight__updatePosition(flashlight, user);

  glUniform3f(glGetUniformLocation(terrain_floor->shader, "flashlightPosition"), flashlight->position.x, flashlight->position.y, flashlight->position.z);
  glUniform3f(glGetUniformLocation(terrain_floor->shader, "flashlightDirection"), flashlight->direction.x, flashlight->direction.y, flashlight->direction.z);
  glUniform1f(glGetUniformLocation(terrain_floor->shader, "flashlightCutOff"), flashlight->cutOffAngle);
	glUniform1f(glGetUniformLocation(terrain_floor->shader, "flashlightOuterCutOff"), flashlight->outerCutOff);



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
	userInput(user, roof, terrain_floor);
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
