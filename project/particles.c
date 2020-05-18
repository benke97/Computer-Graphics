#include "Particles.h"
#include "GL_utilities.h"


void init__Particle(Particle* particle, vec3 position, vec3 velocity, vec4 color, GLfloat life, GLfloat size)
{
  particle->position = position;
  particle->velocity = velocity;
  vec4 white = {1,1,1,0};
  particle->color = white;
  particle->targetColor = color;
  particle->life = life;
  particle->size = size;
}


Particle* createParticle(vec3 position, vec3 velocity, vec4 color, GLfloat life, GLfloat size)
{
  Particle* result = (Particle*) malloc(sizeof(Particle));
  init__Particle(result, position, velocity, color, life, size);

  return result;
}
