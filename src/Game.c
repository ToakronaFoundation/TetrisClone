#include "Game.h"

#include <GLFW/glfw3.h>
#include "Block.h"
#include "BlockTypeData.h"
#include "Map.h"
#include "draw.h"
#include <stdbool.h>

void Game_update(struct GameData* gameData,GLFWwindow* window){
	if(!gameData->blockFalling){
		//For every player
		for(typeof(gameData->playerCount) i=0; i<gameData->playerCount; ++i){
			//Increase fall time counter and check if gravity should pull down the player's block at the moment
			if(++gameData->players[i].fallTimeCounter > gameData->players[i].fallTime){
				//Move down if the player isn't manually holding the key and moving down the block
				if(!glfwGetKey(window,GLFW_KEY_DOWN) && !Player_moveY(&gameData->players[i],gameData->maps[0],1))
					//If moving down failed, that means we have a collision, resolve
					Game_blockTouchesBottom(gameData,i,gameData->maps[0],gameData->blockTypes);

				//Reset the fall time counter in any case
				gameData->players[i].fallTimeCounter=0;
			}
		}
	}
	else {
		if(++(gameData->animationFallCounter) == gameData->players[0].fallTime * gameData->blockFalling){
			gameData->blockFalling         = 0;
			gameData->animationFallCounter = 0;
			Map_merge(gameData->maps[0], gameData->fallingBlocks, 0, gameData->topLineRemoved);
			free(gameData->fallingBlocks);
		}
	}
}

void Game_render(struct GameData* gameData){
	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Draw map
	Map_draw(gameData->maps[0],0,0);


	if(gameData->blockFalling){
		int quotient = gameData->animationFallCounter / gameData->players[0].fallTime;
		Map_draw(gameData->fallingBlocks,0, quotient);
	}

	//Draw players' block
	glColor3f(1.0f,1.0f,1.0f);
	for(typeof(gameData->playerCount) i=0; i<gameData->playerCount; ++i)
		Block_draw(gameData->players[i].selectedBlock.copy,gameData->players[i].x,gameData->players[i].y);
}

void Game_blockTouchesBottom(struct GameData* gameData, unsigned short playerIndex, struct Map* map, const struct BlockTypeData* blockTypes){
	struct Player*const player = &gameData->players[playerIndex];

	//Imprint (copy paste) block (data) to map with its position
	Map_imprintBlock(map,player->selectedBlock.copy,player->x,player->y);

	//Reset position
	player->x = 3;
	player->y = 0;

	//Reset rotation
	player->selectedBlock.rotation = BLOCK_ROTATION_NONE;

	//Select new block randomly
	Player_selectBlock(player,blockTypes->blocks[rand()%blockTypes->blockCount]);

	//Reset fall time counter
	player->fallTimeCounter=0;
	
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
