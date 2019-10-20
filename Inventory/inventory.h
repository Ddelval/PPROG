

/*
 * File:   inventory.h
 * Author: jaime
 *
 * Created on 18 de octubre de 2019
 */


#ifndef INVENTORY_H
#define INVENTORY_H



typedef struct _inventory inventory;

#include "types.h"
#include "object.h"
#include "atb.h"
/*
 Function name: inventory_ini
 Utility: It creates a new inventory structure with NULL in each camp
 Inputs:
 Outputs: inventory pointer
 */
inventory * inventory_ini();
/*
Function name: inventory_destroy
 Utility: Destroys an inventory struct
 Inputs: inventory to destroy (pointer)
 Outputs: inventory pointer
 */
void inventory_destroy(inventory * inventory);
/*
Function name: object_add
Utility: The function receives the id of an object so it creates it and
then places it into the inventory alphabetically sorted
Imputs: inventory pointer, int (object id)
Outputs: ERROR code or Ok code if everything goes as expected
*/
Status object_add(inventory * inventory, int object_id);
/*
Function name: object_remove
Utility: The function receives the id of an object if it is weapon it deletes it
if not it decreseases a counter placed in the inventory structure.
Imputs: inventory pointer, int (object id)
Outputs: ERROR code or Ok code if everything goes as expected
*/
Status object_remove(inventory * inventory, int object_id);
/*
Function name: object_select
Utility: The function goes into the array of the inventory and finds
the desired object, then applies its effects and calls object_remove if necessary
Imputs: inventory pointer, int (object id)
Outputs: ERROR code or Ok code if everything goes as expected
*/
Status object_select (inventory * inv, int object_id);







#endif /* INVENTORY_H*/
