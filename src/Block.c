#include "Block.h"

#include "Bits.h"

extern inline unsigned short Block_getWidth(const struct Block* block);
extern inline unsigned short Block_getHeight(const struct Block* block);
extern inline void Block_setSpacesFromBitlist(struct Block* block,byte* bitlist,size_t size);

struct Block* Block_alloc(unsigned short width,unsigned short height){
	struct Block* block = malloc(sizeof(struct Block) + Bits_sizeof(width*height));
	block->width  = width;
	block->height = height;
	return block;
}

bool Block_getSpace(const struct Block* block,unsigned short x,unsigned short y){
	if(x >= block->width || y >= block->height)
		return false;
	else
		return Bits_get(block->spaces,y*block->width + x);
}

struct Block* Block_rotateHalfTurn(const struct Block* block,struct Block* out){
	size_t i = block->width*block->height,
	       j = 0;
	while(i-->0){
		if(Bits_get(block->spaces,i))
			Bits_set1(out->spaces,j);
		else
			Bits_set0(out->spaces,j);
		++j;
	}

	return out;
}

struct Block* Block_copy(const struct Block* block,struct Block* out){
	memcpy(out,block,sizeof(struct Block) + Bits_sizeof(block->width*block->height));
	return out;
}
