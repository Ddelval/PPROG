/**
 * @file Trigger.h
 * @author David del Val 
 * @date 2019-12-09
 * @brief  Module with the information about the actions that
 *         the player can do in a particular position
 * 
 * Trigger can be divided into 5 types
 * 
 * Obtain triggers will be attached to sprites that will
 * give items to the player. In those, we are storing:
 * - object that will be given
 * - sprite index of the sprite that gives the object
 * - quantity of elements given
 * - tier that the game has to be in at least for this trigger
 *   to be unlocked
 * - whether or not the sprite in the room should be removed
 *   after the item has been collected
 * 
 * Enter triggers are attached to doors or sprites that give
 * access to other locations. Those can be completely different
 * worlds or just a cave in the same world. In this case we are
 * storing:
 * - name of the room that we are entering
 * - tier that is required for this trigger to be available
 * 
 * Talk trigger are used to initiate a dialog with another entity.
 * For these we are storing:
 * - entity_id which is the index of the ally in the array of the 
 *   over sprites in the room
 * - pointer to the entity itself in the array of allies in the 
 *   world
 * - world in which the ally is
 * 
 * Show trigger are used to start a window
 * 
 * Combat triggers start a combat and contain the same information as 
 * the dialog ones
 * 
 */
#ifndef TRIGGER_H
#define TRIGGER_H

#include "Utility.h"
typedef struct _Trigger Trigger;
typedef enum{OBTAIN, ENTER,TALK,SHOW, COMBAT} tr_type;

/**
 * @brief Function that will be called with the trigger
 * 
 * This allows us to store the function that will take
 * the triggers in each element of the UI
 * 
 */
typedef void (*func_trig)(Trigger* t, void* en, void* r);

/*-----------------------------------------------------------------*/
/**
 * @brief Allocates the memory for the new trigger
 * 
 * @return The new trigger
 */
Trigger* tr_ini();

/*-----------------------------------------------------------------*/
/**
 * @brief Frees the memory taken up by the trigger
 * 
 * @param tr Trigger to be freed
 */
void tr_free(Trigger* tr);

/**
 * @brief Loads the trigger from a file
 * Depending on the type, the format will change
 * 
 * OBTAIN
 * <trigger_id> 0
 * <trigger_name>
 * <tier>
 * <object_id> <quantity>
 * <sprite removed or not>
 * 
 * ENTER
 * <trigger_id> 1
 * <trigger_name>
 * <tier>
 * <name of the next room>
 * 
 * TALK
 * <trigger_id> 1
 * <trigger_name>
 * <tier>
 * <position of entity in overs>
 * 
 * 
 * @param f 
 * @return Trigger* 
 */
Trigger* tr_load(FILE* f);

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new trigger that represents a combat
 * 
 * @param e         Pointer to the enemy in the world
 * @param enemy_id  Position of the enemy sprite in the array overs
 * @return          New trigger
 */
Trigger* tr_createAttack(void*e, int enemy_id);

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new trigger that represents a conversation
 * 
 * @param e         Pointer to the ally in the world
 * @param ally_id   Position of the ally sprite in the array overs
 * @return          New trigger
 */
Trigger* tr_createTalk(void* e,int ally_id);

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a copy of the trigger
 *  
 * @param t Trigger to be copied 
 * @return  New trigger with all the information
 */
Trigger* tr_copy(const Trigger* t);

/*-----------------------------------------------------------------*/
//Checks whether or not a Trigger of a type will need additional 
//information that will dpend on the position of the player on the room
bool tr_needsTrigger(tr_type t);
/*-----------------------------------------------------------------*/
//Checks if two triggers contain the same information
bool tr_completeEqual(Trigger* t1,Trigger* t2);

/** SETTERS **/
void tr_setSpr   (Trigger* tr, int i);
void tr_setId    (Trigger* tr, int id);
void tr_setWorld (Trigger* tr, char* wor);

/** GETTERS **/
int     tr_getId        (const Trigger* tr);
tr_type tr_getType      (const Trigger* tr);
int     tr_getObj_id    (const Trigger* tr);
int     tr_getSpr_index (const Trigger* tr);
bool    tr_getSpr_remove(const Trigger* tr);
int     tr_getQuantity  (const Trigger* tr);
char*   tr_getNWorld    (const Trigger* tr);
void*   tr_getEntityRef (const Trigger* tr);
char*   tr_getDesc      (const Trigger* tr);
int     tr_getAlly_id   (const Trigger* tr);
char*   tr_getWorld     (const Trigger* tr);
int     tr_getTier      (const Trigger* tr);

#endif