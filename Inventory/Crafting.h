/*
   File:   Crafting.h
   Author: Pedro Urbina
   Created on: 6 december 2019
*/


#ifndef Crafting_H
#define Crafting_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Inventory.h"
#include "Object.h"
#include "ObjectDic.h"


typedef struct _Recipe Recipe;

Recipe* rec_ini();
void rec_free(Recipe *r);

void recdic_ini();
void rdic_free();
Recipe * rec_copy(Recipe *r);


/*
   Creates the array of objects and stores it in *obj. Also copies the array of
   quantities into *quant.
 */
Recipe* rec_getData(Recipe * r, Object *** obj, int ** quant);

//Following format:  (size) (name) (result_id) (elements_ids)(list_of_quantites)
Recipe* rec_load(FILE *f);

/*
   Checks if this recipe can be prepared with the objects present in the inventory
   Note that it might be a great idea to add the following function to inventory:
   int inv_getQuantity(Inventory* inv, int obj_id)that returns -1 if the element
   is not present or the quantiy that the inventory has of it otherwise.
 */
bool rec_doable(Inventory* inv, Recipe* r);

/*
   Returns the recipies that can be created with that inventory.
   To do so, it will read the recipies in rec_dic and for each one of them, it will evaluate wheter or not it can be done.
   The amount of elements in the final array have to be stored in *size so that the caller can operate with the array.
   Note that the file rec_dic can be created in two ways:
   First the number of recipies and then each of them in the format specified in rec_load
   All the recipies one after another and we read ultil we reach EOF.
 */
Recipe** rec_getAllDoable(Inventory* inv, int * size);

#endif
