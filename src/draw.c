#include "draw.h"

#include <GLFW/glfw3.h>
#include "Block.h"
#include "Map.h"
#include "Game.h"

void Block_draw(const struct Block* block,int x,int y){
	if(block){
		glBegin(GL_POINTS);
			unsigned short w  = Block_getWidth (block),
			               iy = Block_getHeight(block);
			unsigned short ix = w;

			//Render each grid square based on the space data, if it is solid at that coordinate point
			while(iy-->0){
				ix = w;
				while(ix-->0)
					if(Block_getSpace(block,ix,iy))
						glVertex2i((x+ix)*GAME_GRID_SIZE + GAME_GRID_SIZE/2,(y+iy)*GAME_GRID_SIZE + GAME_GRID_SIZE/2);
			}
		glEnd();
	}
}

void Map_draw(const struct Map* map,int x,int y){
	if(map){
		glColor3f(0.2f,0.2f,0.2f);
		glRecti(x*GAME_GRID_SIZE,y*GAME_GRID_SIZE,map->width*GAME_GRID_SIZE,map->height*GAME_GRID_SIZE);

		glColor3f(0.5f,0.5f,0.5f);
		glBegin(GL_POINTS);
			unsigned short w  = map->width,
			               iy = map->height;
			unsigned short ix = w;

			//Render each grid square based on the space data, if it is solid at that coordinate point
			while(iy-->0){
				ix = w;
				while(ix-->0)
					if(Map_getSpace(map,ix,iy))
						glVertex2i((x+ix)*GAME_GRID_SIZE + GAME_GRID_SIZE/2,(y+iy)*GAME_GRID_SIZE + GAME_GRID_SIZE/2);
			}
		glEnd();
	}
}
