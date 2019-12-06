/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dialog.h
 * Author: jaime
 *
 * Created on 6 de diciembre de 2019, 17:01
 */

#ifndef DIALOG_H
#define DIALOG_H

#include "Entity.h"



typedef struct _DialogMan DialogMan;

/*
  Purpose: To create a new DialogMan struct
 Inputs: Nothing
 Outputs: DialogMan struct pointer
 */
DialogMan * dman_ini();

/*
  Purpose: To destroy DialogMan struct
 Inputs: DialogMan Struct
 Outputs: void
 */
void dman_destroy(DialogMan * dman);

/*
 Purpose: To check if there are more lines avaible for a counter
 Inputs: DialogMan pointer struct
 Outputs: Bool
 */
Bool dman_available(DialogMan * dman);

/*
  Purpose: New dialog instance for an entity
 Inputs: int id
 Outputs: DialogMan struct pointer
 */
DialogMan* dman_getDialog(int entity_id);
/*
  Purpose: Returns the next dialog line if there is one
 Inputs: DialogMan struct pointer
 Outputs: char pointer
 */
char* dman_nextLine(DialogMan* d);

int dman_getCounter(DialogMan * dman);

void dman_setCounter(DialogMan * dman, int counter);



#endif /* DIALOG_H */

