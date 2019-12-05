#ifndef entity
#define entity

#include "Inventory.h"
#include "Sprite.h"
#include "Display.h"
#include "Object.h"
#include "Utility.h"

typedef struct _Entity Entity;

typedef enum {PLAYER = 1, ENEMY = 2, ALLY = 3} entType;


/*Creates an entity and assigns a name, a sprite, an entType, a x and y coordinate
   and initializes the inventory and attribute field. If either of the pounter arguments is NULL, it sets
   that field to NULL*/
Entity *entity_ini (char *name, entType t, int i, int j);

/* Reads from the file passed as argument the  name, entity type (player (1), enemy(2) or ally (3)),
   x Coodinate, y coordinate and sprite in this order and initializes an entity with
   those fields. */
Entity *entity_load(FILE* f, Display *r);

void entity_free(Entity *p);

/* Sets the character's name, if either of the arguments is NULL it will return ERROR*/
Entity* entity_setName(Entity* p, char* c);

/* Sets the character's sprite, if either of the arguments is NULL it will return ERROR */
Entity* entity_setSprite(Entity* p, int d);

/* Sets the character's type, if the entity atgument is NULL, or the entity type
   is different from 1, 2 or 3 it will return ERROR*/
Entity* entity_setEntType(Entity* p, entType t);

/* Sets the character's x coordinate,  if the entity atgument is NULL or the coordinate
   is smaller than 0 it will return ERROR */
Entity* entity_setCoordI(Entity* p, int i);

/* Sets the character's y coordinate,  if the entity atgument is NULL or the coordinate
   is smaller than 0 it will return ERROR */
Entity* entity_setCoordJ(Entity* p, int j);


Entity* entity_addtoDisplay(Entity* e, Display* dis);

#endif