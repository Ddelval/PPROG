
#ifndef TRIGGER_H
#define TRIGGER_H

#include "Utility.h"
typedef struct _Trigger Trigger;
typedef enum{ALLOW, CAUSE} trig_type;
typedef void (*func_trig)(Trigger* t, Entity* en);

Trigger* tr_ini();
void tr_free(Trigger* tr);
Trigger* tr_load(FILE* f);
int tr_getID(const Trigger* tr);
Trigger* tr_copy(const Trigger* t);
Trigger* tr_setSpr(Trigger* tr, int i);
trig_type tr_getType(const Trigger* tr);
#endif