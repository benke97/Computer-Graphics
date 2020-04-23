#ifndef _FLAREHANDLER_H_
#define _FLAREHANDLER_H_

#include "VectorUtils3.h"
#include "Flare.h"
#include "User.h"
#include "Terrain.h"
#include "loadobj.h"
#include "LoadTGA.h"



typedef struct FlareHandler {
  vec3 flaresColor[100];
  vec3 flaresPositions[100];
  float flaresIntensities[100];
  Flare flares[100];
  int FlaresQuantity;
  int timeUntilNextFlare;
  int maxDistance;
} FlareHandler;


void initFlareHandler(FlareHandler*);
FlareHandler* createFlareHandler();
void diaplayFlares (FlareHandler*, mat4 *camMatrix);
void CheckForNewFlares (FlareHandler*, User *, mat4);
void MoveAllFlares(FlareHandler*);
void RemoveFlares(FlareHandler*);
void CheckFlaresCollisions (FlareHandler*, Terrain *, Terrain *);

void displayFlaresLight (FlareHandler* lightballhandler, Terrain * terrain);

#endif
