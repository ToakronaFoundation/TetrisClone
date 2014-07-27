#ifndef __TOAKRONAF_TETRIS_BLOCK_H_INCLUDED__
#define __TOAKRONAF_TETRIS_BLOCK_H_INCLUDED__

#include <stdbool.h>
#include <string.h>
#include "Types.h"
#include "Bits.h"

enum BlockRotation{
	BLOCK_ROTATION_NONE,
	BLOCK_ROTATION_LEFT,
	BLOCK_ROTATION_HALFTURN,
	BLOCK_ROTATION_RIGHT
};

/**
 * Represents a block
 */
struct Block{
	unsigned short width;
	unsigned short height;
	
	byte spaces[];
};

//TODO: Should the transformation functions also return a offset the block should move after being transformed?

/**
 * Allocates a block with the given size
 *
 * @param width  Width of the block
 * @param height Height of the block
 * @return       The allocated block structure
 */
struct Block* Block_alloc(unsigned short width,unsigned short height);

struct Block* Block_copy(const struct Block* block,struct Block* out);

/**
 * Rotates the block to the left (anti-clockwise)
 *
 * Requirements:
 *   block = rotateLeft(rotateLeft(rotateLeft(rotateLeft(block))))
 *   block = rotateLeft(rotateRight(block))
 *
 * @param block The block to be rotated
 * @param out   Output for the rotated block. Note that `block` and `out` should not overlap in memory and also should not point to the same data
 * @return      Returns `out`
 */
struct Block* Block_rotateLeft(const struct Block* block,struct Block* out);

/**
 * Rotates the block to the right (clockwise)
 *
 * Requirements:
 *   block = rotateRight(rotateRight(rotateRight(rotateRight(block))))
 *   block = rotateRight(rotateLeft(block))
 *
 * @param block The block to be rotated
 * @param out   Output for the rotated block. Note that `block` and `out` should not overlap in memory and also should not point to the same data
 * @return      Returns `out`
 */
struct Block* Block_rotateRight(const struct Block* block,struct Block* out);

/**
 * Rotates the block to the right (clockwise)
 *
 * Requirements:
 *   block                 = rotateHalfTurn(rotateHalfTurn(block))
 *   rotateHalfTurn(block) = rotateRight(rotateRight(block))
 *   rotateHalfTurn(block) = rotateLeft(rotateLeft(block))
 *
 * @param block The block to be rotated
 * @param out   Output for the rotated block. Note that `block` and `out` should not overlap in memory and also should not point to the same data.
 * @return      Returns `out`
 */
struct Block* Block_rotateHalfTurn(const struct Block* block,struct Block* out);

/**
 * Inverts the x-axis of the blocks spaces
 *
 * Requirements:
 *   block = invertX(invertX(block))
 *
 * @param block The block to be inverted
 * @param out   Output for the inverted block. Note that `block` and `out` should not overlap in memory and also should not point to the same data
 * @return      Returns `out`
 */
struct Block* Block_invertX(const struct Block* block,struct Block* out);

/**
 * Inverts the y-axis of the blocks spaces
 *
 * Requirements:
 *   block = invertY(invertY(block))
 *
 * @param block The block to be inverted
 * @param out   Output for the inverted block. Note that `block` and `out` should not overlap in memory and also should not point to the same data
 * @return      Returns `out`
 */
struct Block* Block_invertY(const struct Block* block,struct Block* out);

/**
 * Gets the space state, if it is solid at the given position.
 *
 * @param block The block to check
 * @return      Returns whether the block is solid at the given position. Positions outside the block's bounds should return false.
 */
bool Block_getSpace(const struct Block* block,unsigned short x,unsigned short y);
inline bool Block__getSpace(const struct Block* block,unsigned short x,unsigned short y){
	return Bits_get(block->spaces,y*block->width + x);	
}

/**
 * Sets the space state, if it is solid at the given position.
 *
 * @param block The block to change
 * @return      Returns whether the operation succeeded. Example of a failure is when the given position is outside the block's bounds.
 */
bool Block_setSpace(struct Block* block,unsigned short x,unsigned short y,bool state);

inline void Block_setSpacesFromBitlist(struct Block* block,byte* bitlist,size_t size){
	memcpy(block->spaces,bitlist,size);
}

/**
 * Gets the block's total width
 *
 * @param block The block
 * @return      Returns the width
 */
inline unsigned short Block_getWidth(const struct Block* block){
	return block->width;
}

/**
 * Gets the block's total height
 *
 * @param block The block
 * @return      Returns the height
 */
inline unsigned short Block_getHeight(const struct Block* block){
	return block->height;
}

void Block_render(const struct Block* block);

#endif
