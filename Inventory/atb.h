
/* 
 * File:   atb.h
 * Author: jaime
 *
 * Created on 30 de septiembre de 2019, 9:48
 */


#ifndef ATB_H
#define ATB_H



typedef struct _atb atb;

#include "types.h"


atb * atb_ini();

void atb_destroy(atb * atb);

Status atb_setter(atb * atb, int value, int id);

int atb_getter(atb * atb, int id);

atb * atb_merge(atb * primary, atb * secondary);

int atb_print(FILE *pf, atb * atb);






#endif /* ATB_H */

