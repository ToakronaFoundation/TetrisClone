#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "Block.h"
#include "Player.h"
#include "Game.h"
#include "Map.h"
#include "draw.h"

struct GameData{
	struct Player players[4];//TODO: Multiplayer using more flexible data structure
	unsigned short playerCount;
	struct Map* map;
	unsigned short blockCount;
	struct Block** blocks;
};

void Game_blockTouchesBottom(struct Player* player,struct Map* map,struct Block** blocks,unsigned int blockCount){
	Map_imprintBlock(map,player->selectedBlock.copy,player->x,player->y);

	//Reset position
	player->x = 3;
	player->y = 0;

	//Select new block randomly
	Player_selectBlock(player,blocks[rand()%blockCount]);
}

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

//TODO: Rewrite input system for extensibility. Maybe using structures holding "input systems" and all players haing a pointer to the system and the data the system uses.
static void onKey(GLFWwindow* window,int key,int scanCode,int action,int modifiers){
	if(action == GLFW_PRESS){
		struct GameData*const gameData = glfwGetWindowUserPointer(window);
		switch(key){
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window,true);
				break;
			case GLFW_KEY_X:{
				Player_rotateBlockLeft(&gameData->players[0]);
			}	break;
			case GLFW_KEY_C:{
				Player_rotateBlockRight(&gameData->players[0]);
			}	break;
			case GLFW_KEY_LEFT:{
				Player_moveX(&gameData->players[0],gameData->map,-1);
			}	break;
			case GLFW_KEY_RIGHT:{
				Player_moveX(&gameData->players[0],gameData->map,1);
			}	break;
			case GLFW_KEY_DOWN:{
				if(!Player_moveY(&gameData->players[0],gameData->map,1))
					Game_blockTouchesBottom(&gameData->players[0],gameData->map,gameData->blocks,gameData->blockCount);
			}	break;
		}
	}
}

#define WINDOW_WIDTH_INITIAL  480
#define WINDOW_HEIGHT_INITIAL 480

int main(int argc,const char* argv[]){
	struct GameData gameData = {.playerCount = 0};
	GLFWwindow* gameWindow;

	//Initiate glfw
	glfwInit();

	//Game window
	if(!(gameWindow=glfwCreateWindow(WINDOW_WIDTH_INITIAL,WINDOW_HEIGHT_INITIAL,"Tetris clone",NULL,NULL))){
		fputs("Game window creation failed",stderr);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	puts("Game window created");
	glfwSetWindowUserPointer(gameWindow,&gameData);//TODO: Temporary code for testing. Also change the onKey function after changing this (the glfwGetWindowUserPointer(window) calls)

	//Set callbacks
	glfwSetFramebufferSizeCallback(gameWindow,onResize);
	glfwSetKeyCallback(gameWindow,onKey);

	//Initiate rendering
	glfwMakeContextCurrent(gameWindow);
	glfwSwapInterval(1);
	glInit2dSettings();
	onResize(gameWindow,WINDOW_WIDTH_INITIAL,WINDOW_HEIGHT_INITIAL);

	//Make the GL_POINTS to grid-sized squares, or try at least because this may not work for some OpenGL implementations it seems
	glDisable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);
	glPointSize(GAME_GRID_SIZE);

	//Initiate gameData.map
	if(!(gameData.map = Map_alloc(10, 15))){
		fprintf(stderr,"Error: Cannot allocate gameData.map with the size %ux%u\n",10,15);
		return 1;
	}

	//Initiate block types
	gameData.blockCount = 7;
	gameData.blocks = malloc(sizeof(struct Block*)*gameData.blockCount);
	{
		byte spaceBuffer;

		//J
		gameData.blocks[0] = Block_alloc(3,2);
		spaceBuffer = 0b100111;
		Block_setSpacesFromBitlist(gameData.blocks[0],&spaceBuffer,1);

		//T
		gameData.blocks[1] = Block_alloc(3,2);
		spaceBuffer = 0b010111;
		Block_setSpacesFromBitlist(gameData.blocks[1],&spaceBuffer,1);

		//L
		gameData.blocks[2] = Block_alloc(3,2);
		spaceBuffer = 0b001111;
		Block_setSpacesFromBitlist(gameData.blocks[2],&spaceBuffer,1);

		//O
		gameData.blocks[3] = Block_alloc(4,1);
		spaceBuffer = 0b1111;
		Block_setSpacesFromBitlist(gameData.blocks[3],&spaceBuffer,1);

		//I
		gameData.blocks[4] = Block_alloc(2,2);
		spaceBuffer = 0b1111;
		Block_setSpacesFromBitlist(gameData.blocks[4],&spaceBuffer,1);

		//S
		gameData.blocks[5] = Block_alloc(3,2);
		spaceBuffer = 0b011110;
		Block_setSpacesFromBitlist(gameData.blocks[5],&spaceBuffer,1);

		//Z
		gameData.blocks[6] = Block_alloc(3,2);
		spaceBuffer = 0b110011;
		Block_setSpacesFromBitlist(gameData.blocks[6],&spaceBuffer,1);
	}
	
	//Initiate players
	gameData.players[0] = (struct Player){
		.x = 3,
		.y = 0,
		.fallTimeCounter = 0,
		.selectedBlock = {NULL,NULL,BLOCK_ROTATION_NONE}
	};
	gameData.playerCount = 1;
	Player_selectBlock(&gameData.players[0],gameData.blocks[0]);

	//Game Loop
	while(!glfwWindowShouldClose(gameWindow)){
		//Events (Input, window, ...)
		glfwPollEvents();

		//Update
		++gameData.players[0].fallTimeCounter;
		if(gameData.players[0].fallTimeCounter>60){//TODO: 1 block per 60 frames is the fallspeed at the moment. Organize code and change to more customizable
			if(!Player_moveY(&gameData.players[0],gameData.map,1))
				Game_blockTouchesBottom(&gameData.players[0],gameData.map,gameData.blocks,gameData.blockCount);
			gameData.players[0].fallTimeCounter=0;
		}

		//Render
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glColor3f(0.5f,0.5f,0.5f);
		Map_draw(gameData.map,0,0);
		glColor3f(1.0f,1.0f,1.0f);
		Block_draw(gameData.players[0].selectedBlock.copy,gameData.players[0].x,gameData.players[0].y);

		//Double buffering
		glfwSwapBuffers(gameWindow);
	}

	//Termination and freeing resources
	glfwDestroyWindow(gameWindow);
	glfwTerminate();

	return EXIT_SUCCESS;
}
