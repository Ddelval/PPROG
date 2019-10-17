/*File creator : Jaime Pons Garrido*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "atb.h"
#include "types.h"
#include "object.h"
extern int errno;

typedef enum {WEAPON = 1, CONSUMABLE = 2} objType;

struct _object {
    int id;
    char name[20];
    sprite * image;
    objType class;
    atb * atb;
    skill * special;
    Bool destroyable;
};

/*
 Function name: object_ini
 Utility: It creates a new object structure with NULL in each camp
 Inputs:
 Outputs: object pointer
 */
object * object_ini(){
    object * object = NULL;
    object = (object *)calloc(1,sizeof(object));
    
     if (object== NULL) {
        printf("Error: calloc.\n");
        return NULL;
    }
    object->atb = atb_ini();
    object->special = NULL;
    
    
    
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
object * object_load( char * name){
    FILE * f;
    int a,b,c,d,e;
    object * object;
    object = object_ini();
    f = fopen(name ,"r");
    
    
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






//CONTINUAMOS EN DESARROLLO A LA ESPERA DE LA INTERFAZ Y EL TAD INVENTORY GRACIAS!!

