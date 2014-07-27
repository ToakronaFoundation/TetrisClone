#include "Map.h"
#include "Bits.h"
#include "Block.h"
#include "Types.h"
#include <stdbool.h>

extern inline bool Map_getSpace(const struct Map* map,unsigned int x,unsigned int y);
extern inline bool Map_setSpace(const struct Map* map,unsigned int x,unsigned int y,bool state);

struct Map* Map_alloc(unsigned int width, unsigned int height){
	struct Map* map = malloc(sizeof(struct Map) + sizeof(bool *) * height);
	if(!map)
		return NULL;
	map->width  = width;
	map->height = height;
	for(int i = 0; i < height; ++i){
		map->collisionMap[i] = calloc(width, sizeof(bool));
	}
	return map;
}

bool Map_intersectsWithBlock(struct Map* map, const struct Block* block, unsigned int x, unsigned int y){
	//bounds check
	if (x + block->width > map->width)
		return false;
	int i,j;
	for     (i = y; i < block->height + y; ++i){
		for (j = x; j < block->width  + x; ++j){
			if (map->height <= y ||  (Map_getSpace(map,j,i) && Block_getSpace(block, x + j, y + i)))
				return true;
		}
	}

	return false;
}

bool Map_imprintBlock(struct Map* map, const struct Block* block, unsigned int x, unsigned int y){
	//bounds check
	if(x+block->width > map->width || y-block->height > map->height)
		return false;
	for(unsigned int iy = 0; iy < block->height; ++iy){
		for(unsigned int ix = 0; ix < block->width ; ++ix){
			if(Block_getSpace(block,ix,iy)){
				Map_setSpace(map,x+ix,y-iy,true);
			}
		}
	}

	return true;
}

bool Map_removeLine(struct Map* map, unsigned int y){
	if(map->height < y){
		return false;
	}
	int i = 0;
	for(; i < map->width; ++i){
		if (Map_getSpace(map,i,y)){
			return false;
		}
	}
	while (i --> 0){
		Map_setSpace(map,i,y,false);
	}
	return true;
}

bool Map_removeLines(struct Map* map){
	unsigned int height = map->height;
	bool lineWasRemoved = false;
	while (height --> 0){
		lineWasRemoved = lineWasRemoved || Map_removeLine(map, height);
	}
	return lineWasRemoved;
}
