//  Test
//	Canvas.h
//  Created by David del Val on 27/09/2019
//
//

#ifndef Canvas_h
#define Canvas_h
#include "Pixel.h"
typedef struct _Canvas Canvas;

Canvas* canv_load(FILE* f);
void canv_free(Canvas* c);
Canvas* canv_copy (Canvas* bas);

char** canv_render(Canvas* c,int wid, int hei);

void canv_print(FILE* f,Canvas* c,int x,int y);
#endif /* Canvas_h */
