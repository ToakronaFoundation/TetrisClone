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
