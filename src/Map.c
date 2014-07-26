#include "Map.h"
#include "Bits.h"
#include "Block.h"
#include "Types.h"
#include <stdbool.h>

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
			if (map->height <= y ||  (map->collisionMap[i][j] && Block_getSpace(block, x + j, y + i)))
				return true;
		}
	}

	return false;
}

bool Map_imprintBlock(struct Map* map, const struct Block* block, unsigned int x, unsigned int y){
	//bounds check
	if (x + block->width > map->width || y + block->height > map->height)
		return false;
	int i,j;
	for     (i = y; i < block->height + y; ++i){
		for (j = x; j < block->width  + x; ++j){
			if (Block_getSpace(block, x + j, y + i))
				map->collisionMap[i][j] = true;
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
		if (map->collisionMap[y][i]){
			return false;
		}
	}
	while (i --> 0){
		map->collisionMap[y][i] = false;
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
