
/*
 * File:   attb.h
 * Author: jaime
 *
 * Created on 30 de septiembre de 2019, 9:48
 */


#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "Utility.h"

typedef struct _Attributes Attributes;
typedef enum {HEALTH,ATTACK,DEFENSE,SPEED,AGILITY} attb_type;
#define ATT_NUMBER 5
/*
   Function name: attb_ini
   Utility: It creates a new attb structure with 0 in each field.
   Inputs:
   Outputs: attb pointer or NULL in case of error.
 */
Attributes* attb_ini();
/*
   Function name: attb_destroy
   Utility: It destroys an attb structure
   Inputs: attb pointer to be freed
   Outputs:
*/
void attb_free(Attributes* attb);
/*
   Function name: attb_set
   Utility: It sets the attribute with the given index to the value p
   Inputs:  attb pointer to be modiffied
            int p: new value of the attribute
            attb_type index: the attribute which is going to be modified.
   Output: The modiffied attribute or NULL in case of error.
*/
Attributes* attb_set(Attributes* attb, int p, attb_type index);
/*
   Function name: attb_get
   Utility: Returns the value of the chosen attribute (with the index variable)
   Inputs:  attb pointer
            attb_type index: variable to select the wished item
   Output: The value of the chosen attribute or -1 in case of error.
*/
int attb_get(const Attributes* attb, attb_type index);
/*
   Function name: attb_getAll
   Utility: Returns an array of ints of all the values of the given attribute.
   Inputs: attb pointer
   Output: A dinamically allocated array of ints containing the attribute values
           or NULL in case of error.
*/
int * attb_getAll(Attributes* attb);
/*
   Function name: attb_setAll
   Utility: Sets all the values of a given attribute.
   Inputs:  attb pointer to be completely modified
            int * p: array of the new values of the attribute
   Output: The modified attribute or NULL in case of error.
*/
Attributes* attb_setAll(Attributes* attb, int *p);
/*
   Function name: attb_mergeItself
   Utility: Adds all the adder attribute values to the first attribute.
   Inputs:  ori: attb pointer to be modified
            adder: constant attb pointer to be added to the modiffied attribute
   Output:
*/
void attb_mergeItself(Attributes * ori, const Attributes * adder);
/*
   Function name: attb_load
   Utility: Creates an attribute and sets its values to the ones written in a file.
   Inputs:  FILE* f: pointer to an opened file where the values of the
                     attributes are written
   Output: An Attributes* with the values written in the file or NULL in case of error.
*/
Attributes* attb_load(FILE* f);


/*
   Function name: attb_merge
   Utility: Simply adds the values of two attb structures into a new third one.
   Inputs: two attb pointers
   Outputs: the new attb pointer with the sum of the values of the ones given as input.
 */
Attributes* attb_merge(Attributes* primary, Attributes* secondary);

/*
   Function name: attb_copy
   Utility: returns a new attribute with the same values of the given one.
   Inputs: attb pointer to the attribute which is going to be copied
   Outputs: a new initialized attribute pointer with the same values of
            the one passed as input.
*/
Attributes* attb_copy(Attributes* a);

#endif /* attb_H */
