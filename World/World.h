//  PPROG
//	World.c
//  Created by David del Val on 15/12/2019


#ifndef WORLD_H
#define WORLD_H

#include <pthread.h>
#include "Entity.h"
#include "Combat.h"
#include "Display.h"
#include "TrigReact.h"
#include "EntityDic.h"
typedef struct _World World;

/**
 * @brief Initializes a new world.
 * @return New World*.
**/
World* wo_ini();
/**
 * @brief Initializes a new world read from a file.
 * @param c: Name of the file where the information of the world is located
 *           (Without folder and .txt).
 * @return New world*.
**/
World* wo_get(char* c);
/**
 * @brief Frees all the memory occupied by the world passed as input.
 * @param w: World * of the world to be freed.
 * @return
**/
void wo_free(World *w);

/**
 * @brief Reads the world from a file (Mainly used by wo_get).
 * Format:
 * Room
 *
 * player
 * number_of_allies
 * allies
 * number_of_enemies
 * enemies
 *
 * The format of every entity is:
 * id ipos jpos name
 * @param f
 * @return World*
**/
World* wo_load(FILE* f);
/**
 * @brief Executes the given world.
 * @param w: World to be played.
 * @return NULL in case of error or the world pointer passed as input otherwise.
**/
World* wo_launch(World* w);
/**
 * @brief Executes the next world.
 * @return NULL in case of error or the loaded world pointer otherwise.
**/
World* wo_getNext();
/**
 * @brief Sets a name the given world.
 * @param w: World whose name is going to be modified.
 * @param c: String containing the new name of the world.
 * @return NULL in case of error or the world pointer passed as input otherwise.
**/
World* wo_setName(World* wp,char*c);
/**
 * @brief Gets the name of given world.
 * @param w: World whose name is going to be gotten.
 * @return NULL in case of error or a char pointer to the world's name.
**/
const char* wo_getName(World* wp);
/**
 * @brief Transfers the player from a world to another.
 * @param next: The new world of the player.
 * @param prev: The old world where the player was.
 * @return NULL in case of error or a pointer to the new world.
**/
World* wo_transferPlayer(World* next, World* prev);

#endif
