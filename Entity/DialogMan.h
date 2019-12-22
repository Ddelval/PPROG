/*  DialogMan.h */

#ifndef DIALOGMAN.H
#define DIALOGMAN.H

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
DialogMan* ddic_ini(FILE* df);

/*
    Frees a DialogMan.
*/
void ddic_free(DialogMan* ddic);

/*
    Returns a copy of the given DialogMan.
*/
DialogMan* ddic_copy(DialogMan* ddic);

/*
    Searches for a Dialog with matching ID in the DialogMan. Returns it if found
    and otherwise gives back NULL;
*/
Dialog* ddic_lookup(DialogMan* ddic, int did);

/*
    Resets a given dialog to its beggining state.
*/
DialogMan* ddic_resetDialog(DialogMan* dd);


char* ddic_getLine(DialogMan* dic);
DialogMan* ddic_setDialog(DialogMan* ddic, int did);
DialogMan* ddic_advance(DialogMan* dd);
#endif /* DIALOGMAN.H */
