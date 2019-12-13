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
#include "Recipe.h"
#include "Object.h"
#include "ObjectDic.h"

void rdic_ini();
void rdic_free();
Recipe* rdic_lookup(int id);
Recipe** rdic_getAllDoable(Inventory* inv, int * size);
#endif
