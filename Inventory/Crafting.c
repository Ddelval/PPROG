/*
   File:   Crafting.c
   Author: Pedro Urbina
   Created on: 6 december 2019
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Crafting.h"
#include "types.h"
#define NAME_LENGTH 50

struct _Recipe {
        int * quantities;
        int * elements;
        int result_id;
        int size;
        char name[NAME_LENGTH]; //It may have several words
};

typedef struct {
        Recipe** rec;
        int size;
} RecipeDic;

RecipeDic* rdic=NULL;
char* rec_dic = "Dictionaries/recipes.txt";


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


void recdic_ini(){
        FILE* f=fopen(rec_dic,"r");
        rdic=calloc(1,sizeof(RecipeDic));
        if(!rdic) return;

        fscanf(f,"%d",&rdic->size);
        rdic->rec=calloc(rdic->size,sizeof(Recipe*));
        if(!rdic->rec) {
                rdic=NULL;
                return;
        }
        for(int i=0; i<rdic->size; ++i) {
                rdic->rec[i]=rec_load(f);
                if(!rdic->rec[i]) {
                        rdic_free();
                        rdic=NULL;
                        return;
                }
        }
}

void rdic_free(){
        if(!rdic) return;
        if(rdic->rec) {
                for(int i=0; i<rdic->size; ++i) {
                        rec_free(rdic->rec[i]);
                }
                free(rdic->rec);
        }
        free(rdic);
}



/*
   Creates the array of objects and stores it in *obj. Also copies the array of
   quantities into *quant.
 */
Status rec_getData(Recipe * r, Object *** obj, int ** quant){
        if(!r) return ERROR;

        if(r->size == 0) return OK;

        (*obj) = (Object **) calloc (sizeof(Object *), r->size);
        if(!(*obj)) return ERROR;
        (*quant) = (int *) calloc (sizeof(int), r->size);
        if(!(*quant)) return ERROR;

        int i = 0;
        while(i < r->size) {
                (*obj)[i] = odic_lookup(r->elements[i]);
                (*quant)[i] = r->quantities[i];
                i++;
        }
        return OK;
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
Bool rec_doable(Inventory* inv, Recipe* r){
        int i = 0;
        if(!inv || !r) return FALSE;

        while(i < r->size) {
                if(inv_getQuantity(inv, r->elements[i]) < r->quantities[i]) {
                        return FALSE;
                }
                i++;
        }

        return TRUE;
}



/*
   Returns the recipies that can be created with that inventory.
   To do so, it will read the recipies in rec_dic and for each one of them, it will evaluate wheter or not it can be done.
   The amount of elements in the final array have to be stored in *size so that the caller can operate with the array.
   Note that the file rec_dic can be created in two ways:
   First the number of recipies and then each of them in the format specified in rec_load
   All the recipies one after another and we read ultil we reach EOF.
 */
Recipe** rec_getAllDoable(Inventory* inv, int * size){
        Recipe **r = malloc(0);

        if(!rdic) recdic_ini();
        if(!rdic) return NULL;
        for(int i=0; i < rdic->size; ++i) {
                if(rec_doable(inv, rdic->rec[i]) == TRUE) {
                        (*size)++;
                        r = realloc(r, (*size) * sizeof(Recipe));
                        r[i] = rec_copy(rdic->rec[i]);
                }
        }
        return r;
}
