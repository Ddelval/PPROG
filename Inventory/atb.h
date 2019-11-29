
/*
 * File:   atb.h
 * Author: jaime
 *
 * Created on 30 de septiembre de 2019, 9:48
 */


#ifndef ATB_H
#define ATB_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _atb atb;

#include "types.h"

/*
 Function name: atb_ini
 Utility: It creates a new atb structure with 0 in each camp
 Inputs:
 Outputs: atb pointer
 */
atb * atb_ini();
/*
 Function name: atb_destroy
 Utility: It destroys an atb structure
 Inputs: atb pointer
 Outputs:
 */
void atb_destroy(atb * atb);
/*
 Function name: atb_setter
 Utility: This function can be used to set an atb camp to an int value
 * GUIDE:
 * 1 = HEALTH
 * 2 = ATTACK
 * 3 = DEFENSE
 * 4 = SPEED
 * 5 = AGILITY
 Inputs: atb pointer, int value (the value t set), int id (the camp you will choose to edit)
 Outputs: Status (OK/ERROR)
 */
Status atb_setter(atb * atb, int value, int id);
/*
 Function name: atb_getter
 Utility: Allows you to obtain the value of a certain atb camp you will bee able to choose
 *  * GUIDE:
 * 1 = HEALTH
 * 2 = ATTACK
 * 3 = DEFENSE
 * 4 = SPEED
 * 5 = AGILITY
 Inputs: atb pointer, int id
 Outputs: int value
 */
int atb_getter(atb * atb, int id);
/*
 Function name: atb_merge
 Utility: Simply adds the values of two atb structures. The primary one is the one which will be returned.
 Inputs: two atb pointers
 Outputs: the primary atb pointer. Usually the one attached to the entity.
 */
atb * atb_merge(atb * primary, atb * secondary);
/*
 Function name: atb_print
 Utility: it prints the attribute values in a string format into a file.
 Inputs: FILE pointer, atb pointer
 Outputs: int that contains the number of characters that have been printed
 */
int atb_print(FILE *pf, atb * atb);

atb *atb_allCopy(atb *a);






#endif /* ATB_H */
