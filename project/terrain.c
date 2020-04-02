#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


//typedef struct {} Terrain;
Model* GenerateTerrain();
GLfloat *vertexArray;


typedef struct Terrain {
  int texwidth;
  int texheight;

  TextureData ttex; // terrain

  Model *tm;
  // Reference to shader this->shader
  GLuint shader;
  GLuint tex1, dirttex;
} Terrain;





void initTerrain(Terrain* this, mat4 projectionMatrix){
  this->shader = loadShaders("shaders/terrain4-5.vert", "shaders/terrain4-5.frag");

  glUseProgram(this->shader);
  printError("init shader");
  glUniformMatrix4fv(glGetUniformLocation(this->shader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glUniform1i(glGetUniformLocation(this->shader, "tex"), 0); // Texture unit 0
  glUniform1i(glGetUniformLocation(this->shader, "dirttex"), 1);
  LoadTGATextureSimple("textures/grass.tga", &this->tex1);
  LoadTGATextureSimple("textures/dirt.tga", &this->dirttex);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->tex1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, this->dirttex);

  LoadTGATextureData("textures/test3.tga", &this->ttex);
  this->tm = GenerateTerrain(&this->ttex, this);
  printError("init terrain");
}

Terrain* createTerrain(mat4 projectionMatrix){
  printf("%d", 5);
  Terrain* result = (Terrain*) malloc(sizeof(Terrain));
  initTerrain(result, projectionMatrix);
  printf("%d", 5);
  return result;
}



Model* GenerateTerrain(TextureData* tex, Terrain * terrain)
{
  //TextureData* tex = &this->ttex;
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	terrain->texwidth = tex->width;
	terrain->texheight = tex->height;
  //GLfloat * vertexArray = this->vertexArray;

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x /1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)]/ 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
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
				normalArray[(x + z * tex->width)*3 + 0] = 0;
				normalArray[(x + z * tex->width)*3 + 1] = 1;
				normalArray[(x + z * tex->width)*3 + 2] = 0;
			}
			else
			{

				vec3 N = {1.0,1.0,1.0};

	// read neightbor heights
				float heightNegZ = vertexArray[(x + (z-1) * tex->width)*3 + 1];
				float heightPosZ = vertexArray[(x + (z+1) * tex->width)*3 + 1];
				float heightNegX = vertexArray[((x-1) + z * tex->width)*3 + 1];
				float heightPosX = vertexArray[((x+1) + z * tex->width)*3 + 1];
	// deduce terrain normal
				N.z = heightNegZ - heightPosZ;
				N.x = heightNegX - heightPosX;
				N.y = 2.0;
				N = Normalize(N);
	// Normal vectors.
				normalArray[(x + z * tex->width)*3 + 0] = N.x;
				normalArray[(x + z * tex->width)*3 + 1] = N.y;
				normalArray[(x + z * tex->width)*3 + 2] = N.z;
			}
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}
