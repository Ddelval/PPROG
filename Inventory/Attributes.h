
/*
 * File:   attb.h
 * Author: jaime
 *
 * Created on 30 de septiembre de 2019, 9:48
 */


#ifndef atb_H
#define atb_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _Attributes Attributes;
typedef enum {HEALTH,ATTACK,DEFENSE,SPEED,AGILITY} attb_type;



/*
   Function name: attb_ini
   Utility: It creates a new attb structure with 0 in each camp
   Inputs:
   Outputs: attb pointer
 */
Attributes * attb_ini();
/*
   Function name: attb_destroy
   Utility: It destroys an attb structure
   Inputs: attb pointer
   Outputs:
 */
void attb_free(Attributes * attb);

Attributes* attb_set(Attributes* attb, int p, attb_type index);

int attb_get(Attributes* attb, attb_type index);

int * attb_getAll(Attributes * attb);

Attributes* attb_setAll(Attributes * attb, int *p);



Attributes* attb_load(FILE* f);


/*
   Function name: attb_merge
   Utility: Simply adds the values of two attb structures. The primary one is the one which will be returned.
   Inputs: two attb pointers
   Outputs: the primary attb pointer. Usually the one attached to the entity.
 */
Attributes * attb_merge(Attributes * primary, Attributes * secondary);
/*
   Function name: attb_print
   Utility: it prints the attribute values in a string format into a file.
   Inputs: FILE pointer, attb pointer
   Outputs: int that contains the number of characters that have been printed
 */

Attributes *attb_copy(Attributes *a);






#endif /* attb_H */
