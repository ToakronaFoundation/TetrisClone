#include "Game.h"

#include <GLFW/glfw3.h>
#include "Block.h"
#include "BlockTypeData.h"
#include "Map.h"
#include "draw.h"

void Game_update(struct GameData* gameData,GLFWwindow* window){
	if(++gameData->players[0].fallTimeCounter > GAME_FALL_SPEED){
		//If not able to move down
		if(!glfwGetKey(window,GLFW_KEY_DOWN) && !Player_moveY(&gameData->players[0],gameData->map,1))
			Game_blockTouchesBottom(&gameData->players[0],gameData->map,gameData->blockTypes);
		gameData->players[0].fallTimeCounter=0;
	}
}

void Game_render(struct GameData* gameData){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	Map_draw(gameData->map,0,0);
	glColor3f(1.0f,1.0f,1.0f);
	Block_draw(gameData->players[0].selectedBlock.copy,gameData->players[0].x,gameData->players[0].y);
}

void Game_blockTouchesBottom(struct Player* player,struct Map* map,const struct BlockTypeData* blockTypes){
	Map_imprintBlock(map,player->selectedBlock.copy,player->x,player->y);

	//Reset position
	player->x = 3;
	player->y = 0;

	//Reset rotation
	player->selectedBlock.rotation = BLOCK_ROTATION_NONE;

	//Select new block randomly
	Player_selectBlock(player,blockTypes->blocks[rand()%blockTypes->blockCount]);

	player->fallTimeCounter=0;

	Map_removeLines(map);//TODO: Not working at the moment and may be more efficient to just check for the rows that the current block affected
}
