
#include "Terrain.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initTerrain(Terrain* this, mat4 projectionMatrix, bool is_floor){
  this->shader = loadShaders("shaders/terrain4-5.vert", "shaders/terrain4-5.frag");
  this->is_floor = is_floor;

  glUseProgram(this->shader);
  printError("init shader");

  glUniformMatrix4fv(glGetUniformLocation(this->shader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glUniform1i(glGetUniformLocation(this->shader, "terrain_texture"), 0);
  LoadTGATextureSimple("textures/dirt.tga", &this->terrain_texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->terrain_texture);
  if (this->is_floor)
  {
  	LoadTGATextureData("textures/floor.tga", &this->texData);
    int height = 0;
  	this->terrainModel = GenerateTerrain(&this->texData, this, height, 1);
  }
  else
  {
  	LoadTGATextureData("textures/roof.tga", &this->texData);
    int height = 20;
  	this->terrainModel = GenerateTerrain(&this->texData, this, height, -1);
  }
  printError("init terrain");
}

Terrain* createTerrain(mat4 projectionMatrix, bool is_floor){

  Terrain* result = (Terrain*) malloc(sizeof(Terrain));
  initTerrain(result, projectionMatrix, is_floor);

  return result;
}



Model* GenerateTerrain(TextureData* tex, Terrain * terrain, int height, int sign)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	terrain->texwidth = tex->width;
	terrain->texheight = tex->height;

	terrain->vertexArray = (GLfloat *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	terrain->normalArray = (GLfloat *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			terrain->vertexArray[(x + z * tex->width)*3 + 0] = x /1.0;
			terrain->vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)]/ 10.0 + height;
			terrain->vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			terrain->normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			terrain->normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			terrain->normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			if (x < 1 || x > tex->width - 1 || z < 1 || z > tex->height - 1)
			{
				terrain->normalArray[(x + z * tex->width)*3 + 0] = 0;
				terrain->normalArray[(x + z * tex->width)*3 + 1] = 1;
				terrain->normalArray[(x + z * tex->width)*3 + 2] = 0;
			}
			else
			{

				vec3 N = {1.0,1.0,1.0};

	// read neightbor heights
				float heightNegZ = terrain->vertexArray[(x + (z-1) * tex->width)*3 + 1];
				float heightPosZ = terrain->vertexArray[(x + (z+1) * tex->width)*3 + 1];
				float heightNegX = terrain->vertexArray[((x-1) + z * tex->width)*3 + 1];
				float heightPosX = terrain->vertexArray[((x+1) + z * tex->width)*3 + 1];
	// deduce terrain normal
				N.z = heightNegZ - heightPosZ;
				N.x = heightNegX - heightPosX;
				N.y = 2.0;
				N = Normalize(N);
	// Normal vectors.
				terrain->normalArray[(x + z * tex->width)*3 + 0] = N.x*sign;
				terrain->normalArray[(x + z * tex->width)*3 + 1] = N.y*sign;
				terrain->normalArray[(x + z * tex->width)*3 + 2] = N.z*sign;
			}
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			terrain->vertexArray,
			terrain->normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


float heightFinder(float xPos, float zPos, Terrain* terrain)
{
  int x = (int)xPos;
	int z = (int)zPos;
  int texwidth = terrain->texwidth;

	//GLfloat * vertexArray = terrain->vertexArray;
	//int texwidth = terrain->texwidth;

	xPos = fmod(xPos, 1.0f);
	zPos = fmod(zPos, 1.0f);

	vec3 v1 = {0.0, 0.0, 0.0};
	vec3 v2 = {0.0, 0.0, 0.0};
	vec3 v3 = {0.0, 0.0, 0.0};

	if (xPos + zPos > 1)
	{

		v1.x = 1;
		v1.y = terrain->vertexArray[((x+1) + z * texwidth)*3 + 1];
		v1.z = 0;

		v2.x = 1;
		v2.y = terrain->vertexArray[((x+1) + (z+1) * texwidth)*3 + 1];
		v2.z = 1;

		v3.x = 0;
		v3.y = terrain->vertexArray[(x + (z+1) * texwidth)*3 + 1];
		v3.z = 1;
	}
	else
	{

		v1.x = 1;
		v1.y = terrain->vertexArray[((x+1) + z * texwidth)*3 + 1];
		v1.z = 0;

		v2.x = 0;
		v2.y = terrain->vertexArray[(x + z * texwidth)*3 + 1];
		v2.z = 0;

		v3.x = 0;
		v3.y = terrain->vertexArray[(x + (z+1) * texwidth)*3 + 1];
		v3.z = 1;
	}

	// Barycentric
	float w1 = ((v2.z - v3.z)*(xPos - v3.x) + (v3.x - v2.x)*(zPos - v3.z))
			/ ((v2.z - v3.z)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.z - v3.z));

	float w2 = ((v3.z - v1.z)*(xPos - v3.x) + (v1.x - v3.x)*(zPos - v3.z))
			/ ((v2.z - v3.z)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.z - v3.z));

	float w3 = 1.0 - w2 - w1;

	return (w1*v1.y + w2*v2.y + w3*v3.y);
}

float heightdiff(float xPos, float zPos, Terrain* roof, Terrain* floor)
{
  float roofheight = heightFinder(xPos,zPos,roof);
  float floorheight = heightFinder(xPos,zPos,floor);
  return roofheight - floorheight;
}

float slidedown(float xPos1, float zPos1, float xPos2, float zPos2, Terrain* floor)
{
  if (heightFinder(xPos1,zPos1, floor) < heightFinder(xPos2, zPos2, floor))
  {
   return true;
  }
  return false;
}

vec3 getNormal(float xPos, float zPos, Terrain* terrain)
{
  int texwidth = terrain->texwidth;
  vec3 norm = {0.0, 0.0, 0.0};
  int x = round(xPos);
  int z = round(zPos);
  norm.x = terrain->normalArray[(x + z * texwidth)*3 + 0];
  norm.y = terrain->normalArray[(x + z * texwidth)*3 + 1];
  norm.z = terrain->normalArray[(x + z * texwidth)*3 + 2];
  return norm;
}

void displayTerrain(Terrain * floor, Terrain * roof, vec3 cam, mat4 * camMatrix) {
  glUseProgram(floor->shader);
  mat4 modelView = IdentityMatrix();
  mat4 total = Mult(*camMatrix, modelView);

	glUniform3f(glGetUniformLocation(floor->shader, "camPos"), cam.x, cam.y, cam.z);

  glUniformMatrix4fv(glGetUniformLocation(floor->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  glBindTexture(GL_TEXTURE_2D, floor->terrain_texture);
  DrawModel(floor->terrainModel, floor->shader, "inPosition", "inNormal", "inTexCoord");

  glBindTexture(GL_TEXTURE_2D, roof->terrain_texture);
  glUniformMatrix4fv(glGetUniformLocation(roof->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(roof->terrainModel, roof->shader, "inPosition", "inNormal", "inTexCoord");
}
