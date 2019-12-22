//  PPROG
//	Inventory.h
//  Created by David del Val on 05/12/2019
//
//

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Utility.h"
#include "Object.h"
#include "ObjectDic.h"
#include "Canvas.h"
#include "Font.h"

typedef struct _Inventory Inventory;


Inventory* inv_ini();
void inv_free(Inventory* inv);

Inventory* inv_copy(const Inventory* inv);
Inventory* inv_insert(Inventory* inv, const Object* ob);
Inventory* inv_remove(Inventory* inv, const Object* ob);
Inventory* inv_decrease(Inventory* inv, const Object* ob, int quantity);
Canvas *** inv_render(const Inventory* inv, int* dim, int ** dimens,char *** texts, pairii** sizes, const Font* ftext, const Font* fnum);
int inv_getQuantity(const Inventory* inv, int obj_id);
Inventory* inv_insertSeveral(Inventory* inv,const Object*ob,int quantity);
Inventory* inv_load(FILE* f);
const Object* inv_getSelected(const Inventory * inv, obj_type t);
int inv_getSelectedIndex(const Inventory* inv, obj_type t);
Inventory* inv_incrementSelected(Inventory* inv, obj_type t, int incr);
Canvas* inv_renderObj(Inventory* inv, obj_type t, int hei, int wid, const Font* ftext, const Font* fnum, int pos,bool selected);
#endif
