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

mat4 projectionMatrix;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x /1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)]/ 100.0;
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
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);

// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}


vec3 cam = {0, 5, 8};
vec3 lookAtPoint = {2, 0, 2};
vec3 upVector = {0, 1, 0};
void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, modelView, camMatrix;

	printError("pre display");

	glUseProgram(program);

	// Build matrix
	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				upVector.x, upVector.y, upVector.z);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

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
	float diffy = y - (glutGet(GLUT_WINDOW_WIDTH)/2);
	if (diffy > 0 && yangle > -M_PI/3)
	{
			yangle -= M_PI/200;
	}
	else if ( diffy < 0 && yangle < M_PI/3)
	{
			yangle += M_PI/200;
	}

	if (diffx > 0)
	{
		angle += M_PI/200;
	}
	else if (diffx < 0)
	{
		angle -= M_PI/200;
	}
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
		vec3 a = ScalarMult(Normalize(VectorSub(cam, lookAtPoint)), 0.1);
		cam = VectorSub(cam, a);
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
	lookAtPoint.x = cam.x + cos(angle);
	lookAtPoint.z = cam.z + sin(angle);
	lookAtPoint.y = cam.y + tan(yangle);

	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH)/ 2,glutGet(GLUT_WINDOW_HEIGHT)/ 2);

	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
