
#include "Flarehandler.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initFlareHandler(FlareHandler* flarehandler) {
  flarehandler->FlaresQuantity = 0;
  flarehandler->timeUntilNextFlare = 0;
  flarehandler->maxDistance = 200;
};


FlareHandler* createFlareHandler() {
  FlareHandler* result = (FlareHandler*) malloc(sizeof(FlareHandler));
  initFlareHandler(result);
  return result;
};


void CheckForNewFlares (FlareHandler* flarehandler, User * user, mat4 projectionMatrix){
  if (user->flareShootingActivated && flarehandler->timeUntilNextFlare < 0) {
    Flare * flare;

    flare = createFlare(projectionMatrix);
    flarehandler->flaresColor[flarehandler->FlaresQuantity] = SetVector(1.0f, 1.0f, 0.0f);
    flare->position = user->cam;
    flarehandler->flaresPositions[flarehandler->FlaresQuantity] = flare->position;
    flarehandler->flaresIntensities[flarehandler->FlaresQuantity] = flare->intensity;
    flare->direction = Normalize(VectorSub(user->lookAtPoint, user->cam));
    flarehandler->flares[flarehandler->FlaresQuantity] = *flare;
    flarehandler->FlaresQuantity += 1;
    flarehandler->timeUntilNextFlare = 10;
  }
	else {
		flarehandler->timeUntilNextFlare--;
		user->flareShootingActivated = false;
	}
}

void diaplayFlares (FlareHandler* flarehandler, mat4 *camMatrix) {
  for (int flare_ind=0; flare_ind < flarehandler->FlaresQuantity; flare_ind++){
    Flare * flare = &flarehandler->flares[flare_ind];

    mat4 trans, rot1;
    trans = T(flare->position.x, flare->position.y, flare->position.z);
    rot1 = Rx(0);
    displayFlare(flare, camMatrix, trans, rot1);
  }
}


void CheckFlaresCollisions (FlareHandler* flarehandler, Terrain * floor, Terrain * roof) {
  for (int flare_ind=0; flare_ind < flarehandler->FlaresQuantity; flare_ind++){
    Flare * flare = &flarehandler->flares[flare_ind];

    float x = flare->position.x;
    float z = flare->position.z;

    float floorheight = heightFinder(x, z, floor);
    float roofheight = heightFinder(x, z, roof);
    //vec3 floorNormal = getNormal(x, z, floor);
    //vec3 roofNormal = getNormal(x, z, roof);

//    if(flare->position.y < floorheight || flare->position.y > roofheight - 1){
    if(flare->position.y < floorheight || flare->position.y > roofheight - 5){

      //flare->flying = false;
      flare->intensity -= 0.05;
      /*
      if(flare->position.y < floorheight ) {
        flarehandler->flaresPositions[flare_ind] = VectorSub(flare->position, ScalarMult(floorNormal, -1));
      }
      else {
        flarehandler->flaresPositions[flare_ind] = VectorSub(flare->position, ScalarMult(roofNormal, -3));
      }
      */

    }

  }
}

void MoveAllFlares(FlareHandler* flarehandler){
  for (int ball=0; ball < flarehandler->FlaresQuantity; ball++){
    Flare * flare = &flarehandler->flares[ball];

    if (flare->flying) {
      MoveFlare(flare);
      flarehandler->flaresPositions[ball] = flare->position;
    }

    flarehandler->flaresIntensities[ball] = flare->intensity*30;
  }
}

void RemoveFlares(FlareHandler* flarehandler){

  for (int ball=0; ball < flarehandler->FlaresQuantity; ball++){
  Flare * flare = &flarehandler->flares[ball];
    //if (Norm(flare->position) > flarehandler->maxDistance) {
    if (flare->intensity < 0.01) {
      flare->active = false;
    }
  }

  Flare new_flares[100];
	vec3 new_FlaresPositions[100];
  float new_FlaresIntensities[100];
	vec3 new_FlaresColor[100];
	int new_ball = 0;

  for (int ball=0; ball < flarehandler->FlaresQuantity; ball++){
    if (flarehandler->flares[ball].active) {
        new_flares[new_ball] = flarehandler->flares[ball];
        new_FlaresPositions[new_ball] = flarehandler->flaresPositions[ball];
        new_FlaresIntensities[new_ball] = flarehandler->flaresIntensities[ball];
				new_FlaresColor[new_ball] = flarehandler->flaresColor[ball];
				new_ball++;
    }
  }

	flarehandler->FlaresQuantity = new_ball;

	for (int ball=0; ball < flarehandler->FlaresQuantity; ball++) {
		flarehandler->flares[ball] = new_flares[ball];
		flarehandler->flaresPositions[ball] = new_FlaresPositions[ball];
    flarehandler->flaresIntensities[ball] = new_FlaresIntensities[ball];
    flarehandler->flaresColor[ball] = new_FlaresColor[ball];
	}
}

void displayFlaresLight (FlareHandler* flarehandler, Terrain * terrain) {
  glUseProgram(terrain->shader);
  //printf("x-pos: %f", flarehandler->flaresPositions[0].x);
  printf("x-int: %f \n", flarehandler->flaresIntensities[0]);
  //printf("x-col: %f", flarehandler->flaresColor[0].x);
  glUniform1i(glGetUniformLocation(terrain->shader, "FlaresQuantity"), flarehandler->FlaresQuantity);
	glUniform3fv(glGetUniformLocation(terrain->shader, "FlaresPositions"), flarehandler->FlaresQuantity, &flarehandler->flaresPositions[0].x);
	glUniform1fv(glGetUniformLocation(terrain->shader, "FlaresIntensities"), flarehandler->FlaresQuantity, &flarehandler->flaresIntensities[0]);
	glUniform3fv(glGetUniformLocation(terrain->shader, "FlaresColor"), flarehandler->FlaresQuantity, &flarehandler->flaresColor[0].x);
}
