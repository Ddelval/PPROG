//  PPROG
//	Pixel.h
//  Created by David del Val on 27/09/2019

/**
 * @brief The pixel ADT represents a color
 * 
 * This ADT also includes all the necessary functions to overlay
 * two colors and deal with transparencies.
 * 
 * In order to do so, aside from the color information, we also
 * store the alpha channel. The higher the alpha value of a pixel,
 * the more opaque it is.
 * 255 -> opaque
 * 0   -> transparent
 */

#ifndef Pixel_h
#define Pixel_h

#include "Utility.h"
typedef struct _Pixel Pixel;


/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new Pixel
 * 
 * @param r The value of the red channel
 * @param g The value of the green channel
 * @param b The value of the blue channel
 * @param a The value of the alpha channel.
 * @return Pixel* New pixel with these values
 */
Pixel* pix_ini(int r,int g, int b, int a);

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new transparent pixel
 * 
 * @return Pixel*: a new transparent pixel
 */
Pixel* pix_newTransparent();

/*-----------------------------------------------------------------*/
/**
 * @brief Loads the pixel from a file
 * This function was created to set up 
 * a format in which the colors should be stored in files.
 * 
 * @param f File
 * @return Pixel* The pixel containing the data read from the file
 */
Pixel* pix_load(FILE* f);

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the pixel to a file
 * 
 * @param p Pixel to be printed
 * @param f File in which the pixel will be printed
 * @return  NULL if error
 */
Pixel* pix_print(Pixel* p, FILE* f);

/*-----------------------------------------------------------------*/
/// Creates a new pixel with the same values in every channel
/// @param src Pixel whose values will be copied
Pixel * pix_copy(const Pixel* src);

/*-----------------------------------------------------------------*/
/**
 * @brief Averages the value of two pixels
 * 
 * This function may be used to scale down images.
 * For instance it can be aplied to graphics generate
 * in-game whose aspect ratio is important. 
 * In order to maintain the aspect ratio, it is necessary to 
 * shrink them by half in the vertical axis
 * 
 * @param p1 
 * @param p2 
 * @return Pixel* 
 */
Pixel* pix_average(const Pixel* p1,const Pixel*p2);

/*-----------------------------------------------------------------*/
/// Blends two pixels. It will take into account the colors and the opacity.
/// @param back The pixel that acts as background
/// @param top The pixel that is placed on top of the previous one
/// @return New pixel with the result of the blend
Pixel* pix_overlay(const Pixel* back, const Pixel* top);



/*-----------------------------------------------------------------*/
/// Frees the memory allocated for this pixel
void pix_free(Pixel* p);

/*-----------------------------------------------------------------*/
/// Copies the contents of one pixel to another.
/// This function can be useful when we have a grid of pixels
/// and we want to make a part of it equal to another set of pixels
/// @param dest Pixel that will take the new values
/// @param src  Pixel whose values are copied
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
/**
 * @brief Checks if the pixel is half transparent or more
 * 
 * This function will be used to differenciate pixels that 
 * represent shadows from the ones that represent actual objects
 * 
 * @param a Pixel to be tested
 * @return true if the pixel is less than or half opaque
 * @return false if the pixel is more than half opaque
 */
bool pix_halfTransparent(const Pixel* a);

/*-----------------------------------------------------------------*/
/// Returns the scape sequence that will change the background
/// color of the terminal to the color of this pixel.
/// @param a Pixel whose color will be taken
char* pix_scapeSeq(const Pixel* a);

/*-----------------------------------------------------------------*/
/// Render a horizontal line of pixels.
/// @param a	Pointer to the array of pixels
/// @param len 	Amount of pixels to be rendered
/// @return An array of chars that, if printed on the screen will 
///         represent this line of pixels.
char * pix_renderLine(const Pixel**a,int len);

/*-----------------------------------------------------------------*/
/**
 * @brief Reduces the brightness of the pixel
 * 
 * @param p     Pixel to be modified
 * @param light The percentage of light that will remain
 * @return Pixel* A not-null pointer if the process was successful
 */
Pixel* pix_darken(Pixel* p,double light);



/* ############################################################### */
/* ###################### SETTERS AND GETTERS #################### */
/* ############################################################### */

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

#endif /* Pixel_h */
