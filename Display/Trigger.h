
#ifndef TRIGGER_H
#define TRIGGER_H

#include "Utility.h"
typedef struct _Trigger Trigger;
typedef enum{ALLOW, CAUSE} trig_type;

Trigger* tr_ini();
void tr_free(Trigger* tr);
Trigger* tr_load(FILE* f);
Trigger* tr_copy(const Trigger* t);

#endif