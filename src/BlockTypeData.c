#include "BlockTypeData.h"

#include <stdlib.h>
#include "Block.h"

struct BlockTypeData* BlockTypeData_alloc(unsigned short blockCount){
	struct BlockTypeData* out;
	if(!(out = malloc(sizeof(struct BlockTypeData) + sizeof(struct Block*)*blockCount)))
		return NULL;
	out->blockCount = blockCount;
	return out;
}
