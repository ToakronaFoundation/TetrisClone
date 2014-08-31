#include "Map.h"
#include "Bits.h"
#include "Block.h"
#include "Types.h"
#include <stdbool.h>

extern inline bool Map__getSpace(const struct Map* map,unsigned short x,unsigned short y);
extern inline void Map__setSpace(const struct Map* map,unsigned short x,unsigned short y,bool state);
extern inline bool Map_getSpace(const struct Map* map,unsigned short x,unsigned short y);
extern inline bool Map_setSpace(const struct Map* map,unsigned short x,unsigned short y,bool state);

struct Map* Map_alloc(unsigned short width, unsigned short height){
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

bool Map_intersectsWithBlock(const struct Map* map, const struct Block* block, unsigned short x, unsigned short y){
	//bounds check
	if (x+block->width > map->width || y+block->height > map->height)
		return false;
	for     (unsigned int iy = 0; iy < block->height; ++iy){
		for (unsigned int ix = 0; ix < block->width ; ++ix){
			if(Block__getSpace(block,ix,iy) && Map__getSpace(map,x+ix,y+iy))
				return true;
		}
	}

	return false;
}

bool Map_imprintBlock(struct Map* map, const struct Block* block, unsigned short x, unsigned short y){
	//bounds check
	if(x+block->width > map->width || y+block->height > map->height)
		return false;
	for    (unsigned int iy = 0; iy < block->height; ++iy){
		for(unsigned int ix = 0; ix < block->width ; ++ix){
			if(Block__getSpace(block,ix,iy)){
				Map__setSpace(map,x+ix,y+iy,true);
			}
		}
	}

	return true;
}

bool Map_removeLine(struct Map* map, unsigned short y){
	if(map->height < y){
		return false;
	}
	int i;
	for(i=0; i < map->width; ++i){
		if (!Map__getSpace(map,i,y)){
			return false;
		}
	}
	while (i --> 0){
		Map__setSpace(map,i,y,false);
	}
	return true;
}

void Map_removeLines(struct Map* map, short* topLine, unsigned short* linesRemoved){
  *topLine = -1;
  unsigned short height = map->height;
	while(height --> 0){
    //If we remove a higher line number, replace the result with that linenumber
    if (Map_removeLine(map, height)){
      *topLine = height;
      *linesRemoved++;
    }
  }
}

void Map_merge(struct Map* map1, struct Map* map2, unsigned short x, unsigned short y){
  for(  int yy=0; yy < map2->height; ++yy){
    for(int xx=0; xx < map2->width;  ++xx){
      if(map2->collisionMap[yy][xx]){
        map1->collisionMap[y+yy][x+xx] = true;
      }
    }
  }
}
