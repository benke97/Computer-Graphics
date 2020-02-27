// Lab 4, terrain generation :)

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <math.h>

mat4 projectionMatrix;
float heightFinder(float xPos, float zPos);
GLfloat *vertexArray;
int texwidth;
int texheight;
Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	texwidth = tex->width;
	texheight = tex->height;
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
			if (x == 0 || x == tex->width || z == 0 || z == tex->height)
			{

			}
			else
			{
				//vec3 P = {1.0,1.0,1.0};
				vec3 N = {1.0,1.0,1.0};
				//P.x = vertexArray[(x + z * tex->width)*3 + 0];
				//P.y = vertexArray[(x + z * tex->width)*3 + 1];
				//P.z = vertexArray[(x + z * tex->width)*3 + 2];

	// read neightbor heights using an arbitrary small offset
				float heightNegZ = vertexArray[(x + (z-1) * tex->width)*3 + 1];
				float heightPosZ = vertexArray[(x + (z+1) * tex->width)*3 + 1];
				float heightNegX = vertexArray[((x-1) + z * tex->width)*3 + 1];
				float heightPosX = vertexArray[((x+1) + z * tex->width)*3 + 1];
	// deduce terrain normal
				N.z = heightNegZ - heightPosZ;
				N.x = heightNegX - heightPosX;
				N.y = 2.0;
				N = Normalize(N);
	// Normal vectors. You need to calculate these.
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


// vertex array object
Model *m, *m2, *tm, *ball, *skybox, *lake;
// Reference to shader program
GLuint program, skyboxshader, waterShader;
GLuint tex1, tex2, balltex, dirttex, skyTex;
TextureData ttex; // terrain
mat4 Rot, Trans, skyMatrix;
GLfloat t;
void init(void)
{
	// GL inits
	glClearColor(0.859,0.957,0.998,1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");
	skybox = LoadModelPlus("skybox.obj");
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 5000.0);
	// Load and compile shader
	program = loadShaders("terrain4-5.vert", "terrain4-5.frag");
	skyboxshader = loadShaders("sb.vert", "sb.frag");
	waterShader = loadShaders("water.vert", "water.frag");

	glUseProgram(program);
	printError("init shader");
	ball = LoadModelPlus("groundsphere.obj");
	lake = LoadModelPlus("ground.obj");
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	glUniform1i(glGetUniformLocation(program, "dirttex"), 1);
	LoadTGATextureSimple("grass.tga", &tex1);
	LoadTGATextureSimple("conc.tga", &balltex);
	LoadTGATextureSimple("dirt.tga", &dirttex);
	LoadTGATextureSimple("SkyBox512.tga", &skyTex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dirttex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, skyTex);
// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	glUseProgram(skyboxshader);
	glUniform1i(glGetUniformLocation(skyboxshader, "texUnit"), 2);
	glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glEnableVertexAttribArray(glGetAttribLocation(skyboxshader, "projectionMatrix"));

	glUseProgram(waterShader);
	glUniformMatrix4fv(glGetUniformLocation(waterShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
}


vec3 cam = {25,10, 100};
vec3 lookAtPoint = {200, 20, 200};
vec3 upVector = {0, 1, 0};
void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	mat4 total, modelView, camMatrix;

	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				upVector.x, upVector.y, upVector.z);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);

	// ###### Skybox ###############
	glUseProgram(skyboxshader);
	skyMatrix = T(cam.x,cam.y,cam.z);
	glDisable(GL_DEPTH_TEST);
	glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "wtvMatrix"), 1, GL_TRUE, total.m);
	glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "mdlMatrix"), 1, GL_TRUE, skyMatrix.m);
	glBindTexture(GL_TEXTURE_2D, skyTex);
	DrawModel(skybox, skyboxshader, "in_Position", NULL , "inTexCoord");
	glEnable(GL_DEPTH_TEST);

	printError("pre display");

	// ###### TERRAIN #################
	glUseProgram(program);
	// Build matrix
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	//glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	//Trans = T(cam.x + 10*cos(t/600), heightFinder(cam.x + 10*cos(t/600), cam.z + 10*sin(t/600)), cam.z + 10*sin(t/600));
	//total = Mult(camMatrix, Trans);
	//glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	//glBindTexture(GL_TEXTURE_2D, balltex);		// Texture for ball
	//DrawModel(ball, program, "inPosition", "inNormal", "inTexCoord");
	printError("display 2");

	// ###### Water #################
	glUseProgram(waterShader);
	glUniform3f(glGetUniformLocation(waterShader, "camPos"), cam.x, cam.y, cam.z);
	modelView = T(0,3.5,0);
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(waterShader, "mdlMatrix"), 1, GL_TRUE, total.m);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawModel(lake, waterShader, "inPosition", "inNormal", "inTexCoord");
	glDisable(GL_BLEND);

	glutSwapBuffers();
}


//########################################
// ######## Declarations################
// ########################################
vec3 side_movement = {0,0,0};
float angle = 0;
float yangle = 0;
//########################################
// ########  ################
// ########################################



//########################################
// ######## FUNKCTIONS ################
// ########################################

void mouse(int x, int y)
{
	//printf("%d %d\n", x, y);
	float diffx = x - (glutGet(GLUT_WINDOW_WIDTH)/2);
	float diffy = y - (glutGet(GLUT_WINDOW_HEIGHT)/2);
	yangle += -diffy/700;

	if (yangle < -M_PI/3)
	{
			yangle = -M_PI/3;
	}
	else if (yangle > M_PI/3)
	{
			yangle = M_PI/3;
	}
	angle += diffx/700;
}

float heightFinder(float xPos, float zPos)
{
  int x = (int)xPos;
	int z = (int)zPos;

	xPos = fmod(xPos, 1.0f);
	zPos = fmod(zPos, 1.0f);

	vec3 v1 = {0.0, 0.0, 0.0};
	vec3 v2 = {0.0, 0.0, 0.0};
	vec3 v3 = {0.0, 0.0, 0.0};

	if (xPos + zPos > 1)
	{

		v1.x = 1;
		v1.y = vertexArray[((x+1) + z * texwidth)*3 + 1];
		v1.z = 0;

		v2.x = 1;
		v2.y = vertexArray[((x+1) + (z+1) * texwidth)*3 + 1];
		v2.z = 1;

		v3.x = 0;
		v3.y = vertexArray[(x + (z+1) * texwidth)*3 + 1];
		v3.z = 1;
	}
	else
	{

		v1.x = 1;
		v1.y = vertexArray[((x+1) + z * texwidth)*3 + 1];
		v1.z = 0;

		v2.x = 0;
		v2.y = vertexArray[(x + z * texwidth)*3 + 1];
		v2.z = 0;

		v3.x = 0;
		v3.y = vertexArray[(x + (z+1) * texwidth)*3 + 1];
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

//########################################
// ########  ################
// ########################################

void timer(int i)
{
	side_movement = ScalarMult(Normalize(CrossProduct(VectorSub(cam, lookAtPoint),upVector)), 0.1);
	if (glutKeyIsDown('s'))
	{
		vec3 a = ScalarMult(Normalize(VectorSub(cam, lookAtPoint)), 0.1);
		cam = VectorAdd(cam, a);
	}
	if (glutKeyIsDown('w'))
	{
		if (glutKeyIsDown(GLUT_KEY_LEFT_SHIFT))
		{
			vec3 a = ScalarMult(Normalize(VectorSub(cam, lookAtPoint)), 0.5);
			cam = VectorSub(cam, a);
		}
		else
		{
			vec3 a = ScalarMult(Normalize(VectorSub(cam, lookAtPoint)), 0.1);
			cam = VectorSub(cam, a);
		}
	}
	if (glutKeyIsDown('a'))
	{
		lookAtPoint = VectorAdd(lookAtPoint ,side_movement);
		cam = VectorAdd(cam, side_movement);
	}
	if (glutKeyIsDown('d'))
	{
		lookAtPoint = VectorSub(lookAtPoint,side_movement);
		cam = VectorSub(cam, side_movement);
	}

	cam.y = heightFinder(cam.x, cam.z) + 2;

	lookAtPoint.x = cam.x + cos(angle)*cos(yangle);
	lookAtPoint.z = cam.z + sin(angle)*cos(yangle);
	lookAtPoint.y = cam.y + sin(yangle);


	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/ 2,glutGet(GLUT_WINDOW_HEIGHT)/ 2);

	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1200, 800);
	glutCreateWindow ("TSBK07 Lab 4");
	//glutFullScreen();
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
