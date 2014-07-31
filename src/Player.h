#ifndef __TOAKRONAF_TETRIS_PLAYER_H_INCLUDED__
#define __TOAKRONAF_TETRIS_PLAYER_H_INCLUDED__

#include "Block.h"

struct Map;

struct Player{
	unsigned short x;
	unsigned short y;

	unsigned short fallTimeCounter;//TODO: Make an abstract controller, in this case both a HumanControl and a FallTimeControl
	short downKeyCounter;

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
bool Player_rotateBlockLeft(struct Player* player,const struct Map* map);

/**
 * Rotates the player's currently selected block. Does nothing if the nothing is selected.
 */
bool Player_rotateBlockRight(struct Player* player,const struct Map* map);

/**
 * Moves player horizontally (x-axis)
 */
bool Player_moveX(struct Player* player,const struct Map* map,int x);

/**
 * Moves player vertically (y-axis)
 */
bool Player_moveY(struct Player* player,const struct Map* map,int y);

#endif
