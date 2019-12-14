/*  DialogDic.h */

#ifndef DIALOGDIC_H
#define DIALOGDIC_H

#include "Dialog.h"
#include <stdlib.h>
#include <stdio.h>


/*
    DialogDic is not a dictionary per se, but a collection of all the dialogs an
    entity can have (should we rename it to DialogCatalog?)
*/
typedef struct _DialogDic DialogDic;

/*
    Initializes a new DialogDic from a file.
*/
DialogDic* ddic_ini(FILE* df);

/*
    Frees a DialogDic.
*/
void ddic_free(DialogDic* ddic);

/*
    Returns a copy of the given DialogDic.
*/
Dialog* ddic_copy(DialogDic* ddic);

/*
    Searches for a Dialog with matching ID in the DialogDic. Returns it if found
    and otherwise gives back NULL;
*/
Dialog* ddic_lookup(DialogDic* ddic, int did);

#endif /* DIALOGDIC_H */
