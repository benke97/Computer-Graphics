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


void CheckForNewLasers(LaserHandler* laserhandler,User * user, Gun * gun, mat4 projectionMatrix, ParticleGenerator* particleGen){
  if (user->laser_shooting_activated && laserhandler->timeUntilNextLaser < 0 && !gun->overheated) {
    if (gun->heat > 200)
    {
      gun->overheated = true;
    }
    else
    {
      Laser * laser;
      laser = createLaser(gun);
      laserhandler->laserColors[laserhandler->LaserQuantity] = SetVector(1.0f, 0.0f, 0.0f);
      laserhandler->laserPositions[laserhandler->LaserQuantity] = laser->position;
      laserhandler->lasers[laserhandler->LaserQuantity] = *laser;
      laserhandler->LaserQuantity += 1;
      laserhandler->timeUntilNextLaser = 2;
      gun->heat += 10;
      printf("%f heat\n",gun->heat);

      // Create paricles on gun pos
      vec3 initSpeed = ScalarMult(Normalize(gun->direction), 2);
      vec4 targetColor = {1,0,0,0.5};
      vec3 initPosition = {gun->position.x + initSpeed.x - 0.1, gun->position.y + initSpeed.y - 0.1, gun->position.z + initSpeed.z - 0.1};
      generateParticles(particleGen, 100, initSpeed, initPosition, 0.1f, targetColor, 0.01f, 0.15f, 1.0f);
    }
  }
	else {
		laserhandler->timeUntilNextLaser--;
		user->laser_shooting_activated = false;
	}
  if (gun->heat > 0)
  {
    gun->heat -= 1;
  }
  if (gun->heat < 0)
  {
    gun->heat = 0;
  }
  if (gun->heat == 0 && gun->overheated)
  {
    gun->overheated = false;
  }

  if (gun->overheated)
  {
    vec3 initSpeed = ScalarMult(Normalize(gun->direction), 2);
    vec4 targetColor = {0.5,0.5,0.5,0.5};
    vec3 initPosition = {gun->position.x + initSpeed.x*0.5, gun->position.y + initSpeed.y*0.5, gun->position.z + initSpeed.z*0.5};
    initSpeed = SetVector(0,1,0);
    generateParticles(particleGen, 80, initSpeed, initPosition, 0.18f, targetColor, 0.001f, 0.2f, 1.0f);
  }
}

void CheckLaserCollisions (LaserHandler* laserhandler, Terrain * floor, Terrain * roof) {
  for (int lasercount=0; lasercount < laserhandler->LaserQuantity; lasercount++){
    Laser * laser = &laserhandler->lasers[lasercount];

    float x = laser->position.x;
    float z = laser->position.z;

    float floorheight = heightFinder(x, z, floor);
    float roofheight = heightFinder(x, z, roof);
    vec3 floorNormal = getNormal(x, z, floor);
    vec3 roofNormal = getNormal(x, z, roof);

    if(laser->position.y < floorheight || laser->position.y > roofheight - 5){

      laser->flying = false;
      laser->active = false;



      if(laser->position.y < floorheight ) {
      laserhandler->laserPositions[lasercount] = VectorSub(laser->position, ScalarMult(floorNormal, -1));
      }
      else {
        laserhandler->laserPositions[lasercount] = VectorSub(laser->position, ScalarMult(roofNormal, -3));
      }

    }

  }
}

void MoveAllLasers(LaserHandler* laserhandler, mat4 *camMatrix, mat4 projectionMatrix){
  for (int lasercount=0; lasercount < laserhandler->LaserQuantity; lasercount++){
    Laser * laser = &laserhandler->lasers[lasercount];

    if (laser->flying) {
      Laser__updatePosition(laser);
      laserhandler->laserPositions[lasercount] = laser->position;
    }
    glUseProgram(laser->shader);
    mat4 trans, scale, tot;
    trans = T(laser->position.x, laser->position.y, laser->position.z);
    scale = S(0.7,0.7,0.7);
    tot = Mult(trans,scale);
    displayLaser(laser, camMatrix, tot, laser->orientation, projectionMatrix);

  }
}

void RemoveLasers(LaserHandler* laserhandler){
  Laser new_lasers[100];
	vec3 new_laserPositions[100];
  float new_laserIntensities[100];
	vec3 new_laserColors[100];
	int new_laser = 0;

  for (int lasercount=0; lasercount < laserhandler->LaserQuantity; lasercount++){
    if (laserhandler->lasers[lasercount].active) {
        new_lasers[new_laser] = laserhandler->lasers[lasercount];
        new_laserPositions[new_laser] = laserhandler->laserPositions[lasercount];
        new_laserIntensities[new_laser] = laserhandler->laserIntensities[lasercount];
				new_laserColors[new_laser] = laserhandler->laserColors[lasercount];
				new_laser++;
    }
  }

	laserhandler->LaserQuantity = new_laser;

	for (int lasercount=0; lasercount < laserhandler->LaserQuantity; lasercount++) {
		laserhandler->lasers[lasercount] = new_lasers[lasercount];
		laserhandler->laserPositions[lasercount] = new_laserPositions[lasercount];
    laserhandler->laserIntensities[lasercount] = new_laserIntensities[lasercount];
    laserhandler->laserColors[lasercount] = new_laserColors[lasercount];
	}
}

void displayLaserLight (LaserHandler* laserhandler, Terrain * terrain) {
  glUseProgram(terrain->shader);
  glUniform1i(glGetUniformLocation(terrain->shader, "LaserQuantity"), laserhandler->LaserQuantity);
	glUniform3fv(glGetUniformLocation(terrain->shader, "laserPositions"), laserhandler->LaserQuantity, &laserhandler->laserPositions[0].x);
	glUniform1fv(glGetUniformLocation(terrain->shader, "laserIntensities"), laserhandler->LaserQuantity, &laserhandler->laserIntensities[0]);
	glUniform3fv(glGetUniformLocation(terrain->shader, "laserColors"), laserhandler->LaserQuantity, &laserhandler->laserColors[0].x);
}
