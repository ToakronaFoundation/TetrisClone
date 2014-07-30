#include "Player.h"

#include <string.h>
#include <stdlib.h>
#include "Block.h"
#include "Bits.h"
#include "Map.h"

void Player_selectBlock(struct Player* player,const struct Block* block){
	player->selectedBlock.original = block;

	if(block==NULL){
		player->selectedBlock.copy     = NULL;
		player->selectedBlock.rotation = BLOCK_ROTATION_NONE;
	}else{
		size_t newSize = Bits_sizeof(block->width*block->height);
		if(player->selectedBlock.copy==NULL || newSize > Bits_sizeof(player->selectedBlock.copy->width*player->selectedBlock.copy->height)){
			player->selectedBlock.copy = realloc(player->selectedBlock.copy,newSize);
		}
		memcpy(player->selectedBlock.copy,player->selectedBlock.original,sizeof(struct Block)+newSize);
	}
};

void Player_rotateBlockLeft(struct Player* player){
	if(!player->selectedBlock.original)
		return;

	switch(player->selectedBlock.rotation){
		case BLOCK_ROTATION_NONE:
			Block_rotateLeft(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_LEFT;
			break;
		case BLOCK_ROTATION_LEFT:
			Block_rotateHalfTurn(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_HALFTURN;
			break;
		case BLOCK_ROTATION_HALFTURN:
			Block_rotateRight(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_RIGHT;
			break;
		case BLOCK_ROTATION_RIGHT:
			Block_copy(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_NONE;
			break;
	}
}

void Player_rotateBlockRight(struct Player* player){
	if(!player->selectedBlock.original)
		return;
	
	switch(player->selectedBlock.rotation){
		case BLOCK_ROTATION_NONE:
			Block_rotateRight(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_RIGHT;
			break;
		case BLOCK_ROTATION_LEFT:
			Block_copy(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_NONE;
			break;
		case BLOCK_ROTATION_HALFTURN:
			Block_rotateLeft(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_LEFT;
			break;
		case BLOCK_ROTATION_RIGHT:
			Block_rotateHalfTurn(player->selectedBlock.original,player->selectedBlock.copy);
			player->selectedBlock.rotation = BLOCK_ROTATION_HALFTURN;
			break;
	}
}

bool Player_moveX(struct Player* player,struct Map* map,int x){
	//New x
	x+=player->x;

	//If out of bounds
	if(x<0 || x+player->selectedBlock.copy->width > map->width)
		return false;

	//If intersects
	if(Map_intersectsWithBlock(map,player->selectedBlock.copy,x,player->y))
		return false;
	
	//Move
	player->x = x;
	return true;
}

bool Player_moveY(struct Player* player,struct Map* map,int y){
	//New y
	y+=player->y;

	//If out of bounds
	if(y<0 || y+player->selectedBlock.copy->height > map->height)
		return false;

	//If intersects
	if(Map_intersectsWithBlock(map,player->selectedBlock.copy,player->x,y))
		return false;

	//Move
	player->y = y;
	return true;
}
