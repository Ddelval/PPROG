#ifndef QUEST_H
#define QUEST_H

#include "Utility.h"
#include "Canvas.h"
#include "Welem.h"
#include "ObjectDic.h"
#include "FontCatalog.h"
typedef struct _Quest Quest;

Quest* quest_ini();
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
 * @return  New quest 
 */
Quest* quest_load(FILE* f,Canvas* ent);
Quest* quest_copy(const Quest* src);
void quest_setCompleted(Quest* q, bool completed);
Canvas* quest_render(Quest* src, int wid);
void quest_setAsigner(Quest* q, char* a);
char* quest_getAsigner(Quest* q);
bool quest_getFulfilled(Quest* q);
pairii* quest_getRequirements(Quest* q, int* len);
#endif