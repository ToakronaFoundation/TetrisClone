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
    if(++(gameData->animationFallCounter) == GAME_FALL_SPEED * gameData->blockFalling){
      gameData->blockFalling         = 0;
      gameData->animationFallCounter = 0;
      Map_merge(gameData->map, gameData->fallingBlocks, 0, gameData->topLineRemoved);
      free(gameData->fallingBlocks);
    }
  }
}

void Game_render(struct GameData* gameData){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	Map_draw(gameData->map,0,0);
  if (gameData->blockFalling){
    int quotient = gameData->animationFallCounter / GAME_FALL_SPEED;
    Map_draw(gameData->fallingBlocks,0, quotient);
  }
  glColor3f(1.0f,1.0f,1.0f);
	Block_draw(gameData->players[0].selectedBlock.copy,gameData->players[0].x,gameData->players[0].y);
}

void Game_blockTouchesBottom(struct GameData* gameData, unsigned short playerIndex, struct Map* map, const struct BlockTypeData* blockTypes){
  struct Player player = gameData->players[playerIndex];
	Map_imprintBlock(map,player.selectedBlock.copy,player.x,player.y);

	//Reset position
	player.x = 3;
	player.y = 0;

	//Reset rotation
	player.selectedBlock.rotation = BLOCK_ROTATION_NONE;

	//Select new block randomly
	Player_selectBlock(&player,blockTypes->blocks[rand()%blockTypes->blockCount]);

	player.fallTimeCounter=0;
  
  Map_removeLines(map, &gameData->topLineRemoved, &gameData->blockFalling);
  if (gameData->blockFalling){
    gameData->animationFallCounter = 0;
    gameData->fallingBlocks        = Map_alloc(map->width, gameData->topLineRemoved);
    for(int i = 0; i < gameData->topLineRemoved; ++i){
      for(int j = 0; j < map->width; ++j){
        if (map->collisionMap[i][j]){
          map->collisionMap[i][j] = false;
          gameData->fallingBlocks->collisionMap[i][j] = true;
        }
      }
    }
  }
}
