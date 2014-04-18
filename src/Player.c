#include "Player.h"

#include <string.h>
#include <stdlib.h>
#include "Block.h"
#include "Bits.h"

void Player_selectBlock(struct Player* player,const struct Block* block){
	player->selectedBlock.original = block;

	if(block==NULL)
		player->selectedBlock.copy = NULL;
	else{
		size_t newSize = Bits_sizeof(block->width*block->height);
		if(player->selectedBlock.copy==NULL || newSize > Bits_sizeof(player->selectedBlock.copy->width*player->selectedBlock.copy->height)){
			player->selectedBlock.copy = realloc(player->selectedBlock.copy,newSize);
		}
		memcpy(player->selectedBlock.copy,player->selectedBlock.original,sizeof(struct Block)+newSize);
	}
};

void Player_rotateBlockLeft(struct Player* player){
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
