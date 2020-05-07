
#include "Collisionhandler.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initCollisionHandler(CollisionHandler* collisionhandler) {
  printf("%i", 10);
};


CollisionHandler* createCollisionHandler() {
  CollisionHandler* result = (CollisionHandler*) malloc(sizeof(CollisionHandler));
  initCollisionHandler(result);
  return result;
};

void checkCollisionHandler(LightBallHandler* lightballhandler, EnemyHandler* enemyhandler, LaserHandler* laserhandler, User* user) {
  for (int enemy=0; enemy < enemyhandler->EnemiesQuantity; enemy++){
    bool gotHitByLightBall = false;
    for (int shot=0; shot < laserhandler->LaserQuantity; shot++) {
      if (Norm(VectorSub(VectorAdd(enemyhandler->enemiesPositions[enemy], SetVector(0,1.5,0)), laserhandler->laserPositions[shot])) < 3){
        laserhandler->lasers[shot].active=false;
        enemyhandler->enemies[enemy].lifeTimer -= 5;
      }
    }

    for (int ball=0; ball < lightballhandler->LightBallsQuantity; ball++) {
      if (Norm(VectorSub(VectorAdd(enemyhandler->enemiesPositions[enemy], SetVector(0,1.5,0)), lightballhandler->lightBallsPositions[ball])) < 1.5){
        LightBall * lightball = &lightballhandler->lightballs[ball];
        lightball->flying = false;
        if (lightball->lifeTimer > 1) {
          lightball->intensity = 2*lightball->lifeTimer;
        }
        else {
          lightball->intensity = 1;
        }


        lightball->position = VectorAdd(enemyhandler->enemies[enemy].position, SetVector(0,1.5,0));
        if (enemyhandler->enemies[enemy].lifeTimer <= 0 || !enemyhandler->enemies[enemy].active) {
          lightball->active = false;
        }
      gotHitByLightBall = true;
      }
    }


    if (Norm(VectorSub(VectorAdd(enemyhandler->enemiesPositions[enemy], SetVector(0,1.5,0)), user->cam)) < 1.5){
      user->gameover = true;
    }

    if (gotHitByLightBall) {
      enemyhandler->enemies[enemy].lifeTimer -= 1;
    }


  }
}
