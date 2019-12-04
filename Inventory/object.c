/*File creator : Jaime Pons Garrido*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "atb.h"
#include "skill.h"
#include "types.h"
#include "Sprite.h"
#include "object.h"
extern int errno;

typedef enum {WEAPON = 1, CONSUMABLE = 2} objType;

struct _object {
    int id;
    char name[20];
    Sprite * image;
    objType class;
    int amount;
    atb * atb;
    skill ** attacks;
    Bool destroyable;
};

/*
 Function name: object_ini
 Utility: It creates a new object structure with NULL in each camp
 Inputs:
 Outputs: object pointer
 */
object * object_ini(){
    object * obj = NULL;
    obj = (object*)calloc(1,sizeof(object));

     if (obj== NULL) {
        printf("Error: calloc.\n");
        return NULL;
    }
    obj->atb = atb_ini();
    obj->attacks = NULL;
    obj->amount = 0;
    return obj;
}
/*
Function name: object_destroy
 Utility: Destroys an object struct
 Inputs: object to destroy (pointer)
 Outputs: object pointer
 */
void object_destroy(object* object){
    //FALTAN COSAS POR AÑADIR (CAMPOS TODAVIA NO CREADOS)

    atb_destroy(object->atb);
    free(object);
}
/*
Function name: object_load
 Utility: It takes a file with objects camps and inserts them into an object struct
 Inputs: object pointer, file pointer, name of the file.
 Outputs: object pointer
 */
object * object_load(FILE* f){
    if(!f)return NULL;
    int a,b,c,d,e;
    object * object;
    object = object_ini();
    fscanf(f,"%d %s %d %d %d %d %d %d\n",object->id,object->name,object->class,a,b,c,d,e);
   atb_setter(object->atb,a,1);
   atb_setter(object->atb,b,2);
   atb_setter(object->atb,c,3);
   atb_setter(object->atb,d,4);
   atb_setter(object->atb,e,5);
    if(object->class == 2){
        object->destroyable = TRUE;
    }
    fclose(f);
}

int consumable_decrease(object * obj){
  if(!obj) return -1;
  obj->amount--;
  return obj->amount;
}

int consumable_increase(object * obj){
  if(!obj) return -1;
  obj->amount++;
  return obj->amount;
}

char * object_getname(object * obj){
  return obj->name;
}



//CONTINUAMOS EN DESARROLLO A LA ESPERA DE LA INTERFAZ Y EL TAD INVENTORY GRACIAS!!
