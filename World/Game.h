
#ifndef GAME_H
#define GAME_H

#include "World.h"

typedef struct _Game Game;
Game* game_loadWorlds();

int game_launch();
#endif