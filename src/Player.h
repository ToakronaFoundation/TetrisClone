#ifndef __TOAKRONAF_TETRIS_PLAYER_H_INCLUDED__
#define __TOAKRONAF_TETRIS_PLAYER_H_INCLUDED__

#include "Block.h"

struct Player{
	unsigned short x;
	unsigned short y;

	unsigned short fallTimeCounter;

	struct{
		const struct Block* original;
		struct Block* copy;
		enum BlockRotation rotation;
	}selectedBlock;
};

void Player_selectBlock(struct Player* player,const struct Block* block);

void Player_rotateBlockLeft(struct Player* player);
void Player_rotateBlockRight(struct Player* player);

#endif
