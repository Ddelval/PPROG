/**
 * @file TrigReact.h
 * @author David del Val 
 * @date 2019-12-10
 * 
 * @brief Functions to process triggers
 * 
 */
#ifndef TRIGFUNC_H
#define TRIGFUNC_H

#include "Trigger.h"
#include "Utility.h"
#include "Entity.h"
#include "FontCatalog.h"

/*-----------------------------------------------------------------*/
/**
 * @brief Processes a trigger of the type OBTAIN
 * 
 * It also checks for the completion of quests
 * 
 * @param t Trigger to be processed
 * @param e Player
 * @param d Display
 */
void trig_give(Trigger* t, void* e, void* d);

/*-----------------------------------------------------------------*/
/**
 * @brief Shows the inventory
 * 
 * @param t Irrelevant
 * @param e Player
 * @param d Display
 */
void trig_showInv(Trigger* t, void* e,void* d);

/*-----------------------------------------------------------------*/
/**
 * @brief Shows the crafting window
 * 
 * @param t Irrelevant
 * @param e Player
 * @param d Display
 */
void trig_showRec(Trigger* t,void *e, void* d);

/*-----------------------------------------------------------------*/
/**
 * @brief Shows the quest window
 * 
 * @param t Irrelevant
 * @param e Player
 * @param d Display
 */
void trig_showQuest(Trigger* t, void *e, void* d);

/*-----------------------------------------------------------------*/
/**
 * @brief Starts the dialog with the entity
 * 
 * It also deletes completed quests related to the entity with 
 * which the dialog will start and increases the tier value
 * 
 * @param t Trigger that contains the information about the ally
 * @param e Player
 * @param d Display
 */
void trig_talk(Trigger* t,void* e, void* d);

/*-----------------------------------------------------------------*/
/**
 * @brief Enters a new room
 * 
 * @param t Trigger that contains the information about the next room
 * @param e Player
 * @param d Display
 */
void trig_enter(Trigger* t, void* e, void* d);

#endif