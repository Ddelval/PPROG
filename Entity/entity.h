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
Entity *entity_load(char *file);



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



/* Allocates memory for a char pointer that will be returned with the name of p
unless p or p's name is NULL*/
char *entity_getName(Entity* p);

/* Returns a copy of the entity's sprite, unless argument p is NULL or doesn't
have a sprite, in which case will return NULL*/
Sprite *entity_getSprite(Entity* p);

/* Returns 0 if p is NULL or the entType field has not been defined yet; or
the entTypt of p otherwise. */
entType entity_getEntType(Entity* p);

/* Returns -1 if p is NULL or the x coordinate of p otherwise*/
int entity_getCoordX(Entity* p);

/* Returns -1 if p is NULL or the y coordinate of p otherwise*/
int entity_getCoordY(Entity* p);

/* Returns a pointer to a copy of the the attribute field of the entity passed
 as argument, returns NULL if there is an error, p is NULL or there are no attributes*/
atb *entity_getAttribute(Entity* p);

/* Returns a pointer to a copy of the inventory field of the entity passed as
argument, returns NULL if there is an error, p is NULL or there is no inventory*/
inventory *entity_getInventory(Entity* p);



/* The canvas will be a square with the coordinate (0,0) in the top left corner
and coordinate x and y always positive.
Arguments: p, the entity will be moved and canv: size, the size of the canvas
where the player is located.
If p is NULL return ERROR, if the player tries to go out of the canvas he
will not move and will recieve a warning error and the function will return
OK. It will increase coordinate once otherwise.*/
Status entity_moveUp(Entity* p);
Status entity_moveDown(Entity* p, int canv_size);
Status entity_moveLeft(Entity* p);
Status entity_moveRight(Entity* p, int canv_size);



/* Will free all the memory used by the entity and set its pointer to NULL*/
void entity_destroy(Entity *p);


#endif

