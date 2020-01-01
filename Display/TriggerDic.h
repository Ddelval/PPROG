#ifndef TRIGGERDIC_H
#define TRIGGERDIC_H
#include "Trigger.h"



void trdic_free();
Trigger* trdic_lookup(int id);
int trdic_insert(Trigger* t);
int trdic_talksearch(int ally_id);
int trdic_attacksearch(int entity_id);
#endif