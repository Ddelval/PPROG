//  PPROG
//	Inventory.h
//  Created by David del Val on 05/12/2019
//
//

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Utility.h"
#include "Object.h"

typedef struct _Inventory Inventory;


Inventory* inv_ini();
void inv_free(Inventory* inv);
Inventory* inv_insert(Inventory* inv, Object* ob);
Inventory* inv_remove(Inventory* inv, Object* ob);
Inventory* inv_decrease(Inventory* inv, Object* ob);


#endif