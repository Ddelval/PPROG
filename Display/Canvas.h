//  PPROG
//	Canvas.h
//  Created by David del Val on 27/09/2019

/// Description:
/// The canvas ADT is the data structure that holds an array
/// of pixels. Additionally, it includes all the functions required
/// to work with them.
/// Instances of Canvas may be overlayed, appended, splitted...
/// Furthermore, the objects that are printed to the screen are
/// of the type Canvas.
///
/// It is important to note that coordinates (0,0) will be considered
/// the top left corner and we wil refer to the row number with i and
/// to the column number with j.
/// We will follow the same conventions as when we access a 2-D array

#ifndef Canvas_h
#define Canvas_h
#include "Pixel.h"
typedef struct _Canvas Canvas;
typedef enum {LEFT,RIGHT} CAlign;

/*-----------------------------------------------------------------*/
/// Free the allocated memory
void canv_free(Canvas* c);

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new canvas
 * 
 * Note that this function will not initalise the array of pixels.
 * It will only allocate the memory for it.
 * 
 * @param wid Width of the new canvas
 * @param hei Height of the new canvas
 * @return New canvas
 */
Canvas* canv_ini(int wid,int hei);

/*-----------------------------------------------------------------*/
/**
 * @brief Loads a canvas from a file
 * 
 * The format should be the following:
 * (width) (height)
 * [r,g,b,a] [r,g,b,a]...
 * 
 * @param f File from which the data will be read
 * @return Pointer to the new canvas
 */
Canvas* canv_load(FILE* f);

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new canvas of a given color and dimensions
 * 
 * @param r         red component
 * @param g         green component
 * @param b         blue component
 * @param a         alpha value
 * @param width     width of the canvas
 * @param height    height of the canvas
 * @return Canvas*  to the new canvas
 */
Canvas* canv_backGrnd(int r, int g, int b, int a, int width, int height);

/*-----------------------------------------------------------------*/
/**
 * @brief Copies the given canvas
 * 
 * @param bas   Canvas to be copied
 * @return      New anvas with exactly the same data
 */
Canvas* canv_copy (const Canvas* bas);
/*-----------------------------------------------------------------*/
/**
 * @brief Returns a copy of a section of the given Canvas
 * 
 * @param bas   Source canvas
 * @param i2    i starting index (included)
 * @param i1    i ending   index (excluded) 
 * @param j1    j starting index (included)
 * @param j2    j ending   index (excluded)
 * @return      New canvas containing a copy of the given section
 */
Canvas* canv_subCopy (const Canvas* bas,int i1,int i2,int j1,int j2);
/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases horizontally
 * 
 * That is to say the resulting canvas looks like:
 * 
 * WWWWWWW
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWW
 * 
 * Where the 'W' repesent the west canvas and the 'E'
 * represents the east canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered vertically.
 * 
 * @param west  Canvas to the left
 * @param east  Canvas to the right
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendH(const Canvas* west, const Canvas* east);

/*-----------------------------------------------------------------*/
/**
 * @brief   Appends two canvases horizontally without aligning
 *          the smaller one
 * 
 * That is to say the resulting canvas looks like:
 * 
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWW
 * WWWWWWW
 * 
 * Where the 'W' repesent the west canvas and the 'E'
 * represents the east canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be aligned relative to its top.
 * 
 * @param west  Canvas to the left
 * @param east  Canvas to the right
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendHNL(const Canvas* west, const Canvas* east);

/*-----------------------------------------------------------------*/
/**
 * @brief Appends the east canvas to the right of the west one.
 *        In this case the west canvas will reflect the changes
 * 
 * That is to say the resulting canvas looks like:
 * 
 * WWWWWWW
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWW
 * 
 * Where the 'W' repesent the west canvas and the 'E'
 * represents the east canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered vertically.
 * 
 * @remark  The result of the operation will be stored in the west
 *          canvas. The return value is only used to report errors.
 * 
 * @param west  Canvas to the left
 * @param east  Canvas to the right
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendHI(Canvas* west, const Canvas* east);



/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases vertically
 * 
 * That is to say the resulting canvas looks like:
 * 
 *    NNNNNN
 *    NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 * 
 * Where the 'N' repesent the north canvas and the 'S'
 * represents the south canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered horizontally.
 * 
 * @param north  Canvas on the top
 * @param south  Canvas on the bottom
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendV(const Canvas* north, const Canvas* south);

/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases vertically and stores the result
 *        in the top one
 * 
 * That is to say the resulting canvas looks like:
 * 
 *    NNNNNN
 *    NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 * 
 * Where the 'N' repesent the north canvas and the 'S'
 * represents the south canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered horizontally.
 * 
 * @param north  Canvas on the top
 * @param south  Canvas on the bottom
 * @return       A NULL pointer in case of error, north otherwise
 */
Canvas* canv_appendVI(Canvas* north, const Canvas* south);

/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases vertically using the given alignment 
 *        and stores the result in the top one
 * 
 * When al=LEFT, the resulting canvas looks like:
 * 
 *  NNNNNN
 *  NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 *
 * When al=RIGHT, the resulting canvas looks like:
 * 
 *      NNNNNN
 *      NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 * Where the 'N' repesent the north canvas and the 'S'
 * represents the south canvas.
 * 
 * 
 * @param north Canvas on the top
 * @param south Canvas on the bottom
 * @param al    Alignement of the canvases
 * @return      A NULL pointer in case of error, north otherwise
 */
Canvas* canv_appendVIA(Canvas* north,const Canvas* south,CAlign al);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a margin to a canvas. The padding will be transparent.
 * 
 * @param src       Canvas that will be placed in the center
 * @param top       Margin in the top
 * @param right     Margin in the right
 * @param bottom    Margin in the bottom
 * @param left      Margin in the left
 * @return          New canvas with the given canvas and the margins
 */
Canvas* canv_addMargin (const Canvas *src, int top, int right, int bottom, int left);

/*-----------------------------------------------------------------*/
/**
 * @brief Split a canvas into smaller sub-canvases making vertical separations
 * 
 * This function will split the canvas into pieces that are separated by 
 * transparent horizontal gaps.
 * This can be particularly useful when we want to load more than one 
 * canvas in a single file. 
 * 
 * For instance, if a picture contains several leters:
 * a b c d e
 * This function will return each letter in a separete canvas
 * 
 * @param src       Canvas to be splitted
 * @param nelem     Pointer to an integer where the number
 *                  of returned elements will be stored
 * @return          An array of Canvas* that contains all non-transparent
 *                  sub-sections of the given canvas separated.
 */
Canvas ** canv_VSplit(const Canvas* src, int* nelem);

/*-----------------------------------------------------------------*/
/**
 * @brief   Adjust a canvas to a given dimensions.
 * 
 * 
 * If the canvas is smaller, it will be centered and the rest
 * of the pixels will be transparent
 * If the canvas is bigger, it will be centered and cropped
 * 
 * @param src       Original canvas
 * @param nwidth    New width
 * @param nheight   New height
 * @return          New canvas with the given dimensions
 */
Canvas* canv_AdjustCrop(const Canvas* src, int nwidth,int nheight);

/*-----------------------------------------------------------------*/
/**
 * @brief       Merges two canvases into one
 * 
 * The last two parameters indicate the coordinates of
 * the top-left corner of over when taking the top-left corner
 * of base as a reference frame.
 * 
 * @param base  The canvas that will act as background
 * @param over  The canvas that will act as foreground
 * @param o_i   The offset in the vertical axis
 * @param o_j   The offset in the horizontal axis
 * @return      New canvas with the result of overlaying one canvas
 *              on top of the other
 */
Canvas* canv_Overlay(const Canvas* base, const Canvas* over, int o_i, int o_j);
/*-----------------------------------------------------------------*/
/**
 * @brief Merges a new canvas in the base one
 * 
 * @param base  The canvas that will act as background
 * @param over  The canvas that will act as foreground
 * @param o_i   The top limit of the foreground canvas
 * @param o_j   The left limit of the foreground canvas
 * @return      A NULL pointer if an error ocurred.
 *              base otherwise
 */
Canvas* canv_addOverlay(Canvas* base, const Canvas* over, int o_i, int o_j);

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the canvas to the file
 * 
 * Note that this function will not check if the canvas fits on the screen
 * 
 * @param f File in which the canvas will be printed
 * @param c Canvas to be printed
 * @param i x-coordinate of of the canvas when it is displayed on the screen
 * @param j Left limit of the canvas when it is displayed on the screen
 */
void canv_print(FILE* f, Canvas* c,int i,int j);

/*-----------------------------------------------------------------*/
/**
 * @brief Returns the characters that have to be echoed to the terminal
 *        in order to print this canvas.
 * 
 * The characters include the movement of the cursor to the required
 * positions
 * 
 * 
 * @param c Canvas to be printed
 * @param i x-coordinate of of the canvas when it is displayed on the screen
 * @param j Left limit of the canvas when it is displayed on the screen
 * 
 * @returns An array of characters with the information required
 *          to print the Canvas
 */
char * canv_StorePrint(Canvas* c, int i, int j);

/**
 * @brief Prints only the sections that are different between the new 
 *        and the old canvas
 * 
 * Note that for this function to work correctly, old should be the 
 * exact canvas that is now being displayed
 * 
 * @param f     File to which the canvas will be printed
 * @param new   New canvas
 * @param old   Previous canvas
 * @param oi    Vertical offset
 * @param oj    Horizontal offset
 * @return      A NULL pointer if there was an error. 
 *              canv otherwise
 */
Canvas* canv_printDiff(FILE* f,const Canvas* new,const Canvas* old,int oi, int oj);


/*-----------------------------------------------------------------*/
/**
 * @brief Prints the solid section of the canvas
 * 
 * Note that all pixel that are completely transparent will not be printed
 * The pixels that are partilly transparent will be overlaied on top
 * of the background
 * 
 * @param c     Canvas to be printed
 * @param backg Background canvas, it will be used when pixels are not completely opaque.
 *              It must have the same width and height
 * @param oi    Top row
 * @param oj    Left column
 */

void canv_printAllSolid(FILE* f, const Canvas* c,const Canvas* backg,int oi,int oj);

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the section that is not completely transparent
 *        of the canvas
 * 
 * Note that all pixel that not completely transparent will be printed
 * 
 * @param c     Canvas to be printed
 * @param oi    Top row
 * @param oj    Left column
 */
void canv_printAllNonTransparent(FILE* f, const Canvas* c,int oi,int oj);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the value of a specific pixel in the 
 *        canvas
 * 
 * @param c Canvas that contains the pixel that has to be changed
 * @param p New pixel
 * @param i I coordinate of the pixel in the canvas
 * @param j J coordinate of the pixel in the canvas
 * @return Canvas* 
 */
Canvas* canv_setPixel(Canvas* c,const Pixel* p, int i,int j);

/*-----------------------------------------------------------------*/
/**
 * @brief Returns a blurred version of the canvas
 * 
 * @param c     Canvas that will be blurred
 * @param rad   Radious of the blur effect
 * @return      New canvas with the blur applied
 */
Canvas* canv_blur(Canvas* c,int rad);

/*-----------------------------------------------------------------*/
/**
 * @brief Darkens the pixels in the canvas
 * It will not create a new canvas, the Canvas* return value
 * will only indicate wheter or not the process was successful
 * 
 * @param c     Canvas to be darken
 * @param light Percentage of light that the canvas will have 
 *              relative to its current brightness
 * @return  c if the process was successful
 *          NULL if there was any error
 */
Canvas* canv_darken(Canvas* c,double light);

/*-----------------------------------------------------------------*/
/**
 * @brief Applies a color filter to the Canvas
 * 
 * This filter works by reducing the values of some of the channels
 * of the pixels.
 * 
 * @param c     Canvas that will be filter
 * @param p     Color that will be applied in the filter
 * @return      A new canvas with the filter applied
 */
Canvas* canv_filter(Canvas* c,Pixel* p);

/*-----------------------------------------------------------------*/
/**
 * @brief Draws a circle of this color
 * 
 * @param p     The color that will fill the circle
 * @param rad   The radious of the requested circle
 * @return      New canvas containing the circle
 */
Canvas* canv_circle(Pixel* p,int rad);

/*-----------------------------------------------------------------*/
/// Returns the width of the canvas
int canv_getWidth(const Canvas* c);

/*-----------------------------------------------------------------*/
/// Returns the height of the canvas
int canv_getHeight(const Canvas* c);

/*-----------------------------------------------------------------*/
/// Returns the pixel of the canvas in the given position
const Pixel* canv_getPixel(const Canvas* c,int i,int j);
#endif /* Canvas_h */
