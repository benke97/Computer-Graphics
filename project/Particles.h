#ifndef _PARTICLES_H_
#define _PARTICLES_H_
#include "VectorUtils3.h"

typedef struct
{
  vec3 position;
  vec3 velocity;
  vec3 color;
  GLfloat life;


} Particle;


void init__Particle(Particle* particle, vec3 position, vec3 velocity, vec3 color, GLfloat life);
Particle* spawnParticle(vec3 position, vec3 velocity, vec3 color, GLfloat life);

#endif
