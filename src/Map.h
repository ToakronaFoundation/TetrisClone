#ifndef __TOAKRONAF_TETRIS_MAP_H_INCLUDED__
#define __TOAKRONAF_TETRIS_MAP_H_INCLUDED__

#include <stdbool.h>
#include "Block.h"

/**
 * A map is simply a twodimensional array of booleans
 * Indexed by [y][x]
 */
struct Map {
	unsigned int width;
	unsigned int height;

	bool* collisionMap[];
};

/**
 * Allocates space for the map
 * @param width  Width of map
 * @param height Height of map
 * @return		 pointer to the allocated structure
 */

struct Map* Map_alloc(unsigned int width, unsigned int height);

/**
 * Tests if a block with position (x,y) intersects with the map
 * @param map	The map to test
 * @param block	The block to test
 * @param x		The x position of the block
 * @param y		The y position of the block
 * @return		Whether it intersects or not
 */

bool Map_intersectsWithBlock(const struct Map* map, const struct Block* block, unsigned int x, unsigned int y);

/**
 * Superimmposes a block on a map, making coordinates of the map solid where the block with position x and y would be solid.
 * @param map	The map to mutate
 * @param block	The block to superimpose
 * @param x		The x position of the block
 * @param y		The y position of the block
 * @return		Whether the imprint was succesful. The imprint can only fail if the block is out of bounds.
 */

bool Map_imprintBlock(struct Map* map, const struct Block* block, unsigned int x, unsigned int y);

/**
 * Removes specified line from the supplied map, if the whole line is solid
 * @param map	The map to remove a line from
 * @param y		The line number to remove
 * @return		Whether the line was removed. The line isn't removed unless it's fully solid and the the line number is in bound
 */

bool Map_removeLine(struct Map* map, unsigned int y);

/**
 * Removes all lines that are fully solid.
 * @param map	The map to remove lines from
 * @return		Whether any lines were removed
 */

bool Map_removeLines(struct Map* map);

inline bool Map__getSpace(const struct Map* map,unsigned int x,unsigned int y){
	return map->collisionMap[y][x];
}

inline bool Map_getSpace(const struct Map* map,unsigned int x,unsigned int y){
	if(x<0 || x>=map->width || y<0 || y>=map->height)
		return false;
	return Map__getSpace(map,x,y);
}

inline void Map__setSpace(const struct Map* map,unsigned int x,unsigned int y,bool state){
	map->collisionMap[y][x] = state;
}

inline bool Map_setSpace(const struct Map* map,unsigned int x,unsigned int y,bool state){
	if(x<0 || x>=map->width || y<0 || y>=map->height)
		return false;
	map->collisionMap[y][x] = state;
	return true;
}

#endif
