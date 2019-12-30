
#ifndef TRIGFUNC_H
#define TRIGFUNC_H

#include "Trigger.h"
#include "Utility.h"
#include "Entity.h"
#include "FontCatalog.h"

void trig_give(Trigger* t, void* e, void* r);
void trig_showInv(Trigger* t, void* e,void* d);
void trig_showRec(Trigger* t,void *e, void* d);
void trig_talk(Trigger* t,void* e, void* d);
void trig_showQuest(Trigger* t, void *e, void* d);
#endif