#include "types.h"
#include "object.h"
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
  id idcons = 0;
}


inventory * inventory_ini(){
  inventory * inv = NULL;

  inv = (inventory *) calloc(1, sizeof(inventory));
  if ( !inv) return NULL;
}

void inventory_destroy(inventory * inv){
  if(!inv) return NULL;
  free(inv);
}

Bool inventorycons_full(inventory* inv){
  if(idcons == 14) return TRUE
  return FALSE;
}
Bool inventoryperm_full(inventory* inv){
  if(idperm == 14) return TRUE
  return FALSE;
}


Status object_add(inventory * inventory, char * object_id){
  if (!inventory || !object_id || object_id < 0) return ERROR;

  object * obj
  obj = object_load(object_id);

  if(object_class(obj) == TRUE){
    if(inventorycons_full(inventory) == TRUE) return ERROR;
    inventory->consumable[idcons] = obj;
    idcons++;
  }

  else{
    if(inventoryperm_full(inventory) == TRUE) return ERROR;
    inventory->permanent[idperm] = obj;
    idperm++;
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
