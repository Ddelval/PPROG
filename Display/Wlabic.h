//  ppro
//	Wlabic.h
//  Created by David del Val on 20/10/2019
//
//

#ifndef Wlabic_h
#define Wlabic_h

#include "Utility.h"
#include "Wlabel.h"

typedef struct _Wlabic Wlabic;


Wlabic* wi_ini(char *t, const Font* f,int vgap, int hgap);
void wi_free(Wlabic* w);
Wlabic* wi_copy(const Wlabic* src);


#endif /* Wlabic_h */
