/*File creator : Jaime Pons Garrido*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Attributes.h"
#include "types.h"
#include "errno.h"
extern int errno;



#define ATTRIBUTE_SIZE 5
struct _Attributes {
        int data[ATTRIBUTE_SIZE];
};

/*
   Function name: attb_ini
   Utility: It creates a new attb structure with 0 in each camp
   Inputs:
   Outputs: attb pointer
 */
Attributes * attb_ini(){
        Attributes * a = NULL;
        int aux = 1;
        a = (Attributes *) calloc(1,sizeof (Attributes));
        if (a == NULL) {
                printf("Error: calloc.\n");
                return NULL;
        }

        for (aux = 0; aux <= ATTRIBUTE_SIZE; aux++) {
                attb_set(a, 0, aux);
        }

        return a;
}
/*
   Function name: attb_destroy
   Utility: It destroys an attb structure
   Inputs: attb pointer
   Outputs:
 */
void attb_free(Attributes* attb) {
        if (!attb) {
                fprintf(stderr, "%s\n", strerror(errno));
                return;
        }
        free(attb);
}
/*
   Function name: attb_setter
   Utility: This function can be used to set an attb camp to an int value
 * GUIDE:
 * 1 = HEALTH
 * 2 = ATTACK
 * 3 = DEFENSE
 * 4 = SPEED
 * 5 = AGILITY
   Inputs: attb pointer, int value (the value t set), int id (the camp you will choose to edit)
   Outputs: Status (OK/ERROR)
 */



int attb_get(Attributes* attb, attb_type index){
        if (attb == NULL || index < 0) return -1;

        return attb->data[index];
}

Status attb_set(Attributes* attb, int p, attb_type index){
        if(!attb || index < 0) return ERROR;

        attb->data[index] = p;

        return OK;
}

int * attb_getAll(Attributes * attb){
        if(!attb) return NULL;

        int * a;
        
        a = (int*)calloc(ATTRIBUTE_SIZE,sizeof(int));
        
        memcpy(a, attb->data, ATTRIBUTE_SIZE * sizeof(int));
        
        return a;
}

Status attb_setAll(Attributes * attb, int *p){
        if (attb == NULL || !p) return ERROR;

        for (int i = 0; i < ATTRIBUTE_SIZE; i++) {
                attb->data[i] = p[i];
        }

        return OK;
}


/*
   Function name: attb_merge
   Utility: Simply adds the values of two attb structures. The primary one is the one which will be returned.
   Inputs: two attb pointers
   Outputs: the primary attb pointer. Usually the one attached to the entity.
 */
Attributes * attb_merge(Attributes * primary, Attributes * secondary){
        int i = 0;
        Attributes * a = attb_ini();
        if(!a || !primary || !secondary) return NULL;

        while(i < ATTRIBUTE_SIZE) {
                if(attb_set(a, attb_get(secondary, i) + attb_get(primary, i), i) == ERROR) return NULL;
                i++;
        }
        return a;
}

Attributes *attb_copy(Attributes *a){
        int i = 0;
        Attributes * b = attb_ini();
        if(!a || !b) return NULL;
        while(i < ATTRIBUTE_SIZE) {
                if(attb_set(b, attb_get(a, i), i) == ERROR) return NULL;
                i++;
        }
        return b;
}



Attributes* attb_load(FILE* f){
        int i = 0;
        if(!f) return NULL;
        Attributes * a = attb_ini();
        if(!a) return NULL;

        while(i < ATTRIBUTE_SIZE) {
                fscanf(f, "%d", &a->data[i]);
                i++;
        }

        return a;
}



/*
   NO USAR!!!
   Function name: attb_print
   Utility: it prints the attribute values in a string format into a file.
   Inputs: FILE pointer, attb pointer
   Outputs: int that contains the number of characters that have been printed
 */
int attb_print(FILE *pf, Attributes * attb){

        int i = 0;
        if (!attb || !pf) {
                fprintf(stderr, "%s", strerror(errno));
                return -1;
        }

        i = fprintf(pf, "[Health: %d,Attack: %d,Defense: %d,Speed: %d,Agility: %d]", attb_get(attb, 1),attb_get(attb, 2),attb_get(attb, 3),attb_get(attb, 4),attb_get(attb, 5));

        if (ferror(pf) || i < 0) {
                fprintf(stderr, "%s\n", strerror(errno));
                return -1;
        }

        return i;
}
