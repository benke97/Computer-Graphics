
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

typedef struct Terrain {
  GLfloat *vertexArray;
  GLfloat *normalArray;
  int texwidth;
  int texheight;
  TextureData texData;
  Model *terrainModel;

  GLuint shader;
  GLuint terrain_texture;
  bool is_floor;
} Terrain;

void initTerrain(Terrain* this, mat4 projectionMatrix, bool is_floor);
Terrain* createTerrain(mat4 projectionMatrix, bool is_floor);
Model* GenerateTerrain(TextureData* tex, Terrain * terrain, int height, int sign);
float heightFinder(float xPos, float zPos, Terrain* terrain);
float heightdiff(float xPos, float zPos, Terrain* roof, Terrain* floor);
float slidedown(float xPos1, float zPos1, float xPos2, float zPos2, Terrain* floor);
vec3 getNormal(float xPos, float zPos, Terrain* floor);
void displayTerrain(Terrain * floor, Terrain * roof, float specularExponent, vec3 cam, mat4 * camMatrix);
#endif
