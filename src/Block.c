#include "Block.h"

#include "Bits.h"

extern inline bool Block__getSpace(const struct Block* block,unsigned short x,unsigned short y);
extern inline unsigned short Block_getWidth(const struct Block* block);
extern inline unsigned short Block_getHeight(const struct Block* block);
extern inline void Block_setSpacesFromBitlist(struct Block* block,byte* bitlist,size_t size);

struct Block* Block_alloc(unsigned short width,unsigned short height){
	struct Block* block = malloc(sizeof(struct Block) + Bits_sizeof(width*height));
	if(!block)
		return NULL;
	block->width  = width;
	block->height = height;
	return block;
}

bool Block_getSpace(const struct Block* block,unsigned short x,unsigned short y){
	//If outside of bounds
	if(x >= block->width || y >= block->height)
		//No space outside is solid, return false
		return false;
	else
		//Check if the given coordinates of space is solid
		return Block__getSpace(block,x,y);
}

struct Block* Block_rotateHalfTurn(const struct Block* block,struct Block* out){
	out->width  = block->width;
	out->height = block->height;

	//Reverse bits
	size_t i = out->width*out->height,
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

struct Block* Block_rotateLeft(const struct Block* block,struct Block* out){
	out->width  = block->height;
	out->height = block->width;

	size_t i = block->width*block->height,
	       x = 0,
	       y = block->height;
	while(i-->0){
		if(y==0){
			y = block->height;
			++x;
		}
		--y;

		if(Bits_get(block->spaces,y*block->width + x))
			Bits_set1(out->spaces,i);
		else
			Bits_set0(out->spaces,i);
	}

	return out;
}

struct Block* Block_rotateRight(const struct Block* block,struct Block* out){
	out->width  = block->height;
	out->height = block->width;

	size_t i = block->width*block->height,
	       x = block->width-1,
	       y = 0;
	while(i-->0){
		if(Bits_get(block->spaces,y*block->width + x))
			Bits_set1(out->spaces,i);
		else
			Bits_set0(out->spaces,i);

		if(++y==block->height){
			y = 0;
			--x;
		}
	}

	return out;
}

struct Block* Block_copy(const struct Block* block,struct Block* out){
	memcpy(out,block,sizeof(struct Block) + Bits_sizeof(block->width*block->height));
	return out;
}
