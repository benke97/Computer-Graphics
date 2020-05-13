
#include "Enemyhandler.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"
#include <math.h>


void initEnemyHandler(EnemyHandler* enemyhandler) {
  enemyhandler->EnemiesQuantity = 0;
  enemyhandler->timeUntilNextEnemy = 0;
};


EnemyHandler* createEnemyHandler() {
  EnemyHandler* result = (EnemyHandler*) malloc(sizeof(EnemyHandler));
  initEnemyHandler(result);
  return result;
};


void CheckForNewEnemies (EnemyHandler* enemyhandler, User * user, Terrain * floor, mat4 projectionMatrix, GLuint enemyShader){
  bool newEnemy = user->enemyShootingActivated || enemyhandler->timeUntilNextEnemy < 0;
  bool enemyLimitReached = enemyhandler->EnemiesQuantity > 99;

  if (newEnemy && !enemyLimitReached) {
    Enemy * enemy;
    enemy = createEnemy(projectionMatrix, enemyShader);

    bool goodPositionFound = false;
    while (!goodPositionFound) {
      float x = rand() % 35 + 15;
      float z = rand() % 110 + 90;
      float y = heightFinder(x, z, floor);
      if(y < 10) {
        goodPositionFound = true;
        enemy->position = SetVector(x, y, z);
      }
    }

    enemyhandler->enemiesPositions[enemyhandler->EnemiesQuantity] = enemy->position;
    enemyhandler->enemies[enemyhandler->EnemiesQuantity] = *enemy;
    enemyhandler->EnemiesQuantity += 1;
    enemyhandler->timeUntilNextEnemy = 100;
  }
	else {
		enemyhandler->timeUntilNextEnemy--;
	}
	user->enemyShootingActivated = false;
}

void diaplayEnemies (EnemyHandler* enemyhandler, mat4 *camMatrix, User * user) {
  for (int enemy_ind=0; enemy_ind < enemyhandler->EnemiesQuantity; enemy_ind++){
    Enemy * enemy = &enemyhandler->enemies[enemy_ind];

    mat4 trans, rot1;
    float dx = enemy->position.x - user->cam.x;
    float dz = enemy->position.z - user->cam.z;

    trans = T(enemy->position.x, enemy->position.y, enemy->position.z);
    rot1 = Ry( - atan2(dz , dx) - M_PI/2);

    displayEnemy(enemy, camMatrix, trans, rot1);
  }
}

void MoveAllEnemies(EnemyHandler* enemyhandler, User * user, Terrain * floor){
  for (int e=0; e < enemyhandler->EnemiesQuantity; e++){
    Enemy * enemy = &enemyhandler->enemies[e];
    MoveEnemy(enemy, user->cam, floor);
    enemyhandler->enemiesPositions[e] = enemy->position;
  }
}

void RemoveEnemies(EnemyHandler* enemyhandler){

  for (int enemy_ind=0; enemy_ind < enemyhandler->EnemiesQuantity; enemy_ind++){
  Enemy * enemy = &enemyhandler->enemies[enemy_ind];
    if (enemy->lifeTimer < 0) {
      enemy->active = false;
    }
  }

  Enemy new_enemies[100];
	vec3 new_EnemiesPositions[100];
	int new_enemy_ind = 0;

  for (int enemy_ind=0; enemy_ind < enemyhandler->EnemiesQuantity; enemy_ind++){
    if (enemyhandler->enemies[enemy_ind].active) {
        new_enemies[new_enemy_ind] = enemyhandler->enemies[enemy_ind];
        new_EnemiesPositions[new_enemy_ind] = enemyhandler->enemiesPositions[enemy_ind];
				new_enemy_ind++;
    }
  }

	enemyhandler->EnemiesQuantity = new_enemy_ind;

	for (int enemy_ind=0; enemy_ind < enemyhandler->EnemiesQuantity; enemy_ind++) {
		enemyhandler->enemies[enemy_ind] = new_enemies[enemy_ind];
		enemyhandler->enemiesPositions[enemy_ind] = new_EnemiesPositions[enemy_ind];
	}
}
