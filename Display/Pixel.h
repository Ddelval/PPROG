//  Test
//	Pixel.h
//  Created by David del Val on 27/09/2019
//
//

#ifndef Pixel_h
#define Pixel_h

#include "Utility.h"
typedef struct _Pixel Pixel;

/*-----------------------------------------------------------------*/
/// Creates a new Pixel
/// @param r The value of the red channel
/// @param g The value of the green channel
/// @param b The value of the blue channel
/// @param a The value of the alpha channel
/// All these values have to be between 0 and 255 (both included)
Pixel* pix_ini(int r,int g, int b, int a);

/*-----------------------------------------------------------------*/
/// Frees the memory allocated for this pixel
void pix_free(Pixel* p);

/*-----------------------------------------------------------------*/
/// Creates a new pixel with the same values in every channel
/// @param src Pixel whose values will be copied
Pixel * pix_copy(const Pixel* src);

/*-----------------------------------------------------------------*/
/// Blends two pixels. It will take into account the colors and the opacity.
/// @param back The pixel that acts as background
/// @param top The pixel that is placed on top of the previous one
/// @return New pixel with the result of the blend
Pixel* pix_overlay(const Pixel* back, const Pixel* top);

/*-----------------------------------------------------------------*/
/// Changes the red value of p to r
Pixel* pix_setR(Pixel* p, int r);

/*-----------------------------------------------------------------*/
/// Changes the green value of p to g
Pixel* pix_setG(Pixel* p, int g);

/*-----------------------------------------------------------------*/
/// Changes the blue value of p to b
Pixel* pix_setB(Pixel* p, int b);

/*-----------------------------------------------------------------*/
/// Changes the alpha value of p to a
Pixel* pix_setA(Pixel* p, int a);

/*-----------------------------------------------------------------*/
/// Returns the value of the red channel of p
int pix_retR(const Pixel* p);

/*-----------------------------------------------------------------*/
/// Returns the value of the green channel of p
int pix_retG(const Pixel* p);

/*-----------------------------------------------------------------*/
/// Returns the value of the blue channel of p
int pix_retB(const Pixel* p);

/*-----------------------------------------------------------------*/
/// Returns the value of the alpha channel of p
int pix_retA(const Pixel* p);

/*-----------------------------------------------------------------*/
/// Copies the contents of one pixel to another.
/// This function can be useful when we have a grid of pixels
/// and we want to make a part of it equal to another set of pixels
/// @param dest Pixel that will take the new values
/// @param src Pixel whose values are copied
void pix_copyVals(Pixel*dest, const Pixel*src);

/*-----------------------------------------------------------------*/
/// Check if two pixels are equal.
/// By definition, for both of them to be equal,
/// the values in all the channels have to be the same
/// @param a First pixel
/// @param b Second pixel
bool pix_equals(const Pixel* a,const Pixel*b);

/*-----------------------------------------------------------------*/
/// Check if a pixel is transparent
/// @param a Pixel to be checked
bool pix_transparent(const Pixel* a);

/*-----------------------------------------------------------------*/
/// Render a horizontal line of pixels.
/// @param a	Pointer to the array of pixels
/// @param len 	Amount of pixels to be rendered
/// @return An array of chars that, if printed on the screen will represent this line of pixels.
char * pix_renderLine(Pixel**a,int len);

Pixel* pix_load(FILE* f);
#endif /* Pixel_h */
