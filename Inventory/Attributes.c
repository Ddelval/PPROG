/*File creator : Jaime Pons Garrido*/


#include "Attributes.h"


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
Attributes* attb_ini(){
	Attributes* a = (Attributes*)calloc(1,sizeof(Attributes));
	if(!a) return NULL;
	for(int i=0; i<ATTRIBUTE_SIZE;i++) {
		if(!attb_set(a, 0, i)) {
			attb_free(a);
			return NULL;
		}
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
	if(!attb) return;
	free(attb);
}

int attb_get(Attributes* attb, attb_type index) {
	if(!attb||index<0||index>=ATTRIBUTE_SIZE) return -1;
	return attb->data[index];
}

Attributes* attb_set(Attributes* attb, int p, attb_type index) {
	if(!attb||index<0||index>=ATTRIBUTE_SIZE) return NULL;
	attb->data[index] = p;
	return attb;
}

int* attb_getAll(Attributes* attb) {
	if(!attb) return NULL;
	int* a = (int*)calloc(ATTRIBUTE_SIZE,sizeof(int));
	if(!a) return NULL;
	if(!memcpy(a, attb->data, ATTRIBUTE_SIZE*sizeof(int))) {
		free(a);
		return NULL;
	}
	return a;
}

Attributes* attb_setAll(Attributes* attb, int* p){
	if(!attb||!p) return NULL;
	for(int i=0; i<ATTRIBUTE_SIZE; i++) {
		attb->data[i] = p[i];
	}
	return attb;
}


/*
   Function name: attb_merge
   Utility: Simply adds the values of two attb structures. The primary one is the one which will be returned.
   Inputs: two attb pointers
   Outputs: the primary attb pointer. Usually the one attached to the entity.
 */
Attributes* attb_merge(Attributes* primary, Attributes* secondary) {
	Attributes* a = attb_ini();
	if(!a||!primary||!secondary) return NULL;
	for(int i=0;i<ATTRIBUTE_SIZE;i++)
		if(!attb_set(a, attb_get(secondary, i) + attb_get(primary, i), i)) return NULL;
	
	return a;
}

Attributes* attb_copy(Attributes* a) {
	Attributes* b = attb_ini();
	if(!a||!b) return NULL;
	for(int i=0;i<ATTRIBUTE_SIZE;i++) if(!attb_set(b, attb_get(a, i), i)) return NULL;

	return b;
}



Attributes* attb_load(FILE* f){
	if(!f) return NULL;
	Attributes* a = attb_ini();
	if(!a) return NULL;
	for(int i=0;i<ATTRIBUTE_SIZE;i++) fscanf(f, "%d", &a->data[i]);

	return a;
}



/*
   NO USAR!!!
   Function name: attb_print
   Utility: it prints the attribute values in a string format into a file.
   Inputs: FILE pointer, attb pointer
   Outputs: int that contains the number of characters that have been printed
 */
/* int attb_print(FILE *pf, Attributes* attb){
        int i = 0;
        if(!attb || !pf) return -1;

        i = fprintf(pf, "[Health: %d,Attack: %d,Defense: %d,Speed: %d,Agility: %d]", attb_get(attb, 1),attb_get(attb, 2),attb_get(attb, 3),attb_get(attb, 4),attb_get(attb, 5));

        if (ferror(pf)||i<0) return -1;

        return i;
} */
