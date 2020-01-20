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
	free(attb);
}

int attb_get(const Attributes* attb, attb_type index) {
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


void attb_mergeItself(Attributes * ori, const Attributes * adder){
	if(!ori||!adder) return;
	for(int i=0;i<ATTRIBUTE_SIZE;i++)
		if(!attb_set(ori, attb_get(ori, i) + attb_get(adder, i), i)) return;
	return;
}


Attributes* attb_load(FILE* f){
	if(!f) return NULL;
	Attributes* a = attb_ini();
	if(!a) return NULL;
	for(int i=0;i<ATTRIBUTE_SIZE;i++) fscanf(f, "%d", &a->data[i]);

	return a;
}

Attributes* attb_saveToFile(Attributes* a, FILE* f){
	if(!a||!f)return NULL;
	for(int i=0;i<ATTRIBUTE_SIZE;++i)fprintf(f,"%d ",a->data[i]);
	fprintf(f,"\n");
	return a;
}


