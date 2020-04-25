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

mat4 projectionMatrix;
Terrain* terrain_floor;
Terrain* roof;
User * user;
FlashLight* flashlight;
Gun* gun;
LightBallHandler* lightballhandler;
Laser* laser;
FlareHandler* flarehandler;
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

	// Place flashlight on user position with direction of lookAtPoint
  //vec3 dir = VectorSub(user->lookAtPoint, user->cam);
	flashlight = createFlashLight(user);
	gun = createGun(user);
	laser = createLaser(gun);
  LoadTGATextureSimple("textures/stoneee.tga", &terrain_floor->terrain_texture);
	LoadTGATextureSimple("textures/stoneee.tga", &roof->terrain_texture);
	LoadTGATextureSimple("textures/fonarik_low_Albedo.tga", &flashlight->texture);
	LoadTGATextureSimple("textures/Laser_Rifle_Diffuse.tga", &gun->texture);
	// Particle Generator
	// init shaders
	GLuint PGshaderID = loadShaders("shaders/particleGen1.vert", "shaders/particleGen1.frag");
	glUseProgram(PGshaderID);
	FLParticleGen = createParticleGenerator(250000, &PGshaderID);

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

	displayFlaresLight (flarehandler, terrain_floor);
	displayLightBallsLight (lightballhandler, terrain_floor);
	displayTerrain(terrain_floor, roof, specularExponent, user->cam, &camMatrix);

//LightBalls
	CheckLighballsCollisions (lightballhandler, terrain_floor, roof);
 	MoveAllLightBalls(lightballhandler, &camMatrix, FLParticleGen);
 	RemoveLightBalls(lightballhandler);

	CheckFlaresCollisions (flarehandler, terrain_floor, roof);
	MoveAllFlares(flarehandler);
	RemoveFlares(flarehandler);
	diaplayFlares (flarehandler, &camMatrix);


  // FlashLight
  glUseProgram(terrain_floor->shader);

  FlashLight__updateDirection(flashlight, user);
	FlashLight__updatePosition(flashlight, user);
	Gun__updateDirection(gun, user);
	Gun__updatePosition(gun, user);

  glUniform3f(glGetUniformLocation(terrain_floor->shader, "flashlightPosition"), flashlight->position.x, flashlight->position.y, flashlight->position.z);
  glUniform3f(glGetUniformLocation(terrain_floor->shader, "flashlightDirection"), flashlight->direction.x, flashlight->direction.y, flashlight->direction.z);
  glUniform1f(glGetUniformLocation(terrain_floor->shader, "flashlightCutOff"), flashlight->cutOffAngle);
	glUniform1f(glGetUniformLocation(terrain_floor->shader, "flashlightOuterCutOff"), flashlight->outerCutOff);



	mat4 rot1 = Ry(-angle+ M_PI/2);
	mat4 rot2 = Rx(-yangle);
	mat4 rot = Mult(rot1,rot2);
	//printf("angle %f\n", angle);
	glUseProgram(flashlight->shader);
	mat4 trans = T(flashlight->position.x + flashlight->direction.x/1.5, flashlight->position.y + flashlight->direction.y/1.5, flashlight->position.z + flashlight->direction.z/1.5);
	mat4 scale = S(0.007,0.007,0.007);
	mat4 tot = Mult(trans,scale);
	glBindTexture(GL_TEXTURE_2D, flashlight->texture);
	drawFlashlight(flashlight, projectionMatrix);
	displayFlashlight(flashlight, &camMatrix,tot,rot);


	glUseProgram(gun->shader);
	trans = T(gun->position.x + gun->direction.x/1.5, gun->position.y + gun->direction.y/1.5, gun->position.z + gun->direction.z/1.5);
	scale = S(0.015,0.015,0.015);
	tot = Mult(trans,scale);
	rot1 = Ry(-angle+M_PI/2);
	rot2 = Rx(-yangle);
	rot = Mult(rot1,rot2);
	glBindTexture(GL_TEXTURE_2D, gun->texture);
	drawGun(gun, projectionMatrix);
	displayGun(gun, &camMatrix,tot,rot);
	//printf("innercutoff %f\n", flashlight->cutOffAngle);
  //printf("outercutoff %f\n", flashlight->outerCutOff);
	scale = S(1,1,1);
	//trans = T(laser->position.x, laser->position.y, laser->position.z);
	trans = T(gun->position.x + gun->direction.x, gun->position.y + gun->direction.y, gun->position.z + gun->direction.z);
	rot = Rz(M_PI/2);
	tot = Mult(trans,scale);
	displayLaser(laser, &camMatrix, tot, rot, projectionMatrix);
	printError("display 2");
	glutSwapBuffers();

	// Particles, generateParticles(ParticleGenerator* particleGen, int particlesPerSec, vec3 initialSpeed, vec3 initialPostition, vec4 initialColor, float particleSpread, GLfloat initialSize, GLfloat initialLifeInSeconds)
	glUseProgram(FLParticleGen->shaderID);
	//vec3 initSpeed = ScalarMult(Normalize(VectorSub(user->cam, user->lookAtPoint)), 0.05);
	//vec4 initColor = {1,0,0,0.5};
	//generateParticles(FLParticleGen, 1000, initSpeed, flashlight->position, initColor, 1.5f, 0.05f, 2.0f);
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
	glutTimerFunc(20, &timer, 0);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	exit(0);
}
