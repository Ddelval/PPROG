//  PPROG
//	Inventory.h
//  Created by David del Val on 05/12/2019
//
//

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Utility.h"
#include "Object.h"
#include "Canvas.h"
#include "Font.h"

typedef struct _Inventory Inventory;


Inventory* inv_ini();
void inv_free(Inventory* inv);
Inventory* inv_insert(Inventory* inv, Object* ob);
Inventory* inv_remove(Inventory* inv, Object* ob);
Inventory* inv_decrease(Inventory* inv, Object* ob, int quantity);
Canvas *** inv_render(Inventory* inv, int* dim, int ** dimens,char *** texts,Font* ftext, Font* fnum);
int inv_getQuantity(Inventory* inv, int obj_id);
Inventory* inv_insertSeveral(Inventory* inv,Object*ob,int quantity);
#endif
