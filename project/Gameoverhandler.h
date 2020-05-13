#ifndef _GAMEOVERHANDLER_H_
#define _GAMEOVERHANDLER_H_

#include "VectorUtils3.h"
#include "User.h"
#include "FlashLight.h"
#include "loadobj.h"
#include "LoadTGA.h"



typedef struct GameOverHandler {
  GLuint skyboxshader;
  GLuint shader;
  Model *skybox;
  Model *gameovermodel;
  GLuint skyTex;

} GameOverHandler;


void initGameOverHandler(GameOverHandler*, mat4 projectionMatrix);
GameOverHandler* createGameOverHandler(mat4 projectionMatrix);
void HandleGameOver(GameOverHandler* gameoverhandler, User* user, mat4 projectionMatrix);

#endif
