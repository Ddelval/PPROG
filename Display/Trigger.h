//  PPROG
//	Trigger.c
//  Created by David del Val on 09/12/2019
//
//
#ifndef TRIGGER_H
#define TRIGGER_H

#include "Utility.h"
typedef struct _Trigger Trigger;
typedef enum{OBTAIN, ENTER,TALK,SHOW} trig_type;
typedef void (*func_trig)(Trigger* t, void* en, void* r);

Trigger* tr_ini();
void tr_free(Trigger* tr);
Trigger* tr_load(FILE* f);
int tr_getID(const Trigger* tr);
Trigger* tr_copy(const Trigger* t);
Trigger* tr_setSpr(Trigger* tr, int i);
trig_type tr_getType(const Trigger* tr);
int tr_getId(const Trigger* tr);
trig_type tr_getType(const Trigger* tr);

int tr_getObj_id(const Trigger* tr);
int tr_getSpr_index(const Trigger* tr);
bool tr_getSpr_remove(const Trigger* tr);
int tr_getQuantity(const Trigger* tr);

bool tr_needsTrigger(trig_type t);
void tr_setId(Trigger * t, int id);
Trigger* tr_createTalk(void* e,int ally_id);
void* tr_getEntityRef(Trigger* t);
#endif