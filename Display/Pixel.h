//  Test
//	Pixel.h
//  Created by David del Val on 27/09/2019
//
//

#ifndef Pixel_h
#define Pixel_h

#include "Utility.h"
typedef struct _Pixel Pixel;

Pixel* pix_ini(int r,int g, int b, int a);
void pix_free(Pixel* p);
Pixel * pix_copy(Pixel* src);

Pixel* pix_overlay(Pixel* back, Pixel* top);

Pixel* pix_setR(Pixel* p, int r);
Pixel* pix_setG(Pixel* p, int g);
Pixel* pix_setB(Pixel* p, int b);
Pixel* pix_setA(Pixel* p, int a);

int pix_retR(Pixel* p);
int pix_retG(Pixel* p);
int pix_retB(Pixel* p);
int pix_retA(Pixel* p);

bool pix_equals(Pixel* a,Pixel*b);
char * pix_renderLine(Pixel**a,int len);


#endif /* Pixel_h */
