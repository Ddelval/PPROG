/**
 * @file TriggerDic.c
 * @author David del Val 
 * @date 2019-10-20
 * 
 * @brief Contains all the triggers that are used
 *        in the game
 * 
 * This ADT will contain every trigger so that they
 * can be identified only with their id or the 
 * entity that they reference without having to 
 * keep several copies on the same trigger in 
 * different modules
 * 
 */


#ifndef TRIGGERDIC_H
#define TRIGGERDIC_H
#include "Trigger.h"

/*-----------------------------------------------------------------*/
/**
 * @brief Frees all the memory related to the trigger dictionary
 * 
 * This function has to be called before the game exits
 * 
 */
void trdic_free();


/*-----------------------------------------------------------------*/
/**
 * @brief Look for a trigger with a particular id
 * 
 * @param id  Id of the trigger we are looking for
 * @return    Copy of the trigger
 */
Trigger* trdic_lookup(int id);

/*-----------------------------------------------------------------*/
/**
 * @brief Inserts a new trigger on the dictionary
 * 
 * This function should not be used for triggers
 * that will already be attached to each sprite
 * before the game starts
 * 
 * It is intended to be used to insert triggers
 * related to the allies and the enemies present in 
 * each world
 * 
 * @param t 
 * @return int 
 */
int trdic_insert(Trigger* t);

/*-----------------------------------------------------------------*/
/**
 * @brief Search for the conversation trigger associated
 *        to this ally in this world
 * 
 * @param ally_id Position of the sprite of the ally in overs
 * @return        Id of the trigger
 */
int trdic_talksearch(int ally_id);

/*-----------------------------------------------------------------*/
/**
 * @brief Search for the combat trigger associated
 *        to this enemy in this world
 * 
 * @param entity_id Position of the sprite of the enemy in overs
 * @return          Id of the trigger
 */
int trdic_attacksearch(int entity_id);
#endif