#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "VectorUtils3.h"
#include <stdio.h>

#include "Lightballhandler.h"
#include "Terrain.h"
#include "User.h"
#include "FlashLight.h"
#include "Gun.h"
#include "Laser.h"
#include "ParticleGenerator.h"
#include "Flarehandler.h"
#include "Laserhandler.h"
#include "Enemyhandler.h"

mat4 projectionMatrix;
Terrain* terrain_floor;
Terrain* roof;
User * user;
FlashLight* flashlight;
Gun* gun;
LightBallHandler* lightballhandler;
LaserHandler* laserhandler;
FlareHandler* flarehandler;
EnemyHandler* enemyhandler;
GLfloat specularExponent = 100;
ParticleGenerator* FLParticleGen;

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
	flarehandler = createFlareHandler();
	laserhandler = createLaserHandler();
	enemyhandler = createEnemyHandler();
	// Place flashlight on user position with direction of lookAtPoint
  //vec3 dir = VectorSub(user->lookAtPoint, user->cam);
	flashlight = createFlashLight(user);
	gun = createGun(user);
  LoadTGATextureSimple("textures/stoneee.tga", &terrain_floor->terrain_texture);
	LoadTGATextureSimple("textures/stoneee.tga", &roof->terrain_texture);

	// Particle Generator
	// init shaders
	GLuint PGshaderID = loadShaders("shaders/particleGen1.vert", "shaders/particleGen1.frag");
	glUseProgram(PGshaderID);
	FLParticleGen = createParticleGenerator(100000, &PGshaderID);

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
	CheckForNewFlares(flarehandler, user, projectionMatrix);
	CheckForNewEnemies(enemyhandler, user, terrain_floor, projectionMatrix);



	displayFlaresLight (flarehandler, terrain_floor);
	displayLightBallsLight (lightballhandler, terrain_floor);
	//displayEnemiesLight (enemyhandler, terrain_floor);
	displayTerrain(terrain_floor, roof, specularExponent, user->cam, &camMatrix);

//LightBalls
	CheckLighballsCollisions (lightballhandler, terrain_floor, roof);
 	MoveAllLightBalls(lightballhandler, &camMatrix, FLParticleGen);
 	RemoveLightBalls(lightballhandler);

	CheckFlaresCollisions (flarehandler, terrain_floor, roof);
	MoveAllFlares(flarehandler);
	RemoveFlares(flarehandler);
	diaplayFlares (flarehandler, &camMatrix);





	CheckEnemiesCollisions (enemyhandler, terrain_floor, roof);
	MoveAllEnemies(enemyhandler);
	RemoveEnemies(enemyhandler);
	diaplayEnemies (enemyhandler, &camMatrix);



	CheckForNewLasers(laserhandler,user,gun,projectionMatrix);
	displayLaserLight (laserhandler, terrain_floor);
	CheckLaserCollisions(laserhandler, terrain_floor, roof);
	MoveAllLasers(laserhandler,&camMatrix, projectionMatrix);
	RemoveLasers(laserhandler);



  // FlashLight


	drawGun(gun, projectionMatrix);
	displayGun(gun, &camMatrix, angle, yangle);
	drawFlashlight(flashlight, projectionMatrix);
	displayFlashlight(flashlight, &camMatrix,angle,yangle);


  FlashLight__updateDirection(flashlight, user);
	FlashLight__updatePosition(flashlight, user);
	Gun__updateDirection(gun, user);
	Gun__updatePosition(gun, user);

  glUseProgram(terrain_floor->shader);
	glUniform3f(glGetUniformLocation(terrain_floor->shader, "flashlightPosition"), flashlight->position.x, flashlight->position.y, flashlight->position.z);
  glUniform3f(glGetUniformLocation(terrain_floor->shader, "flashlightDirection"), flashlight->direction.x, flashlight->direction.y, flashlight->direction.z);
  glUniform1f(glGetUniformLocation(terrain_floor->shader, "flashlightCutOff"), flashlight->cutOffAngle);
	glUniform1f(glGetUniformLocation(terrain_floor->shader, "flashlightOuterCutOff"), flashlight->outerCutOff);











	//printf("innercutoff %f\n", flashlight->cutOffAngle);
  //printf("outercutoff %f\n", flashlight->outerCutOff);
	//scale = S(1,1,1);
	//trans = T(laser->position.x, laser->position.y, laser->position.z);
	//trans = T(gun->position.x + gun->direction.x, gun->position.y + gun->direction.y, gun->position.z + gun->direction.z);
	//rot = Rz(M_PI/2);
	//tot = Mult(trans,scale);
	//displayLaser(laser, &camMatrix, tot, rot, projectionMatrix);
	printError("display 2");

	// Particles, generateParticles(ParticleGenerator* particleGen, int particlesPerSec, vec3 initialSpeed, vec3 initialPostition, vec4 initialColor, float particleSpread, GLfloat initialSize, GLfloat initialLifeInSeconds)
	glUseProgram(FLParticleGen->shaderID);
	//vec3 initSpeed = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.0005);
	//vec4 initColor = {1,0,0,0.5};
	//generateParticles(FLParticleGen, 1000, initSpeed, flashlight->position, initColor, 0.1f, 0.3f, 1.0f);
	simulateAllParticles(FLParticleGen, user);
	drawAllParticles(FLParticleGen, &camMatrix, projectionMatrix);

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
	glutHideCursor();
	glutTimerFunc(20, &timer, 0);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	exit(0);
}
