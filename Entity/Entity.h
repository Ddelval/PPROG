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


/*
    Given a name, an entity type and a pair of coordinates, i and j, it allocates memory
    for a entity, assigning the given parameters to it and initializing the attribute
    and inventory field.
    Returns: NULL in case of error or the initialized Entity* otherwise.
 */
Entity *entity_ini (char *name, ent_type t, int i, int j);

/*
    Creates and initilizes an entity using the file given as input.
    The file format should be:
      id type spr_id
      name
      ipos jpos has_dialog
      attributes
      inventory
    Inputs:
      f: File where the entity is
      r. Display to which it will be added
    Returns NULL in case of error or the Entity* otherwise.
*/
Entity *entity_load(FILE* f, Display *r);

/*
    Stores the data of the entity in the given file in a format
    that can later be read by entity_load
*/

Entity* entity_saveToFile(Entity* e,FILE* f);

/*
    Given an Entity* it frees it and all its initilized fields.
*/
void entity_free(Entity *p);

/*
    Given an Entity* it inializes a new one where all the information from the
    input entity will be copied.
    Returns: NULL in case of error or the copied entity in an Entity*.
*/
Entity* entity_copy(Entity* e);

/*
    Given an entity and a name, this function sets the given name to the entity.
    Returns: NULL if there has been any problem (doing nothing to the original
    entity) or the Entity* in case everything goes as expected.
*/
Entity* entity_setName(Entity* p, char* c);

/*
    Given an Entity* and the id of the srpite which the user wants to assign to
    to that entity, this function looks for the sprite with the given id and assigns
    it to the entity passed as argument.
    Returns: NULL if the sprite with that id does not exist in our dictionaries
    or if any other problem ot the Entity* otherwise.
*/
Entity* entity_setSprite(Entity* p, int d);

/*
    Given an entity and a sprite, it sets the sprite of the entity to the sprite
    given.
*/
Entity* entity_setActualSprite(Entity* p,Sprite* s);

/*
    Given an Entity* it returns an Sprite* with copy of the sprite associated to
    that entity or NULL in case of errors.
*/
Sprite* entity_getSprite(Entity* p);

/*
    Given an entity and a ent_type, this function sets the given ent_type to the
    entity.
    Returns: NULL if there has been any problem or the Entity* in case
    everything goes as expected.
*/
Entity* entity_setEntType(Entity* p, ent_type t);

/*
    Given an entity and a integer coordinate i, this function moves the entity to
    the desired position in the room, and sets its new position accordingly in the
    Entity structure.
    Returns: NULL if there has been any problem or the Entity* in case everything
    goes as expected.
*/
Entity* entity_setCoordI(Entity* p, int i);

/*
    Given an entity and a integer coordinate j, this function moves the entity to
    the desired position in the room, and sets its new position accordingly in the
    Entity structure.
    Returns: NULL if there has been any problem or the Entity* in case everything
    goes as expected.
*/
Entity* entity_setCoordJ(Entity* p, int j);

/*
    Returns -1 in case of wrong arguments or the actual i coordenate of the entity
    passed as argument otherwise.
*/
int entity_getCoordI(Entity* p);

/*
    Returns -1 in case of wrong arguments or the actual j coordenate of the entity
    passed as argument otherwise.
*/
int entity_getCoordJ(Entity* p);

/*
    Given an Entity* and a Display* adds the entity's sprite to the current room.
    Returns NULL in case of any problem or the Entity* passed as argument otherwise.
*/
Entity* entity_addtoDisplay(Entity* e, Display* dis);

/*
    Given an Entity* it returns a string with copy of the name associated to
    that entity or NULL in case of errors.
*/
char* entity_getName(Entity* p);

/*
    Given an Entity* it returns the pointer to the entity's Attributes or NULL
    in case of errors.
*/
Attributes* entity_getAttributes(Entity* p);

/*
    Given an Entity* it returns the pointer to the entity's Inventory or NULL
    in case of errors.
*/
Inventory * entity_getInventory( Entity * p);

/*
    Given an entity and an array of Attributes sets the entity's attributes to a
    copy of the attributes passed as argument.
    Returns: NULL if there has been any problem or the Entity* in case
    everything goes as expected.
*/
Entity* entity_setAttributes(Entity* p, Attributes* a);


/*
    It moves the entity and its sprite one step up/down/rigth/left if it is possible
    (no collider near him and not close to the screen border). The scroll boolean
    variable indicates whether or not the movement of this sprite should affect the
    scrolling of the room.
    Returns: NULL if there has been any problem or the Entity* in case
    everything goes as expected.
*/
Entity* entity_moveUp(Entity* p,bool scroll);
Entity* entity_moveDown(Entity* p,bool scroll);
Entity* entity_moveLeft(Entity* p,bool scroll);
Entity* entity_moveRight(Entity* p,bool scroll);

/*
    Given an Entity* returs the index of the room where it is located or -1
    in case of ERROR.
*/
int entity_getRoomIndex(const Entity* en);

/*
    Given an entity, a object id and how many of those objects must be added,
    the function looks for the object with that id and adds to the entity's
    inventory as many of these objects are specified.
    Returns: NULL in case of error or object not found in the objects dictionary
    or the Entity* otherwise.
*/
Entity* entity_addItem(Entity* en,int itemId, int quantity);

/*
    Given an Entity* it returns the pointer to the entity's Inventory or NULL
    in case of errors.
*/
Inventory* entity_getInvRef(Entity*en);

/*
    Given an entity and a DialogMansets, the function copies the DialogMan into
    the corresponding entity's field.
    Returns: NULL in case of error or the Entity* otherwise.
*/
Entity* entity_setDialogs(Entity* e, DialogMan* dman);

/*
    Given an entity, gets the next line in the current dialog.
    Returns: NULL in case of error or a char pointer to the string which contains
    the text.
*/
char* entity_getLine(Entity* e);

/*
    Given an entity, resets the current dialog; sets it to the beginning.
    Returns: NULL in case of error or the Entity* otherwise.
 */
Entity* entity_resetDialog(Entity* e);

/*
    Given an entity, sets the ID of the current dialog.
    Returns: NULL in case of error or the Entity* otherwise.
*/
Entity* entity_setDialog(Entity* e, int dialogid);


/*
    Given an Entity* it returns the id associated to that entity or -1 in case
    of errors.
*/
int entity_getId(Entity* e);


/*
    Given an entity and a inventory*, this function sets the given inventory to the
    entity.
    Returns: NULL if there has been any problem or the Entity* in case
    everything goes as expected.
*/
Entity* entity_setInventory(Entity* e, Inventory* inv);

/*
    Given an entity, this function returns a boolean telling us if this entity
    has dialogs.
*/
bool entity_getHasDialog(Entity *e);

/*
    Given a entity, it uses all its parametes to use the function room_processAlly,
    which modifies the room when an ally has been added.
    Returns: NULL if there has been any problem or the Entity* in case
    everything goes as expected.
*/
Entity* entity_processAlly(Entity* e);

/*
    Given a entity, it uses all its parametes to use the function room_processEnemy,
    which adds an enemy to the room.
    Returns: NULL if there has been any problem or the Entity* in case
    everything goes as expected.
*/
Entity* entity_processEnemy(Entity* e);

/*
    Given an entity, gets the DialogMan associated to it.
    Returns: NULL in case of error or a DialogMan* to the entity's dialog.
*/
DialogMan* entity_getDialogs(Entity *e);

/*
    Given an entity and a quest this function adds a copy of the given quest
    the entity.
    Returns: NULL in case of error or the Entity* if everything worked fine.
*/
Entity* entity_addQuest(Entity* en, Quest* q);

/*
    Given an Entity* and the address of an integer the function stores in the
    integer the number of quests and returns an array of Quest* to all the quests
    associated with the entity and or NULL if there has been some error.
*/
Quest** entity_getQuests(Entity* e, int* siz);

/*
    Given to entities, this function copies the inventory and all the Attributes
    of the 'new' entity into the old one ('prev').
    Returns: NULL in case of error and the modified entity ('prev') in case
    everything works fine.
*/
Entity* entity_modPlayer(Entity* prev, Entity* new);

/*
    Given an entity and a string, finds out if there is a fulfilled quest with
    the string's name, and in that case erases it form the entity and returns it.
    Returns NULL if there has been some error.
*/
Quest* entity_fetchFulfilledQuest(Entity* e,char*n);

/*
    Given an entity and a int address (where the number of new completed quests is
    going to be stored) an array of Quest pointers is returned with the recently
    completed quests. Returns NULL in case of error.
*/
Quest** entity_questJustCompleted(Entity* e, int* size);
#endif
