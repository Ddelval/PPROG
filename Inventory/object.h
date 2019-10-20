

/*
 * File:   object.h
 * Author: jaime
 *
 * Created on 2 de octubre de 2019, 16:45
 */


#ifndef OBJECT_H
#define OBJECT_H



typedef struct _object object;

#include "types.h"
/*
 Function name: object_ini
 Utility: It creates a new object structure with NULL in each camp
 Inputs:
 Outputs: object pointer
 */
object * object_ini();
/*
Function name: object_destroy
 Utility: Destroys an object struct
 Inputs: object to destroy (pointer)
 Outputs: object pointer
 */
void object_destroy(object * atb);
/*
Function name: object_load
 Utility: It takes a file with objects camps and inserts them into an object struct
 Inputs: object pointer, file pointer, name of the file.
 Outputs: object pointer
 */
object * object_load(object* object, FILE * pf, char * name);



#endif /* OBJECT_H */
