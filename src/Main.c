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
	switch(key){
		case GLFW_KEY_ESCAPE:
			//TODO: Quit game
			break;
		case GLFW_KEY_LEFT:
			if(action == GLFW_PRESS){
				struct Player*const player = glfwGetWindowUserPointer(window);
				--player->x;
			}
			break;
		case GLFW_KEY_RIGHT:
			if(action == GLFW_PRESS){
				struct Player*const player = glfwGetWindowUserPointer(window);
				++player->x;
			}
			break;
	}
}

#define WINDOW_WIDTH_INITIAL  480
#define WINDOW_HEIGHT_INITIAL 480

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

	/*
	byte spaceBuffer;
	
	struct Block* block = Block_allocate(3,2);
	spaceBuffer = 0b111010;
	Block_setSpacesFromBitlist(block,&spaceBuffer,1);
	*/
	struct Player player = {3,5,0,NULL};
	glfwSetWindowUserPointer(gameWindow,&player);//TODO: Temporary code for testing. Also change the onKey function after changing this (the glfwGetWindowUserPointer(window) calls)

	//Game Loop
	while(!glfwWindowShouldClose(gameWindow)){
		//Events (Input, window, ...)
		glfwPollEvents();

		//Update
		++player.fallTimeCounter;
		if(player.fallTimeCounter>60){
			++player.y;
			player.fallTimeCounter=0;
		}
		
		//Render
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3f(0.5f,0.5f,0.5f);
		glBegin(GL_QUADS);
			glVertex2i(16,16);
			glVertex2i(16,32);
			glVertex2i(32,32);
			glVertex2i(32,16);
		glEnd();

		{
			glColor3f(1.0f,1.0f,1.0f);
			glTranslatef(player.x*GAME_GRID_SIZE,player.y*GAME_GRID_SIZE,0.0f);
			glBegin(GL_POINTS);
				/*
				unsigned short x = Block_getWidth (player->block),
				               y = Block_getHeight(player->block);
				while(y-->0)
					while(x-->0)
						if(Block_getState(block,x,y))
							glVertex2i(x*GAME_GRID_SIZE,y*GAME_GRID_SIZE);
				*/
				glVertex2i(0*GAME_GRID_SIZE,0*GAME_GRID_SIZE);
				glVertex2i(1*GAME_GRID_SIZE,0*GAME_GRID_SIZE);
				glVertex2i(2*GAME_GRID_SIZE,0*GAME_GRID_SIZE);
				glVertex2i(1*GAME_GRID_SIZE,1*GAME_GRID_SIZE);
			glEnd();
			glTranslatef(-player.x*GAME_GRID_SIZE,-player.y*GAME_GRID_SIZE,0.0f);
		}

		//Double buffering
		glfwSwapBuffers(gameWindow);
	}

	//Termination and freeing resources
	glfwDestroyWindow(gameWindow);
	glfwTerminate();

	return EXIT_SUCCESS;
}
