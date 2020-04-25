#ifndef _PARTICLES_H_
#define _PARTICLES_H_
#include "VectorUtils3.h"

#include <stdbool.h>

typedef struct
{
  vec3 position;
  vec3 velocity;
  vec4 color;
  vec4 targetColor;
  GLfloat life;
  GLfloat cameraDistance;
  // Doesn't change, use for reset.
  GLfloat constLife;
  GLfloat size;

} Particle;


void init__Particle(Particle* particle, vec3 position, vec3 velocity, vec4 color, GLfloat life, GLfloat size);
Particle* createParticle(vec3 position, vec3 velocity, vec4 color, GLfloat life, GLfloat size);
void delete__ParticleData(Particle* particle);
void resetParticleLife(Particle particle);
//bool

#endif
