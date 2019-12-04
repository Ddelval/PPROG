#include "types.h"
#include "object.h"
#include "inventory.h"
#include "atb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

extern int errno;

struct _inventory {
  object * consumable[15];
  object * permanent[15];
  int idperm = 0;
  int idcons = 0;
};


inventory* inventory_ini(){
  inventory * inv = NULL;

  inv = (inventory *) calloc(1, sizeof(inventory));
  if ( !inv) return NULL;
}

void inventory_destroy(inventory * inv){
  if(!inv) return NULL;
  free(inv);
}

Bool inventorycons_full(inventory* inv){
  if(inv->idcons == 14) return TRUE;
  return FALSE;
}
Bool inventoryperm_full(inventory* inv){
  if(inv->idperm == 14) return TRUE;
  return FALSE;
}


Status object_add(inventory * inv, char * object_id){
  if (!inv || !object_id || object_id < 0) return ERROR;

  object * obj;
  obj = object_load(object_id);

  if(object_class(obj) == TRUE){
    if(inventorycons_full(inv) == TRUE) return ERROR;
    inv->consumable[inv->idcons] = obj;
    inv->idcons++;
  }

  else{
    if(inventoryperm_full(inv) == TRUE) return ERROR;
    inv->permanent[inv->idperm] = obj;
    inv->idperm++;
  }

return OK;

}


Status object_remove(inventory * inventory, int position, Bool class){
  if (!inventory || !position || position < 0) return ERROR;

  if(class == TRUE){

    consumable_decrease(inventory->consumable[position]
  }

  if(class == FALSE){
    free(inventory->permanent[position]);
    inventory->permanent[position] = NULL;
    for(i = position; i<15; i++){
      inventory->permanent[i] = inventory->permanent[i+1];
    }

  }







}
