
#include "Lightballhandler.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initLightBallHandler(LightBallHandler* lightballhandler) {
  lightballhandler->LightBallsQuantity = 0;
  lightballhandler->timeUntilNextBall = 0;
};


LightBallHandler* createLightBallHandler() {
  LightBallHandler* result = (LightBallHandler*) malloc(sizeof(LightBallHandler));
  initLightBallHandler(result);
  return result;
};


void CheckForNewLightBalls (LightBallHandler* lightballhandler, User * user, mat4 projectionMatrix){
  if (user->lightball_shooting_activated && lightballhandler->timeUntilNextBall < 0) {
    LightBall * lightball;

    lightball = createLightBall(projectionMatrix);
    lightballhandler->lightBallsColor[lightballhandler->LightBallsQuantity] = SetVector(1.0f, 0.0f, 0.0f);
    lightball->position = user->cam;
    lightballhandler->lightBallsPositions[lightballhandler->LightBallsQuantity] = lightball->position;
    lightballhandler->lightBallsIntensities[lightballhandler->LightBallsQuantity] = lightball->intensity;
    lightball->direction = Normalize(VectorSub(user->lookAtPoint, user->cam));
    lightballhandler->lightballs[lightballhandler->LightBallsQuantity] = *lightball;
    lightballhandler->LightBallsQuantity += 1;
    lightballhandler->timeUntilNextBall = 10;
  }
	else {
		lightballhandler->timeUntilNextBall--;
		user->lightball_shooting_activated = false;
	}
}

void CheckLighballsCollisions (LightBallHandler* lightballhandler, Terrain * floor, Terrain * roof) {
  for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++){
    LightBall * lightball = &lightballhandler->lightballs[ball];

    float x = lightball->position.x;
    float z = lightball->position.z;

    float floorheight = heightFinder(x, z, floor);
    float roofheight = heightFinder(x, z, roof);
    vec3 floorNormal = getNormal(x, z, floor);
    vec3 roofNormal = getNormal(x, z, roof);

    if(lightball->position.y < floorheight || lightball->position.y > roofheight - 5){

      lightball->flying = false;
      if (lightball->lifeTimer > 1) {
        lightball->intensity = 2*lightball->lifeTimer;
      }
      else {
        lightball->intensity = 1;
      }

      lightball->lifeTimer--;

      if(lightball->position.y < floorheight ) {
      lightballhandler->lightBallsPositions[ball] = VectorSub(lightball->position, ScalarMult(floorNormal, -1));
      }
      else {
        lightballhandler->lightBallsPositions[ball] = VectorSub(lightball->position, ScalarMult(roofNormal, -3));
      }

    }

  }
}

void MoveAllLightBalls(LightBallHandler* lightballhandler, mat4 *camMatrix, ParticleGenerator* particleGen){
  for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++){
    LightBall * lightball = &lightballhandler->lightballs[ball];

    if (lightball->flying) {
      MoveLightBall(lightball);
      lightballhandler->lightBallsPositions[ball] = lightball->position;
    }
    else {
      lightball->lifeTimer--;
    }


    lightballhandler->lightBallsIntensities[ball] = lightball->intensity;

    mat4 trans, rot1;
    trans = T(lightball->position.x, lightball->position.y, lightball->position.z);
    rot1 = Rx(0);
    displayLightBall(lightball, camMatrix, trans, rot1);

    //PARTICLES
    vec3 initSpeed = ScalarMult(Normalize(lightball->direction), 1);
  	vec4 initColor = {1,0,0,0.5};
    vec3 initPosition = {lightball->position.x, lightball->position.y + 1, lightball->position.z};
    generateParticles(particleGen, 1000, initSpeed, initPosition, 1.1f, initColor, 0.05f, 0.4f, 1.0f);
  }
}

void RemoveLightBalls(LightBallHandler* lightballhandler){

  for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++){
    LightBall * lightball = &lightballhandler->lightballs[ball];
    if (lightball->lifeTimer < -50) {
      lightball->active = false;
    }
  }

  LightBall new_lightballs[100];
	vec3 new_lightBallsPositions[100];
  float new_lightBallsIntensities[100];
	vec3 new_lightBallsColor[100];
	int new_ball = 0;

  for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++){
    if (lightballhandler->lightballs[ball].active) {
        new_lightballs[new_ball] = lightballhandler->lightballs[ball];
        new_lightBallsPositions[new_ball] = lightballhandler->lightBallsPositions[ball];
        new_lightBallsIntensities[new_ball] = lightballhandler->lightBallsIntensities[ball];
				new_lightBallsColor[new_ball] = lightballhandler->lightBallsColor[ball];
				new_ball++;
    }
  }

	lightballhandler->LightBallsQuantity = new_ball;

	for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++) {
		lightballhandler->lightballs[ball] = new_lightballs[ball];
		lightballhandler->lightBallsPositions[ball] = new_lightBallsPositions[ball];
    lightballhandler->lightBallsIntensities[ball] = new_lightBallsIntensities[ball];
    lightballhandler->lightBallsColor[ball] = new_lightBallsColor[ball];
	}
}

void displayLightBallsLight (LightBallHandler* lightballhandler, Terrain * terrain, GLuint enemyShader) {
  glUseProgram(terrain->shader);
  glUniform1i(glGetUniformLocation(terrain->shader, "LightBallsQuantity"), lightballhandler->LightBallsQuantity);
	glUniform3fv(glGetUniformLocation(terrain->shader, "lightBallsPositions"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsPositions[0].x);
	glUniform1fv(glGetUniformLocation(terrain->shader, "lightBallsIntensities"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsIntensities[0]);
	glUniform3fv(glGetUniformLocation(terrain->shader, "lightBallsColor"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsColor[0].x);

  glUseProgram(enemyShader);

  glUniform1i(glGetUniformLocation(enemyShader, "LightBallsQuantity"), lightballhandler->LightBallsQuantity);
	glUniform3fv(glGetUniformLocation(enemyShader, "lightBallsPositions"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsPositions[0].x);
	glUniform1fv(glGetUniformLocation(enemyShader, "lightBallsIntensities"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsIntensities[0]);
	glUniform3fv(glGetUniformLocation(enemyShader, "lightBallsColor"), lightballhandler->LightBallsQuantity, &lightballhandler->lightBallsColor[0].x);

}
