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

/*
   Function name: rec_ini
   Utility: It creates a new Recipe structure with 0's, NULLs in each field (and id -1).
   Inputs:
   Outputs: Recipe pointer or NULL in case of error.
 */
Recipe* rec_ini();
/*
   Function name: rec_free
   Utility: Frees all the memory used by the given Recipe.
   Inputs: Recipe * of the recipe to be freed.
   Outputs:
 */
void rec_free(Recipe *r);
/*
   Function name: rec_copy
   Utility: It creates a new Recipe structure with same values as the one passed as argument.
   Inputs: Recipe * to be copied
   Outputs: Recipe pointer of the copied recipe or NULL in case of error.
 */
Recipe * rec_copy(Recipe *r);

/*
   Function name: rec_getData
   Utility: Creates the array of objects needed for the recipe. Also creates an
            array of quantities where the quantities needed for each object are copied.
   Inputs: The recipe pointer
           A triple pointer to a double array of objets, where the needed objtects
           are going to be stored.
           A pointer to an array of ints where the quantities of each object are to
           be stored.
   Outputs: Recipe pointer passed as input or NULL in case of error.
 */
Recipe* rec_getData(Recipe * r, Object *** obj, int ** quant);
/*
   Function name: rec_load
   Utility: Reads a recipe from file and creates a recipe with those values.
            It uses the following format:  (size) (name) (result_id) (elements_ids)(list_of_quantites)
   Inputs: FILE * f: Pointer to a previously opened file with the specified format.
   Outputs: Recipe pointer of the created recipe or NULL in case of error.
 */
Recipe* rec_load(FILE *f);
/*
   Function name: rec_doable
   Utility: Checks if this recipe can be prepared with the objects present in the inventory
   Inputs: Inventory* inv: The inventory where the checking is going to be carried out.
           Recipe *r: The recipe where are checking if it can be made.
   Outputs: True if it can be done, False otherwise or in case of error.
*/
bool rec_doable(Inventory* inv, Recipe* r);

//IMPORTANT NOTE: The next 5 functions do not follow the principle of encapsulation,
//                they just return a pointer to those fields.
/*
   Function name: rec_getQuantities
   Utility: Returns a pointer to the quantities field of the recipe structure passed as input.
   Inputs: Recipe *r: The recipe.
   Outputs:  A pointer to the quantities field of the recipe structure or NULL in case of error.
*/
int * rec_getQuantities(Recipe * r);
/*
   Function name: rec_getElements
   Utility: Returns a pointer to the elements field of the recipe structure passed as input.
   Inputs: Recipe *r: The recipe.
   Outputs:  A pointer to the elements field of the recipe structure or NULL in case of error.
*/
int * rec_getElements(Recipe * r);
/*
   Function name: rec_getResult_id
   Utility: Returns the id of the recipe structure passed as input.
   Inputs: Recipe *r: The recipe.
   Outputs: An integer representing the id of the recipe or -1 in case of error.
*/
int rec_getResult_id(Recipe * r);
/*
   Function name: rec_getSize
   Utility: Returns the size of the recipe structure passed as input.
   Inputs: Recipe *r: The recipe.
   Outputs: An integer representing the size of the recipe or -1 in case of error.
 */
int rec_getSize(Recipe * r);
/*
   Function name: rec_getName
   Utility: Returns the name of the recipe structure passed as input.
   Inputs: Recipe *r: The recipe.
   Outputs: An char pointer to the name of the recipe or NULL in case of error.
 */
char *rec_getName(Recipe * r);


/*
   Function name: rec_render
   Utility: Renders the recipe.
   Inputs: Recipe *r: The recipe used.
           int obj_wid: object's width
           int wid: width
           int hei: height
           int max_wid: maximum permited width
   Outputs: Returns NULL in case of error or the canvas with the drawn recipe.
*/
Canvas* rec_render(Recipe* r, int obj_wid, int wid, int hei, int max_wid);
/*
   Function name: rec_getObjDimensions
   Utility: Saves in the variables passed as inputs the dimensions of the object.
   Inputs: Recipe *r: The recipe used.
           int *obj_wid: here it is going to be stored the object's width
           int *hei: here it is going to be stored the object's height
   Outputs: Returns the recipe passed as input or -1 in case of error.
*/
Recipe* rec_getObjDimensions(Recipe* r,int *ob_wid, int* hei);
/*
   Function name: rec_getMinWidth
   Utility: Returns the minimum width needed to render the recipe.
   Inputs: Recipe *r: The recipe used.
           int obj_wid: the object's width
           int hei: height
   Outputs: Returns the minimum width needed to render the recipe or -1 in case of error.
*/
int rec_getMinWidth(Recipe* r, int obj_wid, int hei);
/*
   Function name: rec_make
   Utility: Creates the object from the recipe and inserts it into the Inventory,
            errasing the used objects from it as well.
   Inputs: Recipe *r: The recipe which will be done.
           Inventory * inv: The inventory where the objects to do the recipe are
                            taken from and the created object is going to be inserted.
   Outputs: Returns NULL in case of error and the recipe passed as input otherwise.
*/
Recipe* rec_make(Recipe* r, Inventory* inv);
/*
   Function name: rec_decrease
   Utility: Errases the objects from the inventory needed for doing a certain recipe.
   Inputs: Recipe *r: The recipe which will be done.
           Inventory * inv: The inventory where the objects to do the recipe are taken from.
   Outputs: Returns NULL in case of error and the recipe passed as input otherwise.
*/
Recipe* rec_decrease(Recipe* r, Inventory* inv);
/*
   Function name: rec_getTier
   Utility: Returns the tier of the recipe structure passed as input.
   Inputs: Recipe *r: The recipe.
   Outputs: An integer representing the tier of the recipe or -1 in case of error.
*/
int rec_getTier(Recipe* r);
#endif
