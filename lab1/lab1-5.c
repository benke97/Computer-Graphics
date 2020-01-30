// Lab 1-1.
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

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	//Bottom
	-0.5f, -0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	//Bottom
	0.5f, 0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	//1
	0.0f, 0.0f,0.5f,
	0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	//2
	0.0f, 0.0f,0.5f,
	-0.5f,0.5f,0.0f,
	0.5f,0.5f,0.0f,
	//3
	0.0f, 0.0f,0.5f,
	-0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	//4
	0.0f, 0.0f,0.5f,
	0.5f,-0.5f,0.0f,
	-0.5f,-0.5f,0.0f,

};

GLfloat colors[] =
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	0.1f, 0.2f, 0.3f,
	0.0f, 1.0f, 0.5f,
	0.0f, 0.0f, 1.0f,

	0.1f, 0.2f, 0.3f,
	0.5f, 1.0f, 0.0f,
	0.0f, 0.8f, 1.0f,

	0.1f, 0.2f, 0.3f,
	0.3f, 1.0f, 0.0f,
	0.0f, 0.2f, 0.1f,

	0.1f, 0.2f, 0.3f,
	0.0f, 1.0f, 0.0f,
	0.7f, 0.0f, 0.5f
};

GLfloat myMatrix[] = {	1.0f, 0.0f, 0.0f, 0.5f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f	};



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


void OnTimer(int value)

{

    glutPostRedisplay();

    glutTimerFunc(20, &OnTimer, value);

}

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	// Reference to shader program
	GLuint program;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	//glGenVertexArrays(1, &colorArrayObjID);
	//glBindVertexArray(colorArrayObjID);
	glGenBuffers(1, &colorBufferObjID);
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 54*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 54*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_color"));

	/*GLfloat rotMatrix[] = {	0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f };*/

	//SetXRotationMatrix(0, rotMatrix);
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotMatrix"), 1, GL_TRUE, rotMatrix);


	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	// End of upload of geometry

	printError("init arrays");
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLuint program;
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	GLfloat rotMatrix[] = {	0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f };

	SetYRotationMatrix(t/1000, rotMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotMatrix"), 1, GL_TRUE, rotMatrix);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	//glBindVertexArray(colorArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, 18);	// draw object

	printError("display");

	glutSwapBuffers();
}

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
