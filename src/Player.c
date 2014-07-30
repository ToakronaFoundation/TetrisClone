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

bool Player_rotateBlockLeft(struct Player* player,const struct Map* map){
	if(!player->selectedBlock.original)
		return false;

	const size_t size = Block_sizeof(player->selectedBlock.copy);
	byte copy[size];
	memcpy(&copy,player->selectedBlock.copy,size);
	const enum BlockRotation rotation = player->selectedBlock.rotation;

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

	//If out of bounds or intersects, revert changes
	if(player->x+player->selectedBlock.copy->width > map->width || player->y+player->selectedBlock.copy->height > map->height || Map_intersectsWithBlock(map,player->selectedBlock.copy,player->x,player->y)){
		memcpy(player->selectedBlock.copy,&copy,size);
		player->selectedBlock.rotation = rotation;
		return false;
	}

	return true;
}

bool Player_rotateBlockRight(struct Player* player,const struct Map* map){
	if(!player->selectedBlock.original)
		return false;

	const size_t size = Block_sizeof(player->selectedBlock.copy);
	byte copy[size];
	memcpy(&copy,player->selectedBlock.copy,size);
	const enum BlockRotation rotation = player->selectedBlock.rotation;

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

	//If out of bounds or intersects, revert changes
	if(player->x+player->selectedBlock.copy->width > map->width || player->y+player->selectedBlock.copy->height > map->height || Map_intersectsWithBlock(map,player->selectedBlock.copy,player->x,player->y)){
		memcpy(player->selectedBlock.copy,&copy,size);
		player->selectedBlock.rotation = rotation;
		return false;
	}

	return true;
}

bool Player_moveX(struct Player* player,const struct Map* map,int x){
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

bool Player_moveY(struct Player* player,const struct Map* map,int y){
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
