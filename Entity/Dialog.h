/*  Dialog.h  */

#ifndef DIALOG_H
#define DIALOG_H

#include "Utility.h"

typedef struct _Dialog Dialog;

/*
    Initializes a new empty Dialog.
*/
Dialog* diag_ini();

/*
    Frees a given Dialog.
*/
void diag_free(Dialog* diag);

/*
    Returns a copy of the given Dialog.
*/
Dialog* diag_copy(Dialog* diag);

/*
    Gets the next line in the dialog, incrementing the line position.
*/
const char* diag_getNext(Dialog* diag);

/*
    Returns the ID of the given Dialog.
*/
int diag_getId(Dialog* diag);

/*
    Returns the number of lines in a given Dialog. The larger the number, the
    more you'll want the guy who is talking to shut up.
*/
int diag_getLength(Dialog* diag);

/*
    Gets the current line in a given Dialog.
*/
int diag_getCurrentLine(Dialog* diag);

/*
    Jumps to a given line in a Dialog.
*/
Dialog* diag_setLine(Dialog* diag, int newline);

/*
    Jumps 'lines' lines (forwards or backwards) in the given Dialog.
*/
Dialog* diag_jumpLines(Dialog* diag, int lines);

/*
    Loads a dialog from file.
*/
Dialog* diag_load(FILE* f);


#endif /* DIALOG_H */
