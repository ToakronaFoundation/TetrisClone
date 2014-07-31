#ifndef __TOAKRONAF_TETRIS_BLOCKTYPEDATA_H_INCLUDED__
#define __TOAKRONAF_TETRIS_BLOCKTYPEDATA_H_INCLUDED__

struct Block;

struct BlockTypeData{
	unsigned short blockCount;
	struct Block* blocks[];
};

struct BlockTypeData* BlockTypeData_alloc(unsigned short blockCount);

#endif
