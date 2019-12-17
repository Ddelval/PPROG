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


/// Create an empty canvas with the especified width and height
/// Note that the pixel pointers are still not allocated 
///
/// @param wid Width of the canvas
/// @param hei Height of the canvas
Canvas* canv_ini(int wid,int hei);


/*-----------------------------------------------------------------*/
/// Read a canvas from a file.
/// The format has to be the following:
/// <width> <height>
/// [r,g,b,a] [r,g,b,a]...
///
/// @param f File from which the data will be read

Canvas* canv_load(FILE* f);


/*-----------------------------------------------------------------*/
/// Generate a canvas of a given color and dimensions
///
/// @param r        red component of this background
/// @param g        green component
/// @param b        blue component
/// @param a        alpha component. 255=opaque, 0=transparent
/// @param width    width of the generated canvas
/// @param height   height of the generated canvas
Canvas* canv_backGrnd(int r, int g, int b, int a, int width, int height);


/*-----------------------------------------------------------------*/
/// Get a new canvas that contains both canvases appended horizontally
///
/// @param west Canvas that will be in the western region of the result
/// @param east Canvas that will be in the eastern region of the result
Canvas* canv_appendH(const Canvas* west, const Canvas* east);

/**
 * @brief Second version of canv_appendH that does not allign both canvases
 * 
 * The return of this function is a canvas that contains both of the canvases
 * appended horizontally and aligned with their top limit. 
 * 
 * @param west Canvas that will be in the western region of the result
 * @param east Canvas that will be in the eastern region of the result
 * @return Canvas* 
 */
Canvas* canv_appendHNL(const Canvas* west, const Canvas* east);
/*-----------------------------------------------------------------*/
/// Get a new canvas that contains both canvases appended vertically
/// 
/// @param north Canvas that will be in the northern region of the result
/// @param south Canvas that will be in the southern region of the result
Canvas* canv_appendV(const Canvas* north, const Canvas* south);


/*-----------------------------------------------------------------*/
/// Append the south Canvas below the North canvas
///
/// @param north Canvas that will be in the northern region of the result
/// @param south Canvas that will be in the southern region of the result
/// @returns     A not NULL value as long as the process has been
///              completed successfully
Canvas* canv_appendVI(Canvas* north, const Canvas* south);


/*-----------------------------------------------------------------*/
/// Get a new canvas that contains both canvases appended horizontally
///
/// @param west  Canvas that will be in the western region of the result
/// @param east  Canvas that will be in the eastern region of the result
/// @returns     A not NULL value as long as the process has been
///              completed successfully
Canvas* canv_appendHI(Canvas* west, const Canvas* east);


/*-----------------------------------------------------------------*/
/// Returns a new Canvas that contains the original one and the margins defined in the input parameters
Canvas* canv_addMargin (const Canvas *src, int top, int right, int bottom, int left);


/*-----------------------------------------------------------------*/
/// Return a copy of the given canvas
///
/// @param bas Canvas to be coppied
Canvas* canv_copy (const Canvas* bas);


/*-----------------------------------------------------------------*/
/// Split a canvas into smaller sub-canvases making vertical separations
/// @param src      Canvas to be splitted
/// @param nelem    Pointer to an integer where the number
///                 of returned elements will be stored
///
/// @return   An array of Canvas* that contains all non-transparent
///           sub-sections of the given canvas separated.
Canvas ** canv_VSplit(const Canvas* src, int* nelem);


/*-----------------------------------------------------------------*/
/// Returns a copy of a section of the given Canvas
///
/// @param bas Source canvas
/// @param i1 i starting index (included)
/// @param i2 i ending   index (excluded)
/// @param j1 j starting index (included)
/// @param j2 j ending   index (excluded)
Canvas* canv_subCopy (const Canvas* bas,int i1,int i2,int j1,int j2);


/*-----------------------------------------------------------------*/
/// Returns a copy of a canvas adjusted to a given dimensions.
/// If the canvas is smaller, it will be centered and the rest
/// of the pixels will be transparent
/// If the canvas is bigger, it will be centered and cropped
///
/// @param src Original canvas
/// @param nwidth new width value
/// @param nheight new height value
Canvas* canv_AdjustCrop(const Canvas* src, int nwidth,int nheight);


/*-----------------------------------------------------------------*/
/// Merge two canvases into a new canvas
///
/// @param base The canvas that will act as background
/// @param over The canvas that will act as foreground
/// @param o_i  The top limit of the foreground canvas
/// @param o_j  The left limit of the foreground canvas
///
/// @remark     (o_x,o_y) are the coordinates of the top-left corner of the foreground canvas
///             with respecto to the background canvas
///
/// @return A new canvas containing the merge of the other two
Canvas* canv_Overlay(const Canvas* base, const Canvas* over, int o_i, int o_j);


/*-----------------------------------------------------------------*/
/// Merge a new canvas in the base one
///
/// @param base The canvas that will act as background
/// @param over The canvas that will act as foreground
/// @param o_i  The top limit of the foreground canvas
/// @param o_j  The left limit of the foreground canvas
///
/// @remark     In this case, no new canvas is created,
///             the changes are applied to the background canvas.
Canvas* canv_addOverlay(Canvas* base, const Canvas* over, int o_i, int o_j);


/*-----------------------------------------------------------------*/
/// Print the canvas to the file
///
/// @param f    File in which the canvas will be printed
/// @param c    Canvas to be printed
/// @param i    Top limit of the canvas when it is displayed in the screen
/// @param j    Left limit of the canvas when it is displayed in the screen
///
/// @remark     The caller must know that the canvas will fit in the screen
void canv_print(FILE* f, Canvas* c,int i,int j);

/**
 * @brief Prepares the printing of the canvas
 * 
 * This function will render the canvas and store in a string all
 * the characters that are requried to print this canvas.
 * This includes all the characters used to move the cursor around and 
 * to print the pixels.
 * 
 * @param c Canvas to be processed
 * @param i Position of the top of the canvas in the screen
 * @param j Position of the left of the canvas in the screen 
 * @return char* 
 */
char * canv_StorePrint(Canvas* c, int i, int j);

/**
 * @brief Prints only the differences between two canvases
 * 
 * This function operates in exactly the same way as canvas_print but
 * it will only print the differences between both canvases
 * @param f     File where the canvas will be printed
 * @param new   New canvas
 * @param old 
 * @param oi 
 * @param oj 
 * @return Canvas* 
 */
Canvas* canv_printDiff(FILE* f, const Canvas* new,const Canvas* old,int oi, int oj);
/*-----------------------------------------------------------------*/
/// Print an area of the canvas to a file
///
/// @param f    File in which the canvas will be printed
/// @param c    Canvas to be printed
/// @param i    Top limit of the canvas when it is displayed in the screen
/// @param j    Left limit of the canvas when it is displayed in the screen
/// @param wid  Maximum width that will be displayed
/// @param hei  Maximum height that will be displayed

void canv_printR(FILE* f, const Canvas* c,int i,int j,int wid,int hei);


/// Return the widht of the canvas
int canv_getWidth(const Canvas* c);


/// Return the height of the canvas
int canv_getHeight(const Canvas* c);


/// Allows the access to a particular pixel stored in a Canvas.
/// @param c Canvas that contains the pixel that is going to be accesed
/// @param i Row of the pixel
/// @param j Column of the pixel
/// @return  Reference to the pixel in the canvas
const Pixel* canv_getPixel(const Canvas* c,int i,int j);




Canvas* canv_setPixel(Canvas* c,const Pixel* p, int i,int j);
Canvas* canv_blur(Canvas* c,int rad);
Canvas* canv_darken(Canvas* c,double light);
void canv_printSolid(FILE* f, const Canvas* c,const Canvas* backg,int oi,int oj);
void canv_printAllNonTransparent(FILE* f, const Canvas* c,const Canvas* backg,int oi,int oj);
Canvas* canv_filter(Canvas* c,Pixel* p);
Canvas* canv_circle(Pixel* p,int rad);
Canvas* canv_appendVIA(Canvas* north,const Canvas* south,CAlign al);
#endif /* Canvas_h */
