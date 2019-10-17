

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

object * object_ini();

void object_destroy(object * atb);

object * object_load(object* object, FILE * pf, char * name);



#endif /* OBJECT_H */

