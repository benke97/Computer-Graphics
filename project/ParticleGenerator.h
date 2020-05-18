#ifndef _PARTICLEGENERATOR_H_
#define _PARTICLEGENERATOR_H_

#include "VectorUtils3.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "Particles.h"
#include "User.h"

// ParticleGenerator object
typedef struct
{
  int maxNrParticles;
  int particlesCount; // Current nr of particles
  GLfloat particleVertices[12]; // 4 vertices for each particle.
  GLuint billboardVertexBuffer; // Buffer for quad/billboard of vertices. Can be thought of as the billboard object
  GLuint particlesPositionBuffer; // Buffer for World positions of particle to be drawn, updated with particlePositionSizeData array below.
  GLuint particlesColorBuffer; // Buffer for Colors of particle to be drawn, updated with particleColorData array below.

  GLfloat* particlePositionSizeData; // Position and size array that is updated from each particlestruct
  GLfloat* particleColorData; // Color array that is updated from each particlestruct
  Particle* particlesContainer; // Holds maxNrParticles amount of particles to be used

  int lastUsedParticleIndex; // Used to faster iteration through particlesContainer when finding a Unused Particle
  GLfloat lastTimeDrawCall; // Helps to calculate deltaTime
  GLfloat deltaTime; // Is used for movement, colorchange etc.
  GLuint shaderID; // Shader ID for particlegenerator
  GLuint particleTexture; // The texture all particles have for this particle generator

} ParticleGenerator;

// Init shaders before and pass shaderID
void init__ParticleGenerator(ParticleGenerator* particleGen, int maxNrParticles, GLuint* shaderID, char* textureFile);
ParticleGenerator* createParticleGenerator(int maxNrParticles, GLuint* shaderID, char* textureFile);
void delete_ParticleGeneratorData(ParticleGenerator* particleGen);

void initParticleVertices(GLfloat* vertexbuffer);
void initBillboardVertexBuffer(GLuint* billboardVertexBuffer, GLfloat* vertexbuffer);
void initParticlePostitionsBuffer(GLuint* particlesPositionBuffer, int maxNrParticles);
void initParticleColorBuffer(GLuint* particlesColorBuffer, int maxNrParticles);

void updateParticleSizePositionBuffer(ParticleGenerator* particleGen);
void updateParticleColorBuffer(ParticleGenerator* particleGen);

void bindBillboardVertexBuffer(ParticleGenerator* particleGen);
void bindParticlesPostitionBuffer(ParticleGenerator* particleGen);
void bindParticlesColorBuffer(ParticleGenerator* particleGen);

void sortParticlesByCameraDistance(Particle* particlesContainer, int size);
int findUnusedParticleIndex(ParticleGenerator* particleGen);

// Set particlesPerSec to 10000. Particle spread to 1.5f. And tweak values from there.

void generateParticles(ParticleGenerator* particleGen, int particlesPerSec, vec3 initialSpeed,
                        vec3 initialPostition, GLfloat radius, vec4 targetColor, float particleSpread,
                        GLfloat initialSize, GLfloat initialLifeInSeconds);

void simulateAllParticles(ParticleGenerator* particleGen, User* user, int gravityOn);
void drawAllParticles(ParticleGenerator* particleGen, mat4* camMatrix, mat4 projectionMatrix);

#endif
