//  ppro
//	Sprite.h
//  Created by David del Val on 06/10/2019
//
//

#ifndef Sprite_h
#define Sprite_h

#include "Utility.h"
#include "Canvas.h"
typedef struct _Sprite Sprite;

Sprite* spr_ini(int id, int width, int height);
void spr_free(Sprite* sp);
Sprite* spr_copy(const Sprite* spr);
Sprite* spr_load(FILE* f);Sprite* spr_load(FILE* f);
const Canvas* spr_getDispData(Sprite* spr);
void spr_setCoodrinates(Sprite* sp,int i,int j);
int spr_getOI(Sprite* spr);
int spr_getOJ(Sprite* spr);
void spr_setOJ(Sprite* spr, int jpos);
void spr_setOI(Sprite* spr,int ipos);
int spr_getId(const Sprite* sp);
#endif /* Sprite_h */
