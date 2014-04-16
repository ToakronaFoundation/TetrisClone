#ifndef __TOAKRONAF_TETRIS_PLAYER_H_INCLUDED__
#define __TOAKRONAF_TETRIS_PLAYER_H_INCLUDED__

struct Block;

struct Player{
	unsigned short x;
	unsigned short y;

	unsigned short fallTimeCounter;

	struct Block* selectedBlock;
};

#endif
