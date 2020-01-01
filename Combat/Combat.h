/*  Combat.h    */

#ifndef COMBAT_H
#define COMBAT_H



#include "Entity.h"

typedef struct _Combat Combat;

Combat* combat_launch(Entity* player, Entity* enemy);
/*
Name: combat_ini
Imputs: entity * player | entity * enemy
Outputs: * combat
Functioning: Creates an structure called combat that manages every aspect of the combat.
*/
Combat* combat_ini(Entity* player, Entity* enemy);

/*
    Executes a combat, already initialized and loaded. Does not finish until one of the
    players is dead. NOTE: in order to make development easy, we can exit by pressing Q.
*/
Combat* combat_execute(Combat* c);

/*
    Prepares the graphical interface for a combat, rendering it.
*/
Combat* combat_load(Combat *c);

/*
    Frees a combat.
*/
void combat_free(Combat* c);

#endif
