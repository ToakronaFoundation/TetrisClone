#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "Block.h"
#include "Player.h"
#include "Game.h"

/**
 * Initiate states for drawing in two dimensions
 */
static inline void glInit2dSettings(){
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glClearColor(0,0,0,0);
	glClearDepth(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

/**
 * Should be called whenever framebuffer is resized for fixing the view
 */
static void onResize(GLFWwindow* window,int width,int height){
	//Perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width,height,0,1,-1);
	glMatrixMode(GL_MODELVIEW);

	//Viewport
	glViewport(0,0,width,height);
}

static void onKey(GLFWwindow* window,int key,int scanCode,int action,int modifiers){
	if(action == GLFW_PRESS){
		switch(key){
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window,true);
				break;
			case GLFW_KEY_X:{
				struct Player*const players = glfwGetWindowUserPointer(window);
				Player_rotateBlockLeft(&players[0]);
			}	break;
			case GLFW_KEY_C:{
				struct Player*const players = glfwGetWindowUserPointer(window);
				Player_rotateBlockRight(&players[0]);
			}	break;
			case GLFW_KEY_LEFT:{
				struct Player*const players = glfwGetWindowUserPointer(window);
				--players[0].x;
			}	break;
			case GLFW_KEY_RIGHT:{
				struct Player*const players = glfwGetWindowUserPointer(window);
				++players[0].x;
			}	break;
			case GLFW_KEY_DOWN:{
				struct Player*const players = glfwGetWindowUserPointer(window);
				++players[0].y;
			}	break;
		}
	}
}

#define WINDOW_WIDTH_INITIAL  480
#define WINDOW_HEIGHT_INITIAL 480

struct Player players[4];
unsigned short playerCount = 0;

int main(int argc,const char* argv[]){
	GLFWwindow* gameWindow;

	//Initiate glfw
	glfwInit();

	//Gaqme window
	if(!(gameWindow=glfwCreateWindow(WINDOW_WIDTH_INITIAL,WINDOW_HEIGHT_INITIAL,"Tetris clone",NULL,NULL))){
		fputs("Game window creation failed",stderr);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	puts("Game window created");

	glfwSetFramebufferSizeCallback(gameWindow,onResize);
	glfwSetKeyCallback(gameWindow,onKey);

	glfwMakeContextCurrent(gameWindow);
	glfwSwapInterval(1);
	glInit2dSettings();
	onResize(gameWindow,WINDOW_WIDTH_INITIAL,WINDOW_HEIGHT_INITIAL);

	//Make the GL_POINTS to grid-sized squares, or try at least because this may not work for some OpenGL implementations it seems
	glDisable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);
	glPointSize(GAME_GRID_SIZE);

	//Initiate block types
	unsigned short blockCount = 5;
	struct Block* blocks[blockCount];
	{
		byte spaceBuffer;

		blocks[0] = Block_alloc(3,2);
		spaceBuffer = 0b100111;
		Block_setSpacesFromBitlist(blocks[0],&spaceBuffer,1);

		blocks[1] = Block_alloc(3,2);
		spaceBuffer = 0b010111;
		Block_setSpacesFromBitlist(blocks[1],&spaceBuffer,1);

		blocks[2] = Block_alloc(3,2);
		spaceBuffer = 0b001111;
		Block_setSpacesFromBitlist(blocks[2],&spaceBuffer,1);

		blocks[3] = Block_alloc(4,1);
		spaceBuffer = 0b1111;
		Block_setSpacesFromBitlist(blocks[3],&spaceBuffer,1);

		blocks[4] = Block_alloc(2,2);
		spaceBuffer = 0b1111;
		Block_setSpacesFromBitlist(blocks[4],&spaceBuffer,1);
	}
	
	//Initiate players
	players[0] = (struct Player){
		.x = 3,
		.y = 5,
		.fallTimeCounter = 0,
		.selectedBlock = {NULL,NULL,BLOCK_ROTATION_NONE}
	};
	playerCount = 1;
	Player_selectBlock(&players[0],blocks[0]);
	glfwSetWindowUserPointer(gameWindow,&players[0]);//TODO: Temporary code for testing. Also change the onKey function after changing this (the glfwGetWindowUserPointer(window) calls)

	//Game Loop
	while(!glfwWindowShouldClose(gameWindow)){
		//Events (Input, window, ...)
		glfwPollEvents();

		//Update
		++players[0].fallTimeCounter;
		if(players[0].fallTimeCounter>60){//TODO: 1 block per 60 frames is the fallspeed at the moment. Organize code and change to more customizable
			++players[0].y;
			players[0].fallTimeCounter=0;

			//If the block is lower than the floor
			if(players[0].y>10){//TODO: 10 is the height at the moment. Organize code and change to more customizable later
				//Reset position
				players[0].x = 3;
				players[0].y = 5;

				//Select new block randomly
				Player_selectBlock(&players[0],blocks[rand()%blockCount]);
			}
		}

		//Render
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		{
			//Render player's selected block
			glColor3f(1.0f,1.0f,1.0f);
			if(players[0].selectedBlock.copy){
				glTranslatef(players[0].x*GAME_GRID_SIZE,players[0].y*GAME_GRID_SIZE,0.0f);
				glBegin(GL_POINTS);
					unsigned short w = Block_getWidth (players[0].selectedBlock.copy),
					               y = Block_getHeight(players[0].selectedBlock.copy);
					unsigned short x = w;

					while(y-->0){
						x = w;
						while(x-->0)
							if(Block_getSpace(players[0].selectedBlock.copy,x,y))
								glVertex2i(x*GAME_GRID_SIZE,y*GAME_GRID_SIZE);
					}
				glEnd();
				glTranslatef(-players[0].x*GAME_GRID_SIZE,-players[0].y*GAME_GRID_SIZE,0.0f);
			}
		}

		//Double buffering
		glfwSwapBuffers(gameWindow);
	}

	//Termination and freeing resources
	glfwDestroyWindow(gameWindow);
	glfwTerminate();

	return EXIT_SUCCESS;
}
