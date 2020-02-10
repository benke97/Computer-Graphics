// Lab 1-6.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <math.h>
#include "loadobj.h"
#include "LoadTGA.h"

// Globals
// Data would normally be read from files
#define near 1.0
#define far 80.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat projectionMatrix[] = {2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                              0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                              0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                              0.0f, 0.0f, -1.0f, 0.0f };

void SetZRotationMatrix(GLfloat a, GLfloat *m)
{
	m[0] = cos(a); m[1] = -sin(a); m[2] = 0.0; m[3] = 0.0;
	m[4] = sin(a); m[5] = cos(a); m[6] = 0.0; m[7] = 0.0;
	m[8] = 0.0; m[9] = 0.0; m[10] = 1.0; m[11] = 0.0;
	m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
};

void SetYRotationMatrix(GLfloat a, GLfloat *m)
{
	m[0] = cos(a); m[1] = 0; m[2] = sin(a); m[3] = 0.0;
	m[4] = 0; m[5] = 1; m[6] = 0.0; m[7] = 0.0;
	m[8] = -sin(a); m[9] = 0.0; m[10] = cos(a); m[11] = 0.0;
	m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
};

void SetXRotationMatrix(GLfloat a, GLfloat *m)
{
	m[0] = 1.0; m[1] = 0.0; m[2] = 0.0; m[3] = 0.0;
	m[4] = 0.0; m[5] = cos(a); m[6] = -sin(a); m[7] = 0.0;
	m[8] = 0.0; m[9] = sin(a); m[10] = cos(a); m[11] = 0;
	m[12] = 0.0; m[13] = 0.0; m[14] = 0; m[15] = 1;
};

void SetY3RotationMatrix(GLfloat a, GLfloat *m)
{
	m[0] = cos(a); m[1] = 0; m[2] = sin(a);
	m[3] = 0.0; m[4] = 1; m[5] = 0;
	m[6] = -sin(a); m[7] = 0.0; m[8] = cos(a);
};

//______________

float angle = 0;
vec3 p = {10, 8, 10};
vec3 l = {0,0,0};
vec3 v = {0,1,0};
float distance = 0;

void OnTimer(int value)
{

    glutPostRedisplay();
		void LoadTGATextureSimple(char *filename, GLuint *tex);

		if (glutKeyIsDown('s'))
		{
			vec3 a = VectorSub(p, l);
			p = VectorAdd(p, Normalize(a));
		}
		if (glutKeyIsDown('w'))
		{
			vec3 a = VectorSub(p, l);
			p = VectorSub(p, Normalize(a));
		}
		if (glutKeyIsDown('a'))
		{
			distance = Norm(VectorSub(p,l));
			angle -= M_PI/50;
		}
		if (glutKeyIsDown('d'))
		{
			distance = Norm(VectorSub(p,l));
			angle += M_PI/50;
		}
		l.x = p.x + cos(angle);
		l.z = p.z + sin(angle);


    glutTimerFunc(20, &OnTimer, value);

}



// vertex array object
GLuint program;
Model *m;
Model *m2;
Model *m3;
Model *m4;
Model *m5;
Model *m6;
Model *m7;
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;
GLuint myTex;
GLuint my2Tex;
mat4 rot1, rot2, trans, total, wtvMatrix, commonBladeRot, worldToViewMatrix, modelToWorldMatrix;
mat4 camRot;

void init(void)
{
	m = LoadModel("windmill/windmill-walls.obj");
	m2 = LoadModelPlus("windmill/windmill-balcony.obj");
	m3 = LoadModelPlus("windmill/windmill-roof.obj");
	m4 = LoadModelPlus("windmill/blade.obj");
	m5 = LoadModelPlus("windmill/blade.obj");
	m6 = LoadModelPlus("windmill/blade.obj");
	m7 = LoadModelPlus("windmill/blade.obj");
	dumpInfo();

	// GL inits
	glClearColor(0.6,0.5,0.9,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");
	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");
	// End of upload of geometry
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
  glGenBuffers(1, &bunnyVertexBufferObjID);
  glGenBuffers(1, &bunnyIndexBufferObjID);
  glGenBuffers(1, &bunnyNormalBufferObjID);
	glGenBuffers(1, &bunnyTexCoordBufferObjID);

	LoadTGATextureSimple("conc.tga", &myTex);
	LoadTGATextureSimple("maskros512.tga", &my2Tex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

	glBindVertexArray(bunnyVertexArrayObjID);
	if (m->texCoordArray != NULL)

	    {

	        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);

	        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);

	        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);

	        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));

	    }
     // VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

     // VBO for normal data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glEnableVertexAttribArray(glGetAttribLocation(program, "projectionMatrix"));

	printError("init arrays");
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

void display(void)
{
	printError("pre display");
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO

  //p = {20*cos(angle), 8, 20*sin(angle)};
	wtvMatrix = lookAtv(p, l, v);
	//BASE
	trans = T(0,0,0);
	rot1 = Rx(0);
	total = Mult(trans, rot1);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "wtvMatrix"), 1, GL_TRUE, wtvMatrix.m);

	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
	//ROOF
	rot2 = Rx(0);
	trans = T(0,0,0);
	total = Mult(trans, rot2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindTexture(GL_TEXTURE_2D, my2Tex);
	DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");
	glUniform1f(glGetUniformLocation(program, "t"), t);

	commonBladeRot = Rx(t/1000);

	rot2 = Rx(0);
	trans = T(0,0,0);
	total = Mult(trans, rot2);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindTexture(GL_TEXTURE_2D, my2Tex);
	DrawModel(m3, program, "in_Position", "in_Normal", "inTexCoord");


	//BLADES
	rot2 = Rx(0);
	trans = T(4.5,9.5,0);
	total = Mult(commonBladeRot, rot2);
	total = Mult(trans, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindTexture(GL_TEXTURE_2D, my2Tex);
	DrawModel(m4, program, "in_Position", "in_Normal", "inTexCoord");

	rot2 = Rx(M_PI/2);
	trans = T(4.5,9.5,0);
	total = Mult(commonBladeRot, rot2);
	total = Mult(trans, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindTexture(GL_TEXTURE_2D, my2Tex);
	DrawModel(m5, program, "in_Position", "in_Normal", "inTexCoord");

	rot2 = Rx(M_PI);
	trans = T(4.5,9.5,0);
	total = Mult(commonBladeRot, rot2);
	total = Mult(trans, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindTexture(GL_TEXTURE_2D, my2Tex);
	DrawModel(m6, program, "in_Position", "in_Normal", "inTexCoord");

	rot2 = Rx(3*M_PI/2);
	trans = T(4.5,9.5,0);
	total = Mult(commonBladeRot, rot2);
	total = Mult(trans, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindTexture(GL_TEXTURE_2D, my2Tex);
	DrawModel(m7, program, "in_Position", "in_Normal", "inTexCoord");

	printError("display");

	glutSwapBuffers();
}

int prevx = 0, prevy = 0;


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
