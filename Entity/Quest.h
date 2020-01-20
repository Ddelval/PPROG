#ifndef QUEST_H
#define QUEST_H

#include "Utility.h"
#include "Canvas.h"
#include "Welem.h"
#include "ObjectDic.h"
#include "FontCatalog.h"
typedef struct _Quest Quest;

/**
 * @brief Creates a quest.
 * @param
 * @return  New empty quest or NULL in case of error.
**/
Quest* quest_ini();
/**
 * @brief Frees all the memory occupied by a quest
 * @param q: The quest to be freed
 * @return
**/
void quest_free(Quest*q);
/**
 * @brief Reads the quest from a file
 *
 * The format is the following:
 * Title
 * Description
 * obj_to_get_id
 *
 * @param f File from which the data will be read
 * @return  New quest or NULL in case of error
**/
Quest* quest_load(FILE* f, const Canvas* ent);

/**
 * @brief Stores a description of the quest in a file
 * 
 * This description can later be read by the load function
 * 
 * @param q  Quest to be stored
 * @param f  File in which it will be stored
 * @return   NULL if error 
 */
Quest* quest_storeinFile(Quest* q, FILE* f);

/**
 * @brief Copies a quest.
 * @param src: The quest to be copied.
 * @return  New quest with the information of the given one or NULL in case of error.
**/
Quest* quest_copy(const Quest* src);
/**
 * @brief Marks a quest as completed or not.
 * @param q: Quest to be modified.
 * @param completed: Boolean to be assigned to the completed field.
 * @return
**/
void quest_setCompleted(Quest* q, bool completed);
/**
 * @brief Renders a quest.
 * @param src: The quest to be rendered.
 * @param wid: Maximum width of the quest to be rendered.
 * @return NULL in case of error or a canvas * to the rendered quest.
**/
Canvas* quest_render(Quest* src, int wid);
/**
 * @brief Sets an assigner to a quest.
 * @param q: Quest to be modified.
 * @param a: String to be set to the assigner.
 * @return
**/
void quest_setAsigner(Quest* q, char* a);
/**
 * @brief Gets the assigner from the quest.
 * @param q: Quest for the assigner to be gotten.
 * @return: NULL in case of error or a char * with a copy of the assigner as string.
**/
char* quest_getAsigner(Quest* q);
/**
 * @brief Tells us if the quest has already been completed or not.
 * @param q: Quest to be evaluated.
 * @return: True or false depending if the quest has already been completed or not.
**/
bool quest_getCompleted(Quest* q);
/**
 * @brief Gets the requirements from the quest.
 * @param q: Quest for the requirements to be gotten.
 * @param len: Address of an int where the number of requirements will be saved.
 * @return: NULL in case of error or an array of pairii with a copy of the requirements.
**/
pairii* quest_getRequirements(Quest* q, int* len);
#endif
