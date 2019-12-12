
#ifndef COMBAT_H
#define COMBAT_H



#include "Entity.h"

typedef struct _Combat Combat;

/*
Name: combat_ini
Imputs: entity * player | entity * enemy
Outputs: * combat
Functioning: Creates an structure called combat that manages every aspect of the combat.
*/
Combat * combat_ini(Entity * player, Entity * enemy);

/*
Name: combat_process
Imputs: combat * state
Outputs : None
Funcioting: It creates a loop that ends when someone health is 0, meanwhile it executes
the movements.
*/
void combat_process(Combat * state);
/*
Name: moveset_loader(entity * fighter, combat * state)
Imputs: entity * player | combat * state
Outputs: None
Functioning: It decides the four movements the player will be able to execute
*/

int player_choice();

/*
Name: IA_choice
Imputs: combat structure
Outputs: int
Functioning: The computer receives the structure and manages to choose a "smart" movement*/

int IA_choice(Combat * state);

/*
Name: move_exe
Imputs: int movement | combat * struct
Outputs: None
Functioning: This function starts deciding who atacks and who defends, then it executes the moveement
*/
void move_exe(int movement, Combat * state);


/* This function will execute the hole combat, main function. */
int combat_exe(Combat *c);

/* This function will apply the changes each movement makes on the receiver entity */
int movement_exe(Combat * c, int action, int ent);

/*This funciton frees everything related to struct combat*/
void combat_destroy(Combat * c);


//OJO HACE FALTA HACER FUNCIONES COPY COMENTAR EN CLASE
#endif
