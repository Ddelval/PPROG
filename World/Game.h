
#ifndef GAME_H
#define GAME_H

#include "World.h"

typedef struct _Game Game;

/**
 * @brief Loads all the worlds
 *
 * This function will load all the worlds that
 * are stored in the folder World/
 *
 * @param g Game in which the worlds will be loaded
 * @return  NULL if error
**/
Game* game_loadWorlds();
/**
 * @brief Opens, initializes, loads the worlds and execute the game.
 *        Finally closes and frees it.
 * @return  1 in case of error 0 otherwise.
**/
int game_launch();
#endif
