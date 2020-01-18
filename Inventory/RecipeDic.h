/*
   File:   Crafting.h
   Author: Pedro Urbina
   Created on: 6 december 2019
*/


#ifndef RecipeDic_H
#define RecipeDic_H


#include "Inventory.h"
#include "Recipe.h"
#include "Object.h"
#include "ObjectDic.h"

/*
   Function name: rdic_lookup
   Utility: Looks for the recipe with the given id in the loaded recipes.
            In order to do that it initializes a global variable with all
            the recipes from a text file.
   Inputs: int id: The id of the recipe to be searched
   Outputs: The recipe we were looking for or NULL in case of error.
 */
Recipe* rdic_lookup(int id);
/*
   Function name: rdic_getAllDoable
   Utility: Looks for all the recipes which can be crafted with the current
            objects of the inventory.
   Inputs: Inventory* inv: The inventory from where we look the recipes that
                           can be manufactured.
           int * size: The address of an integer where the number of recipes
                       which can be done  will be stored.
   Outputs: NULL in case of error or an array of recipe pointers with all
            the recipes that can be created.
 */
Recipe** rdic_getAllDoable(Inventory* inv, int * size);
#endif
