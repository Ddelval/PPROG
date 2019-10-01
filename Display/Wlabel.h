//  ppro
//	Wlabel.h
//  Created by David del Val on 30/09/2019
//
//

#ifndef Wlabel_h
#define Wlabel_h

#include "Utility.h"
#include "Font.h"
#include "Canvas.h"
typedef struct _Wlabel Wlabel;

Wlabel* wl_ini(char* t, const Font* f, int vgap);
void wl_free(Wlabel* l);
Canvas* wl_render(Wlabel* l,int width);



#endif /* Wlabel_h */
