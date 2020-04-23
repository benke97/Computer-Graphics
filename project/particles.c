#include "Particles.h"
#include "GL_utilities.h"


void init__Particle(Particle* particle, vec3 position, vec3 velocity, vec4 color, GLfloat life, GLfloat size)
{
  particle->position = position;
  particle->velocity = velocity;
  particle->color = color;
  particle->life = life;
  particle->constLife = life;
  particle->size = size;
}


Particle* createParticle(vec3 position, vec3 velocity, vec4 color, GLfloat life, GLfloat size)
{
  Particle* result = (Particle*) malloc(sizeof(Particle));
  init__Particle(result, position, velocity, color, life, size);

  return result;
}

void delete__ParticleData(Particle* particle)
{
  // add pointers if needed. Don't use pointers in this class because have to call delete in particlegen destructor.
}

void resetParticleLife(Particle particle)
{
  particle.life = particle.constLife;
}
