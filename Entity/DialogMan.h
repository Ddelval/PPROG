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
DialogMan* dman_ini(FILE* df, const Canvas* ent_pic);

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
/*
    Returns the a char pointer containing the string of the next line of the
    dialog passed as input.
*/
char* dman_getLine(DialogMan* dic);
/*
    Sets the current state of the dialog man to the dialog whose id is the
    one passed as input.
*/
DialogMan* dman_setDialog(DialogMan* dman, int did);
/*
    Sets the dialog man to the next dialog in the conversation. If it is the
    last dialog, returns NULL.
*/
DialogMan* dman_advance(DialogMan* dd);
/*
    Gets the quest of the cureent dialog in the DialogMan.
*/
Quest* dman_fetchQuest(DialogMan* dd);
/*
    Sets the dialog man to the next dialog in the conversation. Increments the
    global variable tier.
*/
void dman_autoAdvance(DialogMan* dd);
#endif /* DIALOGMAN_H */
