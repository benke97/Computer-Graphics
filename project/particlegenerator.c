#include "ParticleGenerator.h"




void init__ParticleGenerator(ParticleGenerator* particleGen, int maxNrParticles, GLuint* shaderID)
{

  particleGen->shaderID = *shaderID;
  glUseProgram(particleGen->shaderID);
  particleGen->maxNrParticles = maxNrParticles;
  particleGen->particlesCount = 0;
  initParticleVertexBuffer(particleGen->gVertexBuffer);
  initBillboardVertexBuffer(&particleGen->billboardVertexBuffer, particleGen->gVertexBuffer);
  initParticlePostitionsBuffer(&particleGen->particlesPositionBuffer, particleGen->maxNrParticles);
  initParticleColorBuffer(&particleGen->particlesColorBuffer, particleGen->maxNrParticles);

  particleGen->particlePositionSizeData = (GLfloat* )malloc(sizeof(GLfloat) * 4 * maxNrParticles); // pos is 3 floats and size is 1 => 4
  particleGen->particleColorData = (GLfloat* )malloc(sizeof(GLfloat) * 4 * maxNrParticles); // 4 floats: rgba
  // ###!!! SHOULD CALL CONSTRUCTOR FOR EACH PARTICLE IF INIT IS WIERD
  particleGen->particlesContainer = (Particle*)malloc(sizeof(Particle) * maxNrParticles);

  vec3 initPos = {0,0,0};
  vec3 initVel = {0,0,0};
  vec4 color = {1,1,1,1};
  for (int i = 0; i < maxNrParticles; i++)
  {
    particleGen->particlesContainer[i] = *createParticle(initPos, initVel, color, -1, 0);
  }

  // First one will be unused.
  particleGen->lastUsedParticleIndex = 0;

  particleGen->lastTimeDrawCall = 0;

  particleGen->deltaTime = 0;
  printError("Init partgen");
}

ParticleGenerator* createParticleGenerator(int maxNrParticles, GLuint* shaderID)
{
  ParticleGenerator* result = (ParticleGenerator*)malloc(sizeof(ParticleGenerator));
  init__ParticleGenerator(result, maxNrParticles, shaderID);

  return result;
}

void delete_ParticleGeneratorData(ParticleGenerator* particleGen)
{
  // add free to all ponters here
  free(particleGen->particlePositionSizeData);
  free(particleGen->particleColorData);
  free(particleGen->particlesContainer);

  /*
  // Cleanup VBO and shader
	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
  */
}

void initParticleVertexBuffer(GLfloat* vertexbuffer)
{
  //1
  vertexbuffer[0] = -0.5f;
  vertexbuffer[1] = -0.5f;
  vertexbuffer[2] = 0.0f;
  //2
  vertexbuffer[3] = 0.5f;
  vertexbuffer[4] = -0.5f;
  vertexbuffer[5] = 0.0f;
  //3
  vertexbuffer[6] = -0.5f;
  vertexbuffer[7] = 0.5f;
  vertexbuffer[8] = 0.0f;
  //4
  vertexbuffer[9] = 0.5f;
  vertexbuffer[10] = 0.5f;
  vertexbuffer[11] = 0.0f;
  printError("Init partgenvertexbuffer");
}


// ---- Initializers -------------------
void initBillboardVertexBuffer(GLuint* billboardVertexBuffer, GLfloat* vertexbuffer)
{
  glGenBuffers(1, billboardVertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, * billboardVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertexbuffer, GL_STATIC_DRAW);
  printError("Init partgenbillboardbuffer");
}

// Init position buffer, contains the pos and size of each particle
void initParticlePostitionsBuffer(GLuint* particlesPositionBuffer, int maxNrParticles)
{
  glGenBuffers(1, particlesPositionBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, *particlesPositionBuffer);
  // Initialize sizes with empty (NULL) buffer : it will be updated later, each frame.
  glBufferData(GL_ARRAY_BUFFER, maxNrParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

void initParticleColorBuffer(GLuint* particlesColorBuffer, int maxNrParticles)
{
  glGenBuffers(1, particlesColorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, *particlesColorBuffer);
  // Initialize colors with empty (NULL) buffer : it will be updated later, each frame.
  glBufferData(GL_ARRAY_BUFFER, maxNrParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

// ---- Updators -----------------------
void updateParticleSizePositionBuffer(ParticleGenerator* particleGen)
{
  glBindBuffer(GL_ARRAY_BUFFER, particleGen->particlesPositionBuffer);
  glBufferData(GL_ARRAY_BUFFER, particleGen->maxNrParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
  glBufferSubData(GL_ARRAY_BUFFER, 0, particleGen->particlesCount * sizeof(GLfloat) * 4, particleGen->particlePositionSizeData);
}

void updateParticleColorBuffer(ParticleGenerator* particleGen)
{
  glBindBuffer(GL_ARRAY_BUFFER, particleGen->particlesColorBuffer);
  glBufferData(GL_ARRAY_BUFFER, particleGen->maxNrParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
  glBufferSubData(GL_ARRAY_BUFFER, 0, particleGen->particlesCount * sizeof(GLfloat) * 4, particleGen->particleColorData);
}

// ---- Binders --------------------------
void bindBillboardVertexBuffer(ParticleGenerator* particleGen)
{
  //glUseProgram(particleGen->shaderID);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, particleGen->billboardVertexBuffer);
  glVertexAttribPointer(
    0, // attribute. No particular reason for 0, but must match the layout in the shader.
    3, // size
    GL_FLOAT, // type
    GL_FALSE, // normalized?
    0, // stride
    (void*)0 // array buffer offset
    );

    printError("Bind billboard");
}

void bindParticlesPostitionBuffer(ParticleGenerator* particleGen)
{
  //glUseProgram(particleGen->shaderID);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, particleGen->particlesPositionBuffer);
  glVertexAttribPointer
  (
    1, // attribute. No particular reason for 1, but must match the layout in the shader.
    4, // size : x + y + z + size => 4
    GL_FLOAT, // type
    GL_FALSE, // normalized?
    0, // stride
    (void*)0 // array buffer offset
  );
  printError("Bind pos");
}

void bindParticlesColorBuffer(ParticleGenerator* particleGen)
{
  //glUseProgram(particleGen->shaderID);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, particleGen->particlesColorBuffer);
  glVertexAttribPointer
  (
    2, // attribute. No particular reason for 1, but must match the layout in the shader.
    4, // size : r + g + b + a => 4
    GL_FLOAT, // type
    GL_FALSE,
    0, // stride
    (void*)0 // array buffer offset
  );
  printError("Bind col");
}

// ---- Rendering -----------
void sortParticlesByCameraDistance(Particle* particlesContainer)
{

}

// This should most often return emediatly
int findUnusedParticleIndex(ParticleGenerator* particleGen)
{
  for(int i = particleGen->lastUsedParticleIndex; i < particleGen->maxNrParticles; i++)
  {
		if (particleGen->particlesContainer[i].life <= 0)
    {
			particleGen->lastUsedParticleIndex = i;
			return i;
		}
	}

	for(int i = 0; i < particleGen->lastUsedParticleIndex; i++){
		if (particleGen->particlesContainer[i].life <= 0){
			particleGen->lastUsedParticleIndex = i;
			return i;
		}
	}
	return 0; // All particles are taken, override the first one
}

void generateParticles(ParticleGenerator* particleGen, int particlesPerSec, vec3 initialSpeed, vec3 initialPostition, vec4 initialColor, float particleSpread, GLfloat initialSize, GLfloat initialLife)
{
  // Delta in ms
  GLfloat currentTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
  if (particleGen->lastTimeDrawCall == 0)
  {
    //First draw call. Then set delta to 60fps 16ms
    particleGen->deltaTime = 16.0f;
  }
  else
  {
    // Calc difference
    particleGen->deltaTime = currentTime - particleGen->lastTimeDrawCall;
    particleGen->lastTimeDrawCall = currentTime;
  }

  // How many particles to Generate. Delta/1000 to get seconds
  int nrNewParticles = (int)((particleGen->deltaTime / 1000) * particlesPerSec);
  //printf("New particles %d\n", nrNewParticles);
  // Create all particles
  for (int i = 0; i < nrNewParticles; i++)
  {
    // Get unused particle and reset life if dead.
    int particleIndex = findUnusedParticleIndex(particleGen);
    particleGen->particlesContainer[particleIndex].life = initialLife;

    // Set position to provided(i.e if spawned on moving object)
    particleGen->particlesContainer[particleIndex].position = initialPostition;

    // Set speed to random plus the initial speed if moving object.
    vec3 speedDir;
    speedDir.x = rand() % 100;
    speedDir.y = rand() % 100;
    speedDir.z = rand() % 100;
    Normalize(speedDir);
    //particleGen->particlesContainer[particleIndex].velocity = VectorAdd(initialSpeed, ScalarMult(speedDir, particleSpread*0.1f));

    //Set color to initcolor
    particleGen->particlesContainer[particleIndex].color = initialColor;

    // Set size
    particleGen->particlesContainer[particleIndex].size = initialSize;


  }
}

void simulateAllParticles(ParticleGenerator* particleGen)
{
  particleGen->particlesCount = 0;
  for (int i = 0; i < particleGen->maxNrParticles; i++)
  {
    // Extract curr particle
    Particle p = particleGen->particlesContainer[i];
    //printf("LIFE %f\n", p.life);
    if (p.life > 0)
    {


    // Decrease life, tweak later
    p.life = p.life - particleGen->deltaTime*10;

      // Only simulate if alive.
      if (p.life > 0)
      {
        // -- Update data arrays
        // Simple gravity sim.
        //vec3 gravity = {0.0f,-9.81f, 0.0f};
        //p.velocity = VectorAdd(p.velocity, ScalarMult(gravity, (float)particleGen->deltaTime * 0.5f));
  			//p.position = VectorAdd(p.position, ScalarMult(p.velocity, (float)particleGen->deltaTime));
        // ###!!! Add user as input var later to calc cameraDistance for now set to 1
        p.cameraDistance = 1;
  			// p.cameradistance = glm::length2( p.pos - CameraPosition );

        // Also decrease size here, Tweak later
        //p.size = p.size - particleGen->deltaTime;

        // Add particle data to position and size data array
        particleGen->particlePositionSizeData[4 * particleGen->particlesCount + 0] = p.position.x;
  			particleGen->particlePositionSizeData[4 * particleGen->particlesCount + 1] = p.position.y;
  			particleGen->particlePositionSizeData[4 * particleGen->particlesCount + 2] = p.position.z;
  			particleGen->particlePositionSizeData[4 * particleGen->particlesCount + 3] = p.size;

        // Set color data, we can alse make colorchange over time if we like
        particleGen->particleColorData[4 * particleGen->particlesCount + 0] = p.color.x; //r
  		  particleGen->particleColorData[4 * particleGen->particlesCount + 1] = p.color.y; //g
  			particleGen->particleColorData[4 * particleGen->particlesCount + 2] = p.color.z; //b
  			particleGen->particleColorData[4 * particleGen->particlesCount + 3] = p.color.w; //a

      }
      else //Else set cameradistance to -1 and the sortingfunction will put dead particles in the end of vector.
      {
        p.life = -1;
        p.cameraDistance = -1; // sort these to end in sortingfunction
        printf("Particle died\n");
      }

      particleGen->particlesCount++;
    }
  }
  // Sorts particles by cameraDistance for renderorder.
  sortParticlesByCameraDistance(particleGen->particlesContainer);

  printf("particle count %d\n", particleGen->particlesCount);
}

void drawAllParticles(ParticleGenerator* particleGen, mat4* camMatrix, mat4 projectionMatrix)
{
  mat4 WTVMatrix = *camMatrix;
  // Use our shader
	glUseProgram(particleGen->shaderID);
  // Update buffers for Gpu
  updateParticleSizePositionBuffer(particleGen);

  updateParticleColorBuffer(particleGen);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  printError("Update error");

/* For textures
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);
*/

  // This to face quads to camera
  glUseProgram(particleGen->shaderID);
  // Same as the billboards tutorial
	//glUniform3f(glGetUniformLocation(particleGen->shaderID, "CamRightWorldSpace"), WTVMatrix.m[0], WTVMatrix.m[4], WTVMatrix.m[8]); // first colonn
	//glUniform3f(glGetUniformLocation(particleGen->shaderID, "CamUpWorldSpace"), WTVMatrix.m[1], WTVMatrix.m[5], WTVMatrix.m[9]); // second colonn


	glUniformMatrix4fv(glGetUniformLocation(particleGen->shaderID, "ProjectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glUniformMatrix4fv(glGetUniformLocation(particleGen->shaderID, "WTVMatrix"), 1, GL_TRUE, WTVMatrix.m);

  printError("Uniform ERROR");
  // Bind buffers to shader

  bindBillboardVertexBuffer(particleGen);
  bindParticlesPostitionBuffer(particleGen);
  bindParticlesColorBuffer(particleGen);
  printError("bind ErROR");


  // -- Draw
  // This is for draw instanced, first arg is id (see bind functions above)
  //Second is how many times repeated per particle

  glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 0); // color : one per quad                                  -> 1

  // Draw function
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleGen->particlesCount);

  glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


}
