#ifndef __TOAKRONAF_TETRIS_MAP_H_INCLUDED__
#define __TOAKRONAF_TETRIS_MAP_H_INCLUDED__

#include <stdbool.h>
#include "Block.h"

#define MAP_DEFAULT_WIDTH  10
#define MAP_DEFAULT_HEIGHT 20

/**
 * A map is simply a twodimensional array of booleans
 * Indexed by [y][x]
 */
struct Map {
	unsigned short width;
	unsigned short height;

	bool* collisionMap[];
};

/**
 * Allocates space for the map
 * @param width  Width of map
 * @param height Height of map
 * @return		 pointer to the allocated structure
 */

struct Map* Map_alloc(unsigned short width, unsigned short height);

/**
 * Tests if a block with position (x,y) intersects with the map
 * @param map	The map to test
 * @param block	The block to test
 * @param x		The x position of the block
 * @param y		The y position of the block
 * @return		Whether it intersects or not
 */

bool Map_intersectsWithBlock(const struct Map* map, const struct Block* block, unsigned short x, unsigned short y);

/**
 * Superimposes a block on a map, making coordinates of the map solid where the block with position x and y would be solid.
 * @param map	The map to mutate
 * @param block	The block to superimpose
 * @param x		The x position of the block
 * @param y		The y position of the block
 * @return		Whether the imprint was succesful. The imprint can only fail if the block is out of bounds.
 */

bool Map_imprintBlock(struct Map* map, const struct Block* block, unsigned short x, unsigned short y);

/**
 * Removes specified line from the supplied map, if the whole line is solid
 * @param map	The map to remove a line from
 * @param y		The line number to remove
 * @return		Whether the line was removed. The line isn't removed unless it's fully solid and the the line number is in bound
 */

bool Map_removeLine(struct Map* map, unsigned short y);

/**
 * Removes all lines that are fully solid.
 * @param map	          The map to remove lines from
 * @param topLine       pointer to mutate with the top line number removed by the function
 * @param linesRemoved  pointer to mutate with the number of lines removed by the function
 * @return		          void
 */

void Map_removeLines(struct Map* map, short* topLine, unsigned short* linesRemoved);

/**
 * Merge two maps, mutating one of them and leaving the other as it is
 * @param map1  The map to mutate
 * @param map2  The other map
 * @param x     x offset
 * @param y     y offset
 */

void Map_merge(struct Map* map1, struct Map* map2, unsigned short x, unsigned short y);


inline bool Map__getSpace(const struct Map* map,unsigned short x,unsigned short y){
	return map->collisionMap[y][x];
}

inline bool Map_getSpace(const struct Map* map,unsigned short x,unsigned short y){
	if(x<0 || x>=map->width || y<0 || y>=map->height)
		return false;
	return Map__getSpace(map,x,y);
}

inline void Map__setSpace(const struct Map* map,unsigned short x,unsigned short y,bool state){
	map->collisionMap[y][x] = state;
}

inline bool Map_setSpace(const struct Map* map,unsigned short x,unsigned short y,bool state){
	if(x<0 || x>=map->width || y<0 || y>=map->height)
		return false;
	map->collisionMap[y][x] = state;
	return true;
}

#endif
