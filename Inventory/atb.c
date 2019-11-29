/*File creator : Jaime Pons Garrido*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "atb.h"
#include "types.h"

extern int errno;

struct _atb {
    int health;
    int attack;
    int defense;
    int speed;
    int agility;
};
/*
 Function name: atb_ini
 Utility: It creates a new atb structure with 0 in each camp
 Inputs:
 Outputs: atb pointer
 */
atb * atb_ini() {
    atb * a = NULL;
    int aux = 1;
    a = (atb *) calloc(1,sizeof (atb));
    if (a == NULL) {
        printf("Error: calloc.\n");
        return NULL;
    }

    for (aux; aux <= 5; aux++) {
        atb_setter(a, 0, aux);
    }

    return a;
}
/*
 Function name: atb_destroy
 Utility: It destroys an atb structure
 Inputs: atb pointer
 Outputs:
 */
void atb_destroy(atb* atb) {
       if (!atb) {
        fprintf(stderr, "%s\n", strerror(errno));
        return ;
    }
    free(atb);
}
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
Status atb_setter(atb * atb, int value, int id) {
    if (atb == NULL) {
        return ERROR;
    }
    if (id < 1 || id > 5) return ERROR;

    if (id == 1) {
        atb->health = value;
    }
    else if (id == 2) {
        atb->attack = value;
    } else if (id == 3) {
        atb->defense = value;
    } else if (id == 4) {
        atb->speed = value;
    } else if (id == 5) {
        atb->agility = value;
    }

    return OK;
}
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
int atb_getter(atb * atb, int id) {
    if (atb == NULL) return ERROR;
    if (id < 1 || id > 5) return ERROR;
    if (id == 1) {
        return atb->health;
    }
    else if (id == 2) {
        return atb->attack;
    }
    else if (id == 3) {
        return atb->defense;
    }
    else if (id == 4) {
        return atb->speed;
    }
    else if (id == 5) {
        return atb->agility;
    }

    return OK;
}
/*
 Function name: atb_merge
 Utility: Simply adds the values of two atb structures. The primary one is the one which will be returned.
 Inputs: two atb pointers
 Outputs: the primary atb pointer. Usually the one attached to the entity.
 */
atb * atb_merge(atb * primary, atb * secondary){
    primary->health += secondary->health;
    primary->attack += secondary->attack;
    primary->defense += secondary->defense;
    primary->speed += secondary->speed;
    primary->agility += secondary->agility;

    return primary;
}
/*
 Function name: atb_print
 Utility: it prints the attribute values in a string format into a file.
 Inputs: FILE pointer, atb pointer
 Outputs: int that contains the number of characters that have been printed
 */
int atb_print(FILE *pf, atb * atb){

    int i = 0;
         if (!atb || !pf) {
     fprintf(stderr, "%s", strerror(errno));
    return -1;
 }

    i = fprintf(pf, "[Health: %d,Attack: %d,Defense: %d,Speed: %d,Agility: %d]", atb_getter(atb, 1),atb_getter(atb, 2),atb_getter(atb, 3),atb_getter(atb, 4),atb_getter(atb, 5));

    if (ferror(pf) || i < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -1;
    }

    return i;
}

atb *atb_allCopy(atb *a){
  atb * cpy = NULL;

  if(!a) return NULL;

  cpy->health = a->health;
  cpy->attack = a->attack;
  cpy->defense = a->defense;
  cpy->speed = a->speed;
  cpy->agility = a->agility;

  return cpy;
}
