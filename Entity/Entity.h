/* Entity.h */

#ifndef entity
#define entity

#include "Inventory.h"
#include "Sprite.h"
#include "Display.h"
#include "Object.h"
#include "ObjectDic.h"
#include "DialogMan.h"
#include "Utility.h"
#include "Trigger.h"

typedef struct _Entity Entity;

typedef enum {PLAYER = 1, ENEMY = 2, ALLY = 3} ent_type;


/*Creates an entity and assigns a name, a sprite, an ent_type, a x and y coordinate
   and initializes the inventory and attribute field. If either of the pounter arguments is NULL, it sets
   that field to NULL*/
Entity *entity_ini (char *name, ent_type t, int i, int j);

/**
 * @brief Reads the entity from a file
 * Format:
 * id type spr_id
 * name
 * ipos jpos has_dialog
 * attributes
 * inventory
 *
 * @param f File where the entity is
 * @param r Display to which it will be added
 * @return Entity*
 */
Entity *entity_load(FILE* f, Display *r);

void entity_free(Entity *p);

Entity* entity_copy(Entity* e);

/* Sets the character's name, if either of the arguments is NULL it will return ERROR*/
Entity* entity_setName(Entity* p, char* c);

/* Sets the character's sprite, if either of the arguments is NULL it will return ERROR */
Entity* entity_setSprite(Entity* p, int d);

/*
    Gets the sprite associated to this entity.
*/
Sprite* entity_getSprite(Entity* p);

/* Sets the character's type, if the entity atgument is NULL, or the entity type
   is different from 1, 2 or 3 it will return ERROR*/
Entity* entity_setEntType(Entity* p, ent_type t);

/* Sets the character's x coordinate,  if the entity atgument is NULL or the coordinate
   is smaller than 0 it will return ERROR */
Entity* entity_setCoordI(Entity* p, int i);

/* Sets the character's y coordinate,  if the entity atgument is NULL or the coordinate
   is smaller than 0 it will return ERROR */
Entity* entity_setCoordJ(Entity* p, int j);


Entity* entity_addtoDisplay(Entity* e, Display* dis);

char* entity_getName(Entity* p);

Attributes* entity_getAttributes(Entity* p);

Inventory * entity_getInventory( Entity * p);

Entity* entity_setAttributes(Entity* p, Attributes* a);

Entity* entity_moveUp(Entity* p);
Entity* entity_moveDown(Entity* p);
Entity* entity_moveLeft(Entity* p);
Entity* entity_moveRight(Entity* p);
int entity_getRoomIndex(const Entity* en);
Entity* entity_addItem(Entity* en,int itemId, int quantity);
const Inventory* entity_getInvRef(Entity*en);

/*
    Sets the DialogMan of a given entity, copying it.
*/
Entity* entity_setDialogs(Entity* e, DialogMan* dman);

/*
    Gets the next line in the current dialog.
*/
char* entity_getLine(Entity* e);

/*
    Resets the current dialog; sets it to the beginning.
*/
Entity* entity_resetDialog(Entity* e);

/*
    Sets the ID of the current dialog.
*/
Entity* entity_setDialog(Entity* e, int dialogid);

int entity_getId(Entity* e);

Entity* entity_setInventory(Entity* e, Inventory* inv);
bool entity_getHasDialog(Entity *e);
Entity* entity_processAlly(Entity* e);
const DialogMan* entity_getDialogs(Entity *e);
#endif
