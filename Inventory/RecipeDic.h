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

Recipe* rdic_lookup(int id);
Recipe** rdic_getAllDoable(Inventory* inv, int * size);
#endif
