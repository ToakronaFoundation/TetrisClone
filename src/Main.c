#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "Block.h"
#include "BlockTypeData.h"
#include "Player.h"
#include "Game.h"
#include "Map.h"

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

//TODO: Rewrite input system for extensibility. Maybe using structures holding "input systems" and all players having a pointer to the system and the data the system uses.
static void onKey(GLFWwindow* window,int key,int scanCode,int action,int modifiers){
	struct GameData*const gameData = glfwGetWindowUserPointer(window);
	if(action == GLFW_PRESS){
		switch(key){
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window,true);
				break;
			case GLFW_KEY_X:
				Player_rotateBlockLeft(&gameData->players[0],gameData->map);
				break;
			case GLFW_KEY_C:
				Player_rotateBlockRight(&gameData->players[0],gameData->map);
				break;
			case GLFW_KEY_LEFT:
				Player_moveX(&gameData->players[0],gameData->map,-1);
				break;
			case GLFW_KEY_RIGHT:
				Player_moveX(&gameData->players[0],gameData->map,1);
				break;
			case GLFW_KEY_DOWN:
				gameData->players[0].downKeyCounter = -GAME_DOWNKEY_TIME;
				gameData->players[0].fallTimeCounter = 0;

				if(!Player_moveY(&gameData->players[0],gameData->map,1))
					Game_blockTouchesBottom(gameData, 0,gameData->map,gameData->blockTypes);
				break;
		}
	}
}

#define WINDOW_WIDTH_INITIAL  480
#define WINDOW_HEIGHT_INITIAL 480

int main(int argc,const char* argv[]){
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

	struct GameData gameData = {.playerCount = 0, .blockFalling = 0, .topLineRemoved = 0, .animationFallCounter = 0};
	glfwSetWindowUserPointer(gameWindow,&gameData);

	//Initiate gameData.map
	if(!(gameData.map = Map_alloc(GAME_INITIAL_WIDTH,GAME_INITIAL_HEIGHT))){
		fprintf(stderr,"Error: Cannot allocate gameData.map with the size %ux%u\n",GAME_INITIAL_WIDTH,GAME_INITIAL_HEIGHT);
		return 1;
	}

	//Initiate block types
	struct BlockTypeData* blockTypes;
	if(!(blockTypes = BlockTypeData_alloc(7))){
		fprintf(stderr,"Error: Cannot allocate %u block types\n",7);
		return 1;
	}
	{
		byte spaceBuffer;

		//J
		blockTypes->blocks[0] = Block_alloc(3,2);
		spaceBuffer = 0b100111;
		Block_setSpacesFromBitlist(blockTypes->blocks[0],&spaceBuffer,1);

		//T
		blockTypes->blocks[1] = Block_alloc(3,2);
		spaceBuffer = 0b010111;
		Block_setSpacesFromBitlist(blockTypes->blocks[1],&spaceBuffer,1);

		//L
		blockTypes->blocks[2] = Block_alloc(3,2);
		spaceBuffer = 0b001111;
		Block_setSpacesFromBitlist(blockTypes->blocks[2],&spaceBuffer,1);

		//O
		blockTypes->blocks[3] = Block_alloc(4,1);
		spaceBuffer = 0b1111;
		Block_setSpacesFromBitlist(blockTypes->blocks[3],&spaceBuffer,1);

		//I
		blockTypes->blocks[4] = Block_alloc(2,2);
		spaceBuffer = 0b1111;
		Block_setSpacesFromBitlist(blockTypes->blocks[4],&spaceBuffer,1);

		//S
		blockTypes->blocks[5] = Block_alloc(3,2);
		spaceBuffer = 0b011110;
		Block_setSpacesFromBitlist(blockTypes->blocks[5],&spaceBuffer,1);

		//Z
		blockTypes->blocks[6] = Block_alloc(3,2);
		spaceBuffer = 0b110011;
		Block_setSpacesFromBitlist(blockTypes->blocks[6],&spaceBuffer,1);
	}
	gameData.blockTypes = blockTypes;

	//Initiate players
	gameData.players[0] = (struct Player){
		.x = 3,
		.y = 0,
		.fallTimeCounter = 0,
		.downKeyCounter = 0,
		.selectedBlock = {NULL,NULL,BLOCK_ROTATION_NONE}
	};
	gameData.playerCount = 1;
	Player_selectBlock(&gameData.players[0],gameData.blockTypes->blocks[0]);


	//Game Loop
	while(!glfwWindowShouldClose(gameWindow)){
		//Events (Input, window, ...)
		glfwPollEvents();

		//Continuous key down //TODO: Acceleration like Tengen Tetris, continuous left and right keys
		if(glfwGetKey(gameWindow,GLFW_KEY_DOWN)){
			if(++gameData.players[0].downKeyCounter > GAME_DOWNKEY_SPEED){
				gameData.players[0].downKeyCounter = 0;

				//If not able to move down
				if(!Player_moveY(&gameData.players[0],gameData.map,1)){
					Game_blockTouchesBottom(&gameData,0,gameData.map,gameData.blockTypes);
					gameData.players[0].fallTimeCounter = 0;
				}
			}
		}

		//Update
		Game_update(&gameData,gameWindow);

		//Render
		Game_render(&gameData);

		//Double buffering
		glfwSwapBuffers(gameWindow);
	}

	//Termination and freeing resources
	glfwDestroyWindow(gameWindow);
	glfwTerminate();

	return EXIT_SUCCESS;
}
