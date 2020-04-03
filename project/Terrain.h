
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "VectorUtils3.h"

#include "loadobj.h"
#include "LoadTGA.h"
//typedef struct {} Terrain;

typedef struct Terrain {
  GLfloat *vertexArray;
  int texwidth;
  int texheight;

  TextureData ttex; // terrain

  Model *tm;
  // Reference to shader this->shader
  GLuint shader;
  GLuint tex1, dirttex;
} Terrain;

void initTerrain(Terrain* this, mat4 projectionMatrix, int heightmap, int height);
Terrain* createTerrain(mat4 projectionMatrix,int heightmap,int height);
Model* GenerateTerrain(TextureData* tex, Terrain * terrain, int height);
float heightFinder(float xPos, float zPos, int texwith, Terrain* terrain);

#endif
