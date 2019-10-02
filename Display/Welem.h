//  ppro
//	Welem.h
//  Created by David del Val on 30/09/2019
//
//

#ifndef Welem_h
#define Welem_h

#include "Utility.h"
#include "Canvas.h"
typedef enum {LABEL, ICONLABEL, ICON} Wtype;
typedef struct _Welem Welem;

Welem* we_ini(Wtype t, void* data);
void we_free(Welem* w);
Welem* we_copy(Welem* w);
Canvas* we_render(Welem*w);
	
#endif /* Welem_h */
