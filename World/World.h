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

World* wo_ini();

World* wo_get(char* c);

void wo_free(World *w);

/**
 * @brief Reads the world from a file
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
 */
World* wo_load(FILE* f);
World* wo_launch(World* w);
World* wo_getNext();

World* wo_setName(World* wp,char*c);
const char* wo_getName(World* wp);
World* wo_transferPlayer(World* next, World* prev);
#endif