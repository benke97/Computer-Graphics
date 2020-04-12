
#include "Lightballhandler.h"

#include "Terrain.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initLightBallHandler(LightBallHandler* lightballhandler) {
  lightballhandler->LightBallsQuantity = 0;
  lightballhandler->timeUntilNextBall = 0;
  lightballhandler->maxDistance = 200;
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
    lightballhandler->lightBallsColor[lightballhandler->LightBallsQuantity] = SetVector(1.0f, 1.0f, 1.0f);
    lightball->position = user->cam;
    lightballhandler->lightBallsPositions[lightballhandler->LightBallsQuantity] = lightball->position;
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


void MoveAllLightBalls(LightBallHandler* lightballhandler, mat4 *camMatrix){
  for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++){
    LightBall * lightball = &lightballhandler->lightballs[ball];

    MoveLightBall(lightball);
    lightballhandler->lightBallsPositions[ball] = lightball->position;

    mat4 trans, rot1;
    trans = T(lightball->position.x, lightball->position.y, lightball->position.z);
    rot1 = Rx(0);
    displayLightBall(lightball, camMatrix, trans, rot1);

  }
}

void RemoveLightBalls(LightBallHandler* lightballhandler){

  for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++){
  LightBall * lightball = &lightballhandler->lightballs[ball];
    if (Norm(lightball->position) > lightballhandler->maxDistance) {
      lightball->active = false;
    }
  }

  LightBall new_lightballs[100];
	vec3 new_lightBallsPositions[100];
	vec3 new_lightBallsColor[100];
	int new_ball = 0;

  for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++){
    if (lightballhandler->lightballs[ball].active) {
        new_lightballs[new_ball] = lightballhandler->lightballs[ball];
        new_lightBallsPositions[new_ball] = lightballhandler->lightBallsPositions[ball];
				new_lightBallsColor[new_ball] = lightballhandler->lightBallsColor[ball];
				new_ball++;
    }
  }

	lightballhandler->LightBallsQuantity = new_ball;

	for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++) {
		lightballhandler->lightballs[ball] = new_lightballs[ball];
		lightballhandler->lightBallsPositions[ball] = new_lightBallsPositions[ball];
		lightballhandler->lightBallsColor[ball] = new_lightBallsColor[ball];
	}
}
