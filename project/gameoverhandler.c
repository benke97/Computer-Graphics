#include "Gameoverhandler.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <math.h>
#include "GL_utilities.h"


void initGameOverHandler(GameOverHandler* gameoverhandler, mat4 projectionMatrix) {
  gameoverhandler->skybox = LoadModelPlus("models/skybox.obj");
  gameoverhandler->gameovermodel = LoadModelPlus("models/gameover.obj");
  gameoverhandler->skyboxshader = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");
  LoadTGATextureSimple("textures/SkyBox512.tga", &gameoverhandler->skyTex);
  gameoverhandler->shader = loadShaders("shaders/gameover.vert", "shaders/gameover.frag");

  glUseProgram(gameoverhandler->shader);
  glUniformMatrix4fv(glGetUniformLocation(gameoverhandler->shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glEnableVertexAttribArray(glGetAttribLocation(gameoverhandler->shader, "projectionMatrix"));
};


GameOverHandler* createGameOverHandler(mat4 projectionMatrix) {
  GameOverHandler* result = (GameOverHandler*) malloc(sizeof(GameOverHandler));
  initGameOverHandler(result, projectionMatrix);
  return result;
};

void HandleGameOver(GameOverHandler* gameoverhandler, User* user, mat4 projectionMatrix) {


  user->cam = SetVector(25,2,-100);

  glDisable(GL_DEPTH_TEST);

  glUseProgram(gameoverhandler->skyboxshader);
  glUniform1i(glGetUniformLocation(gameoverhandler->skyboxshader, "texUnit"), 0);
  glUniformMatrix4fv(glGetUniformLocation(gameoverhandler->skyboxshader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glEnableVertexAttribArray(glGetAttribLocation(gameoverhandler->skyboxshader, "projectionMatrix"));

  mat4 skyMatrix = T(user->cam.x,user->cam.y,user->cam.z);
  mat4 camMatrix = lookAt(user->cam.x, user->cam.y, user->cam.z,
        user->lookAtPoint.x, user->lookAtPoint.y, user->lookAtPoint.z,
        user->upVector.x, user->upVector.y, user->upVector.z);

  glUniformMatrix4fv(glGetUniformLocation(gameoverhandler->skyboxshader, "LookAtMatrix"), 1, GL_TRUE, camMatrix.m);
  glUniformMatrix4fv(glGetUniformLocation(gameoverhandler->skyboxshader, "mdlMatrix"), 1, GL_TRUE, skyMatrix.m);
  glBindTexture(GL_TEXTURE_2D, gameoverhandler->skyTex);
  DrawModel(gameoverhandler->skybox, gameoverhandler->skyboxshader, "inPosition", NULL, "inTexCoord");


  glEnable(GL_DEPTH_TEST);
  glUseProgram(gameoverhandler->shader);
  glUniformMatrix4fv(glGetUniformLocation(gameoverhandler->shader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
  glEnableVertexAttribArray(glGetAttribLocation(gameoverhandler->shader, "projectionMatrix"));
  glUniformMatrix4fv(glGetUniformLocation(gameoverhandler->shader, "wtvMatrix"), 1, GL_TRUE, camMatrix.m);
  mat4 total;
  mat4 trans = T(user->cam.x + 200,user->cam.y,user->cam.z);
  mat4 rot1 = Ry(3*M_PI/2);
  total = Mult(trans, rot1);
  glUniformMatrix4fv(glGetUniformLocation(gameoverhandler->shader, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(gameoverhandler->gameovermodel, gameoverhandler->shader, "in_Position", "in_Normal", "inTexCoord");


}
