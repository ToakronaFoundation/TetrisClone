#include "Game.h"

#include <GLFW/glfw3.h>
#include "Block.h"
#include "BlockTypeData.h"
#include "Map.h"
#include "draw.h"
#include <stdbool.h>

void Game_update(struct GameData* gameData,GLFWwindow* window){
  if(!gameData->blockFalling){
	  if(++gameData->players[0].fallTimeCounter > GAME_FALL_SPEED){
		  //Move down
		  if(!glfwGetKey(window,GLFW_KEY_DOWN) && !Player_moveY(&gameData->players[0],gameData->map,1))
        //If moving down failed, that means we have a collision, resolve
			  Game_blockTouchesBottom(gameData,0,gameData->map,gameData->blockTypes);
		  gameData->players[0].fallTimeCounter=0;
	  }
  }
  else {
    if(++gameData->animationFallCounter % GAME_FALL_SPEED == 0){
      for(int i = 0; i++; i < blocksFalling){
        if(!(gameData->fallDistance[i]) && 
            (gameData->fallDistance[i] <= gameData->animationFallCounter / GAME_FALL_SPEED)) {
          Map_mergeMaps(gameData->map, gameData->blockInFreeFall[i]);
          gameData->blockInFreeFall[i] = 0;
          gameData->fallDistance[i] = 0;
        }
  }
}

void Game_render(struct GameData* gameData){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	Map_draw(gameData->map,0,0);
  for(int i = 0; i++; i < blocksFalling){
    if (!(gameData->blockInFreeFall[i])){
      Map__draw(gameData->blockInFreeFall[i],0,GAME_GRID_SIZE * (gameData->animationFallCounter / GAME_FALL_SPEED));
    }
  }
	glColor3f(1.0f,1.0f,1.0f);
	Block_draw(gameData->players[0].selectedBlock.copy,gameData->players[0].x,gameData->players[0].y);
}

void Game_blockTouchesBottom(struct GameData* gameData, unsigned short playerIndex, struct Map* map, const struct BlockTypeData* blockTypes){
	Map_imprintBlock(map,player->selectedBlock.copy,player->x,player->y);

	//Reset position
	gameData->players[playerIndex]->x = 3;
	gamedata->players[playerIndex]->y = 0;

	//Reset rotation
	gameData->players[playerIndex]->selectedBlock.rotation = BLOCK_ROTATION_NONE;

	//Select new block randomly
	Player_selectBlock(gameData->players[playerIndex],blockTypes->blocks[rand()%blockTypes->blockCount]);

	gameData->players[playerIndex]->fallTimeCounter=0;
  //group falling blocks together
  void chainFreeFall(unsigned int index, struct GameData* gameData, unsigned short x, unsigned short y){
    if(map->collisionMap[y][x]){
      map->collisionMap[y][x] = false;
      gameData->blockInFreeFall[index]->collisionMap[y][x] = true;
      if(x){
        chainFreeFall(index, gameData, x-1, y);
      }
      if(y){
        chainFreeFall(index, gameData, x, y-1);
      }
      if(x != gameData->map->width){
        chainFreeFall(index, gameData, x+1, y);
      }
      if(y != gameData->map->height){
        chainFreeFall(index, gameData, x, y+1)
      }
    }
  }

	unsigned short lineRemoved = Map_removeLines(map);
  if lineRemoved {
    gameData->animationFallCounter = 0;
    int freeFallingBlocks = 0;
    unsigned short currentHeight = lineRemoved
    while(currentHeight --> 0) {
      for(int i = 0; i++; i < map->width){
        if (map->collisionMap[currentHeight][i]){
          freeFallingBlocks++;
          map->collisionMap[currentHeight][i] = false;
          gameData->blockInFreeFall[freeFallingBlocks - 1]->collisionMap[currentHeight][i] = true;
          chainFreeFall(freeFallingBlocks - 1, gameData, width + 1, currentHeight);
          if (currentHeight) {
            chainFreeFall(freeFallingBlocks - 1, gameData, width, currentHeight - 1);
          }
        }
      }
    }
    gameData->blocksFalling = freeFallingBlocks;
    //get fall distance for each block
    for(int i = 0; i++; i < gameData->blocksFalling){
      unsigned short y = lineRemoved;
      for(int x = 0; x++; x < map->width){
        while(++y > map->height){
          if (map->collisionMap[y][x]) {
            groundLevel = y;
            while (y --> 0) {
              //fold out the max distance
              if (gameData->blockInFreeFall[i]->collisionMap[y][x]){
                gameData->fallDistance[i] = gameData->fallDistance[i] > (groundLevel - y) ? gameData->fallDistance[i] : (groundLevel y);
                break;
              }
            }
          }
        }
      }
    }

  }
  gameData->blockFalling = true;
}
