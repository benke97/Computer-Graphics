
#include "Enemyhandler.h"

#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "GL_utilities.h"


void initEnemyHandler(EnemyHandler* enemyhandler) {
  enemyhandler->EnemiesQuantity = 0;
  enemyhandler->timeUntilNextEnemy = 0;
  enemyhandler->maxDistance = 200;
};


EnemyHandler* createEnemyHandler() {
  EnemyHandler* result = (EnemyHandler*) malloc(sizeof(EnemyHandler));
  initEnemyHandler(result);
  return result;
};


void CheckForNewEnemies (EnemyHandler* enemyhandler, User * user, Terrain * floor, mat4 projectionMatrix){
  if (user->enemyShootingActivated && enemyhandler->timeUntilNextEnemy < 0) {
    Enemy * enemy;

    enemy = createEnemy(projectionMatrix);
    enemyhandler->enemiesColor[enemyhandler->EnemiesQuantity] = SetVector(1.0f, 1.0f, 0.0f);

    bool goodPositionFound = false;
    while (!goodPositionFound) {
      float x = rand() % 200 + 25;
      float z = rand() % 200 + 25;
      float y = heightFinder(x, z, floor);
      if(y < 10) {
        goodPositionFound = true;
        enemy->position = SetVector(x, y, z);
        printf("user: x: %f , y: %f , z: %f", user->cam.x, user->cam.y, user->cam.z);
        printf("enemy: x: %f , y: %f , z: %f", enemy->position.x, enemy->position.y, enemy->position.z);
      }
    }





    enemyhandler->enemiesPositions[enemyhandler->EnemiesQuantity] = enemy->position;
    enemyhandler->enemiesIntensities[enemyhandler->EnemiesQuantity] = enemy->intensity;
    enemy->direction = Normalize(VectorSub(user->lookAtPoint, user->cam));
    enemyhandler->enemies[enemyhandler->EnemiesQuantity] = *enemy;
    enemyhandler->EnemiesQuantity += 1;
    enemyhandler->timeUntilNextEnemy = 10;
  }
	else {
		enemyhandler->timeUntilNextEnemy--;
		user->enemyShootingActivated = false;
	}
}

void diaplayEnemies (EnemyHandler* enemyhandler, mat4 *camMatrix) {
  for (int enemy_ind=0; enemy_ind < enemyhandler->EnemiesQuantity; enemy_ind++){
    Enemy * enemy = &enemyhandler->enemies[enemy_ind];

    mat4 trans, rot1;
    trans = T(enemy->position.x, enemy->position.y, enemy->position.z);
    rot1 = Rx(0);
    displayEnemy(enemy, camMatrix, trans, rot1);
  }
}


void CheckEnemiesCollisions (EnemyHandler* enemyhandler, Terrain * floor, Terrain * roof) {
  for (int enemy_ind=0; enemy_ind < enemyhandler->EnemiesQuantity; enemy_ind++){
    Enemy * enemy = &enemyhandler->enemies[enemy_ind];

    float x = enemy->position.x;
    float z = enemy->position.z;

    float floorheight = heightFinder(x, z, floor);
    float roofheight = heightFinder(x, z, roof);
    //vec3 floorNormal = getNormal(x, z, floor);
    //vec3 roofNormal = getNormal(x, z, roof);

//    if(enemy->position.y < floorheight || enemy->position.y > roofheight - 1){
    if(enemy->position.y < floorheight || enemy->position.y > roofheight - 5){

      //enemy->flying = false;
      //enemy->intensity -= 0.05;
      /*
      if(enemy->position.y < floorheight ) {
        enemyhandler->enemiesPositions[enemy_ind] = VectorSub(enemy->position, ScalarMult(floorNormal, -1));
      }
      else {
        enemyhandler->enemiesPositions[enemy_ind] = VectorSub(enemy->position, ScalarMult(roofNormal, -3));
      }
      */

    }

  }
}

void MoveAllEnemies(EnemyHandler* enemyhandler){
  for (int ball=0; ball < enemyhandler->EnemiesQuantity; ball++){
    Enemy * enemy = &enemyhandler->enemies[ball];

    if (enemy->flying) {
      MoveEnemy(enemy);
      enemyhandler->enemiesPositions[ball] = enemy->position;
    }

    enemyhandler->enemiesIntensities[ball] = enemy->intensity*30;
  }
}

void RemoveEnemies(EnemyHandler* enemyhandler){

  for (int ball=0; ball < enemyhandler->EnemiesQuantity; ball++){
  Enemy * enemy = &enemyhandler->enemies[ball];
    //if (Norm(enemy->position) > enemyhandler->maxDistance) {
    /*if (enemy->intensity < 0.01) {
      enemy->active = false;
    }*/
  }

  Enemy new_enemies[100];
	vec3 new_EnemiesPositions[100];
  float new_EnemiesIntensities[100];
	vec3 new_EnemiesColor[100];
	int new_ball = 0;

  for (int ball=0; ball < enemyhandler->EnemiesQuantity; ball++){
    if (enemyhandler->enemies[ball].active) {
        new_enemies[new_ball] = enemyhandler->enemies[ball];
        new_EnemiesPositions[new_ball] = enemyhandler->enemiesPositions[ball];
        new_EnemiesIntensities[new_ball] = enemyhandler->enemiesIntensities[ball];
				new_EnemiesColor[new_ball] = enemyhandler->enemiesColor[ball];
				new_ball++;
    }
  }

	enemyhandler->EnemiesQuantity = new_ball;

	for (int ball=0; ball < enemyhandler->EnemiesQuantity; ball++) {
		enemyhandler->enemies[ball] = new_enemies[ball];
		enemyhandler->enemiesPositions[ball] = new_EnemiesPositions[ball];
    enemyhandler->enemiesIntensities[ball] = new_EnemiesIntensities[ball];
    enemyhandler->enemiesColor[ball] = new_EnemiesColor[ball];
	}
}

void displayEnemiesLight (EnemyHandler* enemyhandler, Terrain * terrain) {
  glUseProgram(terrain->shader);
  //printf("x-pos: %f", enemyhandler->enemiesPositions[0].x);
  //printf("x-int: %f \n", enemyhandler->enemiesIntensities[0]);
  //printf("x-col: %f", enemyhandler->enemiesColor[0].x);
  glUniform1i(glGetUniformLocation(terrain->shader, "EnemiesQuantity"), enemyhandler->EnemiesQuantity);
	glUniform3fv(glGetUniformLocation(terrain->shader, "EnemiesPositions"), enemyhandler->EnemiesQuantity, &enemyhandler->enemiesPositions[0].x);
	glUniform1fv(glGetUniformLocation(terrain->shader, "EnemiesIntensities"), enemyhandler->EnemiesQuantity, &enemyhandler->enemiesIntensities[0]);
	glUniform3fv(glGetUniformLocation(terrain->shader, "EnemiesColor"), enemyhandler->EnemiesQuantity, &enemyhandler->enemiesColor[0].x);
}
