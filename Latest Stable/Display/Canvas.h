//  Test
//	Canvas.h
//  Created by David del Val on 27/09/2019
//
//

#ifndef Canvas_h
#define Canvas_h
#include "Pixel.h"
typedef struct _Canvas Canvas;
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

    
/*-----------------------------------------------------------------*/
/// Get a new canvas that contains both canvases appended vertically
/// 
/// @param north Canvas that will be in the northern region of the result
/// @param south Canvas that will be in the southern region of the result
Canvas* canv_appendV(const Canvas* north, const Canvas* south);


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
/// @param x1 x starting index (included)
/// @param x2 x ending   index (excluded)
/// @param y1 y starting index (included)
/// @param y2 y ending   index (excluded)
Canvas* canv_subCopy (const Canvas* bas,int x1,int x2,int y1,int y2);


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
/// @param o_x  The top limit of the foreground canvas
/// @param o_y  The left limit of the foreground canvas
///
/// @remark     (o_x,o_y) are the coordinates of the top-left corner of the foreground canvas
///             with respecto to the background canvas
///
/// @return A new canvas containing the merge of the other two
Canvas* canv_Overlay(const Canvas* base, const Canvas* over, int o_x, int o_y);


/*-----------------------------------------------------------------*/
/// Merge a new canvas in the base one
///
/// @param base The canvas that will act as background
/// @param over The canvas that will act as foreground
/// @param o_x  The top limit of the foreground canvas
/// @param o_y  The left limit of the foreground canvas
///
/// @remark     In this case, no new canvas is created,
///             the changes are applied to the background canvas.
Canvas* canv_addOverlay(Canvas* base, const Canvas* over, int o_x, int o_y);


/*-----------------------------------------------------------------*/
/// Print the canvas to the file
///
/// @param f    File in which the canvas will be printed
/// @param c    Canvas to be printed
/// @param x    Top limit of the canvas when it is displayed in the screen
/// @param y    Left limit of the canvas when it is displayed in the screen
///
/// @remark     The caller must know that the canvas will fit in the screen
void canv_print(FILE* f, const Canvas* c,int x,int y);


/*-----------------------------------------------------------------*/
/// Print an area of the canvas to a file
///
/// @param f    File in which the canvas will be printed
/// @param c    Canvas to be printed
/// @param x    Top limit of the canvas when it is displayed in the screen
/// @param y    Left limit of the canvas when it is displayed in the screen
/// @param wid  Maximum width that will be displayed
/// @param hei  Maximum height that will be displayed

void canv_printR(FILE* f, const Canvas* c,int x,int y,int wid,int hei);

#endif /* Canvas_h */
