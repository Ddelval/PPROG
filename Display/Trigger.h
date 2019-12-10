
#ifndef TRIGGER_H
#define TRIGGER_H

#include "Utility.h"
typedef struct _Trigger Trigger;
typedef enum{OBTAIN, ENTER,TALK} trig_type;
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

void trig_give(Trigger* t, void* e, void* r);
#endif