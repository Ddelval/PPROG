/*  DialogMan.h */

#ifndef DIALOGMAN_H
#define DIALOGMAN_H

#include "Dialog.h"
#include <stdlib.h>
#include <stdio.h>


/*
    DialogMan is not a dictionary per se, but a collection of all the dialogs an
    entity can have (should we rename it to DialogCatalog?)
*/
typedef struct _DialogMan DialogMan;

/*
    Initializes a new DialogMan from a file.
*/
DialogMan* dman_ini(FILE* df, Canvas* ent_pic);

/*
    Frees a DialogMan.
*/
void dman_free(DialogMan* dman);

/*
    Returns a copy of the given DialogMan.
*/
DialogMan* dman_copy(DialogMan* dman);

/*
    Searches for a Dialog with matching ID in the DialogMan. Returns it if found
    and otherwise gives back NULL;
*/
Dialog* dman_lookup(DialogMan* dman, int did);

/*
    Resets a given dialog to its beggining state.
*/
DialogMan* dman_resetDialog(DialogMan* dd);


char* dman_getLine(DialogMan* dic);
DialogMan* dman_setDialog(DialogMan* dman, int did);
DialogMan* dman_advance(DialogMan* dd);
Quest* dman_fetchQuest(DialogMan* dd);
#endif /* DIALOGMAN_H */
