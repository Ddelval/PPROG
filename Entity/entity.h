#ifndef entity
#define entity

#include "inventory.h"
#include "Sprite.h"
#include "object.h"
#include "types.h"

typedef struct _Entity Entity;

typedef enum {PLAYER = 1, ENEMY = 2, ALLY = 3} entType;


/*Creates an entity and assigns a name, a sprite, an entType, a x and y coordinate
   and initializes the inventory and attribute field. If either of the pounter arguments is NULL, it sets
   that field to NULL*/
Entity *entity_ini (char *name, entType t, int x, int y);

/* Reads from the file passed as argument the  name, entity type (player (1), enemy(2) or ally (3)),
   x Coodinate, y coordinate and sprite in this order and initializes an entity with
   those fields. */
Entity *entity_load(char *file, Room *r);



/* Sets the character's name, if either of the arguments is NULL it will return ERROR*/
Status entity_setName(Entity* p, char* c);

/* Sets the character's sprite, if either of the arguments is NULL it will return ERROR */
Status entity_setSprite(Entity* p, Sprite *s);

/* Sets the character's type, if the entity atgument is NULL, or the entity type
   is different from 1, 2 or 3 it will return ERROR*/
Status entity_setEntType(Entity* p, entType t);

/* Sets the character's x coordinate,  if the entity atgument is NULL or the coordinate
   is smaller than 0 it will return ERROR */
Status entity_setCoordX(Entity* p, int x);

/* Sets the character's y coordinate,  if the entity atgument is NULL or the coordinate
   is smaller than 0 it will return ERROR */
Status entity_setCoordY(Entity* p, int y);
