
#ifndef TRIGGER_H
#define TRIGGER

#include "Utility.h"
typedef struct _Trigger Trigger;
typedef enum{ALLOW, GIVE, DIALOG} trig_type;

Trigger* tr_ini();
Trigger* tr_free(Trigger* tr);
Trigger* tr_load(FILE* f);
Trigger* tr_copy(Trigger* t);

#endif