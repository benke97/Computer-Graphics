
#include "Laserhandler.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initLaserHandler(LaserHandler* laserhandler) {
  laserhandler->LaserQuantity = 0;
  laserhandler->timeUntilNextLaser = 0;
  laserhandler->maxDistance = 200;
};


LaserHandler* createLaserHandler() {
  LaserHandler* result = (LaserHandler*) malloc(sizeof(LaserHandler));
  initLaserHandler(result);
  return result;
};


void CheckForNewLasers(LaserHandler* laserhandler,User * user, Gun * gun, mat4 projectionMatrix){
  if (user->laser_shooting_activated && laserhandler->timeUntilNextLaser < 0) {
    Laser * laser;
    laser = createLaser(gun);
    laserhandler->laserColors[laserhandler->LaserQuantity] = SetVector(1.0f, 0.0f, 0.0f);
    laserhandler->laserPositions[laserhandler->LaserQuantity] = laser->position;
    laserhandler->lasers[laserhandler->LaserQuantity] = *laser;
    laserhandler->LaserQuantity += 1;
    laserhandler->timeUntilNextLaser = 10;
  }
	else {
		laserhandler->timeUntilNextLaser--;
		user->laser_shooting_activated = false;
	}
}

void CheckLaserCollisions (LaserHandler* laserhandler, Terrain * floor, Terrain * roof) {
  for (int lasercount=0; lasercount < laserhandler->LaserQuantity; lasercount++){
    Laser * laser = &laserhandler->lightballs[lasercount];

    float x = laser->position.x;
    float z = laser->position.z;

    float floorheight = heightFinder(x, z, floor->texwidth, floor);
    float roofheight = heightFinder(x, z, roof->texwidth, roof);
    vec3 floorNormal = getNormal(x, z, floor->texwidth, floor);
    vec3 roofNormal = getNormal(x, z, roof->texwidth, roof);

    if(laser->position.y < floorheight || laser->position.y > roofheight - 5){

      laser->flying = false;
      //lightball->lifeTimer--;



      if(lightball->position.y < floorheight ) {
      laserhandler->laserPositions[lasercount] = VectorSub(laser->position, ScalarMult(floorNormal, -1));
      }
      else {
        laserhandler->laserPositions[lasercount] = VectorSub(laser->position, ScalarMult(roofNormal, -3));
      }

    }

  }
}

void MoveAllLasers(LaserHandler* laserhandler, mat4 *camMatrix){
  for (int lasercount=0; lasercount < laserhandler->LaserQuantity; lasercount++){
    Laser * laser = &laserhandler->lasers[lasercount];

    if (laser->flying) {
      Laser__updatePosition(laser);
      laserhandler->laserPositions[lasercount] = laser->position;
    }

    mat4 trans, rot1;
    trans = T(laser->position.x, laser->position.y, laser->position.z);
    rot1 = Rz(M_PI/2);
    displayLightBall(lightball, camMatrix, trans, rot1);

  }
}

void RemoveLasers(LaserHandler* laserhandler){

  for (int lasercount=0; lasercount < laserhandler->LaserQuantity; lasercount++){
  Laser * laser = &laserhandler->lasers[lasercount];
    //if (Norm(lightball->position) > laserhandler->maxDistance) {
    if (lightball->lifeTimer < -50) {
      lightball->active = false;
    }
  }

  LightBall new_lightballs[100];
	vec3 new_laserPositions[100];
  float new_laserIntensities[100];
	vec3 new_laserColors[100];
	int new_ball = 0;

  for (int ball=0; ball < laserhandler->LaserQuantity; ball++){
    if (laserhandler->lightballs[ball].active) {
        new_lightballs[new_ball] = laserhandler->lightballs[ball];
        new_laserPositions[new_ball] = laserhandler->laserPositions[ball];
        new_laserIntensities[new_ball] = laserhandler->laserIntensities[ball];
				new_laserColors[new_ball] = laserhandler->laserColors[ball];
				new_ball++;
    }
  }

	laserhandler->LaserQuantity = new_ball;

	for (int ball=0; ball < laserhandler->LaserQuantity; ball++) {
		laserhandler->lightballs[ball] = new_lightballs[ball];
		laserhandler->laserPositions[ball] = new_laserPositions[ball];
    laserhandler->laserIntensities[ball] = new_laserIntensities[ball];
    laserhandler->laserColors[ball] = new_laserColors[ball];
	}
}

void displayLaserLight (LaserHandler* laserhandler, Terrain * terrain) {
  glUseProgram(terrain->shader);
  glUniform1i(glGetUniformLocation(terrain->shader, "LaserQuantity"), laserhandler->LaserQuantity);
	glUniform3fv(glGetUniformLocation(terrain->shader, "laserPositions"), laserhandler->LaserQuantity, &laserhandler->laserPositions[0].x);
	glUniform1fv(glGetUniformLocation(terrain->shader, "laserIntensities"), laserhandler->LaserQuantity, &laserhandler->laserIntensities[0]);
	glUniform3fv(glGetUniformLocation(terrain->shader, "laserColors"), laserhandler->LaserQuantity, &laserhandler->laserColors[0].x);
}
