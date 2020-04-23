#ifndef _PARTICLEGENERATOR_H_
#define _PARTICLEGENERATOR_H_

#include "VectorUtils3.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "Particles.h"

// ParticleGenerator object
typedef struct
{
  int maxNrParticles;
  // Holds current nr of particles
  int particlesCount;
  // Vertices for each particle.
  GLfloat gVertexBuffer[12];
  GLuint billboardVertexBuffer;
  GLuint particlesPositionBuffer;
  GLuint particlesColorBuffer;

  GLfloat* particlePositionSizeData;
  GLfloat* particleColorData;
  Particle* particlesContainer;

  int lastUsedParticleIndex;

  GLfloat lastTimeDrawCall;

  GLfloat deltaTime;

  GLuint shaderID;

} ParticleGenerator;

// Init shaders before and pass shaderID
void init__ParticleGenerator(ParticleGenerator* particleGen, int maxNrParticles, GLuint* shaderID);
ParticleGenerator* createParticleGenerator(int maxNrParticles, GLuint* shaderID);
void delete_ParticleGeneratorData(ParticleGenerator* particleGen);

void initParticleVertexBuffer(GLfloat* vertexbuffer);
void initBillboardVertexBuffer(GLuint* billboardVertexBuffer, GLfloat* vertexbuffer);
void initParticlePostitionsBuffer(GLuint* particlesPositionBuffer, int maxNrParticles);
void initParticleColorBuffer(GLuint* particlesColorBuffer, int maxNrParticles);

void updateParticleSizePositionBuffer(ParticleGenerator* particleGen);
void updateParticleColorBuffer(ParticleGenerator* particleGen);

void bindBillboardVertexBuffer(ParticleGenerator* particleGen); 
void bindParticlesPostitionBuffer(ParticleGenerator* particleGen);
void bindParticlesColorBuffer(ParticleGenerator* particleGen);

void sortParticlesByCameraDistance(Particle* particlesContainer);
int findUnusedParticleIndex(ParticleGenerator* particleGen);

// Set particlesPerSec to 10000. Particle spread to 1.5f. And tweak values from there.
void generateParticles(ParticleGenerator* particleGen, int particlesPerSec, vec3 initialSpeed, vec3 initialPostition, vec4 initialColor, float particleSpread, GLfloat initialSize, GLfloat initialLife);
void simulateAllParticles(ParticleGenerator* particleGen);
void drawAllParticles(ParticleGenerator* particleGen, mat4* camMatrix, mat4 projectionMatrix);

#endif
