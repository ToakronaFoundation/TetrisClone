#ifndef __TOAKRONAF_TETRIS_DRAW_H_INCLUDED__
#define __TOAKRONAF_TETRIS_DRAW_H_INCLUDED__

struct Block;
struct Map;

void Block_draw(const struct Block* block,int x,int y);
void Map_draw(const struct Map* map,int x,int y);

#endif