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

/**
 * @brief Creates and arranges memory for an inventory structure
 *
 * @param inventory Inventory structure pointer
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_ini();
/**
 * @brief Frees an inventory structure
 *
 * @param inventory Inventory structure pointer
 * @return  void
 *           
 */
void inv_free(Inventory* inv);
/**
 * @brief Creates and delivers a copy of an inventory structure
 *
 * @param inventory Inventory structure pointer
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_copy(const Inventory* inv);
/**
 * @brief Inserts an object into an inventory.
 *
 * @param inventory Inventory structure pointer
 * @param object Object pointer
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_insert(Inventory* inv, const Object* ob);
/**
 * @brief Deletes an object in an inventory.
 *
 * @param inventory Inventory structure pointer
 * @param object Object pointer
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_remove(Inventory* inv, const Object* ob);
/**
 * @brief Decreases the amount of an object in an inventory.
 *
 * @param inventory Inventory structure pointer
 * @param object Object pointer
 * @param int quantity
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_decrease(Inventory* inv, const Object* ob, int quantity);
/**
 * @brief Returns the formed canvas of an inventory
 *
 * @param inventory Inventory structure pointer
 * @param integer pointer dim
 * @param integer double pointer dimens
 * @param char double pointer texts
 * @param pairii double pointer sizes
 * @param font pointer ftext
 * @param font pointer fnum
 * @return  NULL if there has been any error
 *          Canvas triple pointer if everything was ok
 *           
 */
Canvas *** inv_render(const Inventory* inv, int* dim, int ** dimens,char *** texts, pairii** sizes, const Font* ftext, const Font* fnum);
/**
 * @brief Gets the amount of an object in an inventory.
 *
 * @param inventory Inventory structure pointer
 * @param object Object pointer
 * @return  NULL if there has been any error
 *          Integer amount of the selected object
 *           
 */
int inv_getQuantity(const Inventory* inv, int obj_id);
/**
 * @brief Inserts several amounts of an object into an inventory.
 *
 * @param inventory Inventory structure pointer
 * @param object Object pointer
 * @param integer amount
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_insertSeveral(Inventory* inv,const Object*ob,int quantity);
/**
 * @brief Loads an already existing inventory from a file
 *
 * @param FILE pointer 
 
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_load(FILE* f);
/**
 * @brief Checks if the object is present in the inventory
 *
 * @param inventory Inventory structure pointer
 * @param pairii pointer obj
 * @param integer len
 * @return  bool True
 *          bool False
 *           
 */
bool inv_checkPresent(const Inventory* inv, pairii* obj, int len);
/**
 * @brief Provides a copy of the selected object
 *
 * @param inventory Inventory structure pointer
 * @param obj_type t
 * @return  NULL if there has been any error
 *          Object pointer if everything was ok
 *           
 */
Object* inv_getSelectedCopy(const Inventory* inv, obj_type t);
const Object* inv_getSelected(const Inventory * inv, obj_type t);
int inv_getSelectedIndex(const Inventory* inv, obj_type t);
/**
 * @brief Moves the position that marks the selected object
 *
 * @param inventory Inventory structure pointer
 * @param obj_type t
 * @param integer incr
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Inventory* inv_incrementSelected(Inventory* inv, obj_type t, int incr);
/**
 * @brief Renders an inventory object for the display
 *
 * @param inventory Inventory structure pointer
 * @param obj_type t
 * @param integer hei
 * @param integer wid
 * @param font ftext
 * @param font fnum
 * @param integer pos
 * @param bool selected
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Canvas* inv_renderObj(Inventory* inv, obj_type t, int hei, int wid, const Font* ftext, const Font* fnum, int pos,bool selected);
#endif
