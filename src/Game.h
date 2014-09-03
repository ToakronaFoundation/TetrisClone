#ifndef __TOAKRONAF_TETRIS_GAME_H_INCLUDED__
#define __TOAKRONAF_TETRIS_GAME_H_INCLUDED__

#include "Player.h"
#include <stdbool.h>
struct Map;
struct BlockTypeData;
struct GLFWwindow;

#define GAME_GRID_SIZE 16

#define GAME_INITIAL_WIDTH  10
#define GAME_INITIAL_HEIGHT 20

//TODO: These should be a in-game settings
#define GAME_DOWNKEY_TIME 15
#define GAME_DOWNKEY_SPEED 2
#define GAME_FALL_SPEED     60

struct GameData{
	/**
	 * Represents all the available block types
	 */
	const struct BlockTypeData* blockTypes;

	/**
	 * Player data
	 */
	struct Player players[4];//TODO: Multiplayer using more flexible data structure
	unsigned short playerCount;

	/**
	 * Map data
	 */
	struct Map* map;
	
	struct Map* fallingBlocks;
	unsigned short blockFalling;
	short topLineRemoved;
	int animationFallCounter;
};

void Game_update(struct GameData* gameData,struct GLFWwindow* window);//TODO: Fix input system and remove window parameter
void Game_render(struct GameData* gameData);
void Game_blockTouchesBottom(struct GameData* gameData, unsigned short playerIndex, struct Map* map, const struct BlockTypeData* blockTypes);

#endif
