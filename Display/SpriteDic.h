//  ppro
//	SpriteDic.h
//  Created by David del Val on 20/10/2019
//
//

#ifndef SpriteDic_h
#define SpriteDic_h

#include "Utility.h"
#include "Sprite.h"
typedef struct _SpriteDic SpriteDic;
SpriteDic* sdic_ini(void);
void sdic_free(SpriteDic* d);
Sprite* sdic_lookup(int id);

#endif /* SpriteDic_h */
