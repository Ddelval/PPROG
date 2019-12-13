/*
   File:   Crafting.c
   Author: Pedro Urbina
   Created on: 6 december 2019
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Crafting.h"

#define NAME_LENGTH 64

struct _Recipe {
        int * quantities;
        int * elements;
        int result_id;
        int size;
        char name[NAME_LENGTH];
};


Recipe * rec_ini(){
        Recipe *r = (Recipe *) calloc (1, sizeof(Recipe));
        if(!r) return NULL;

        r->quantities = NULL;
        r->elements = NULL;
        r->result_id = -1;
        r->size = 0;

        return r;
}

void rec_free(Recipe *r){
        if(!r) return;
        if(r->quantities) free(r->quantities);
        if(r->elements) free(r->elements);
        free(r);
        return;
}

Recipe * rec_copy(Recipe *r){
        if(!r) return NULL;
        Recipe * c = rec_ini();
        if(!c) return NULL;

        int *q = (int *) calloc (sizeof(int), r->size);
        int *e = (int *) calloc (sizeof(int), r->size);

        c->size = r->size;
        c->result_id = r->result_id;
        strcpy(c->name, r->name);

        for(int i = 0; i < c->size; i++) {
                q[i] = r->quantities[i];
                e[i] = r->elements[i];
        }
        c->elements = e;
        c->quantities = q;

        return c;
}


/*
   Creates the array of objects and stores it in *obj. Also copies the array of
   quantities into *quant.
 */
Recipe* rec_getData(Recipe * r, Object *** obj, int ** quant){
        if(!r) return NULL;

        if(r->size == 0) return r;

        (*obj) = (Object **) calloc (sizeof(Object *), r->size);
        if(!(*obj)) return NULL;
        (*quant) = (int *) calloc (sizeof(int), r->size);
        if(!(*quant)) return NULL;

        int i = 0;
        while(i < r->size) {
                (*obj)[i] = odic_lookup(r->elements[i]);
                (*quant)[i] = r->quantities[i];
                i++;
        }
        return r;
}

//Following format:  (size) (name) (result_id) (elements_ids) (list_of_quantites)
Recipe* rec_load(FILE *f){
        Recipe *r = rec_ini();
        int i = 0;
        if(!f || !r) return NULL;

        fscanf(f, "%d\n", &(r->result_id));
        fscanf(f, "%d\n", &(r->size));
        fgets(r->name, NAME_LENGTH ,f);

        r->quantities = (int *) calloc (sizeof(int), r->size);
        r->elements = (int *) calloc (sizeof(int), r->size);


        while(i < r->size) {
                fscanf(f, "%d", &(r->elements[i]));
                i++;
        }
        i = 0;
        while(i < r->size) {
                fscanf(f, "%d", &(r->quantities[i]));
                i++;
        }

        return r;
}

/*
   Checks if this recipe can be prepared with the objects present in the inventory
   Note that it might be a great idea to add the following function to inventory:
   int inv_getQuantity(Inventory* inv, int obj_id)that returns -1 if the element
   is not present or the quantiy that the inventory has of it otherwise.
 */
bool rec_doable(Inventory* inv, Recipe* r){
        int i = 0;
        if(!inv || !r) return false;

        while(i < r->size) {
                if(inv_getQuantity(inv, r->elements[i]) < r->quantities[i]) {
                        return false;
                }
                i++;
        }

        return true;
}


int * rec_getQuantities(Recipe * r){
  if(!r) return NULL;
  return r->quantities;
}
int * rec_getElements(Recipe * r){
  if(!r) return NULL;
  return r->elements;
}
int rec_getResult_id(Recipe * r){
  if(!r) return -1;
  return r->result_id;
}
int rec_getSize(Recipe * r){
  if(!r) return -1;
  return r->size;
}
char *rec_getName(Recipe * r){
  if(!r) return NULL;
  return r->name;
}

