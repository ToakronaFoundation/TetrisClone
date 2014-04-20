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

/**
 * Selects the player's currently holding block. This function is also used for delecting a block.
 *
 * @param block The block to select or replace the selection with. NULL if deselecting.
 */
void Player_selectBlock(struct Player* player,const struct Block* block);

/**
 * Rotates the player's currently selected block. Does nothing if the nothing is selected.
 */
void Player_rotateBlockLeft(struct Player* player);

/**
 * Rotates the player's currently selected block. Does nothing if the nothing is selected.
 */
void Player_rotateBlockRight(struct Player* player);

#endif
