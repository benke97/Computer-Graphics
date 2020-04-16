#include "Particles.h"

#include "GL_utilities.h"


void init__Particle(Particle* particle, vec3 position, vec3 velocity, vec3 color, GLfloat life)
{
  particle->position = position;
  particle->velocity = velocity;
  particle->color = color;
  particle->life = life;
}


Particle* spawnParticle(vec3 position, vec3 velocity, vec3 color, GLfloat life)
{
  Particle* result = (Particle*) malloc(sizeof(Particle));
  init__Particle(result, position, velocity, color, life);

  return result;
}
