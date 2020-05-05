#ifndef _LASERHANDLER_H_
#define _LASERHANDLER_H_

#include "VectorUtils3.h"
#include "Terrain.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "Laser.h"
#include "Gun.h"
#include "User.h"
#include "ParticleGenerator.h"



typedef struct LaserHandler {
  vec3 laserColors[100];
  vec3 laserPositions[100];
  float laserIntensities[100];
  Laser lasers[100];
  int LaserQuantity;
  int timeUntilNextLaser;
  int maxDistance;
} LaserHandler;


void initLaserHandler(LaserHandler*);
LaserHandler* createLaserHandler();
void RemoveLasers(LaserHandler*);
void CheckLaserCollisions (LaserHandler* laserhandler, Terrain * floor, Terrain * roof);
void MoveAllLasers(LaserHandler*, mat4*,mat4);
void CheckForNewLasers (LaserHandler* laserhandler, User * user, Gun * gun, mat4 projectionMatrix, ParticleGenerator* particleGen);
void displayLaserLight (LaserHandler* laserhandler, Terrain * terrain);

#endif
