/*
   File:   Crafting.h
   Author: Pedro Urbina
   Created on: 6 december 2019
*/


#ifndef Recipe_H
#define Recipe_H


#include "Inventory.h"
#include "Object.h"
#include "ObjectDic.h"
#include "FontCatalog.h"


typedef struct _Recipe Recipe;

Recipe* rec_ini();
void rec_free(Recipe *r);

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


int * rec_getQuantities(Recipe * r);
int * rec_getElements(Recipe * r);
int rec_getResult_id(Recipe * r);
int rec_getSize(Recipe * r);
char *rec_getName(Recipe * r);
Canvas* rec_render(Recipe* r, int obj_wid, int wid, int hei, int max_wid);
Recipe* rec_getObjDimensions(Recipe* r,int *ob_wid, int* hei);
int rec_getMinWidth(Recipe* r, int obj_wid, int hei);
Recipe* rec_make(Recipe* r, Inventory* inv);
#endif
