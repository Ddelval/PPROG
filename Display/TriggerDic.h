#ifndef TRIGGERDIC_H
#define TRIGGERDIC_H
#include "Trigger.h"



void trdic_free();
Trigger* trdic_lookup(int id);
int trdic_insert(Trigger* t);
#endif