//  Test
//	Canvas.h
//  Created by David del Val on 27/09/2019
//
//

#ifndef Canvas_h
#define Canvas_h
#include "Pixel.h"
typedef struct _Canvas Canvas;

/// Create an empty canvas with the especified width and height
/// @param wid Width of the canvas
/// @param hei Height of the canvas
Canvas* canv_ini(int wid,int hei);

/// Read a canvas from a file.
/// The format has to be the following:
/// <width> <height>
/// [r,g,b,a] [r,g,b,a]...
///
/// @param f File from which the data will be read
Canvas* canv_load(FILE* f);

/// Free the allocated memory
void canv_free(Canvas* c);

/// Create the array of strings that contain the ansi scape sequences
/// required to display the colors on the screen.
/// @param c Canvas to be processed
/// @param wid width of the canvas that will be rendered
/// @param hei height of the canvas that will be rendered
/// @remark If wid or hei are less than the dimensions of the canvas,
/// only the area defined by wid and hei will be rendered
char** canv_render(Canvas* c,int wid, int hei);

void canv_print(FILE* f,Canvas* c,int x,int y);

void canv_printR(FILE* f,Canvas* c,int x,int y,int wid,int hei);

//Places over on top of base and returns the result. It does not modify either of the arguments
Canvas* canv_Overlay(Canvas* base, Canvas* over, int o_x, int o_y);

//It places over on top of base, modifying base
Canvas* canv_addOverlay(Canvas* base, Canvas* over, int o_x, int o_y);

//Copies the entire canvas
Canvas* canv_copy (Canvas* bas);

/// Returns a copy of a section of the given Canvas
/// @param bas Source canvas
/// @param x1 x starting index (included)
/// @param x2 x ending   index (excluded)
/// @param y1 y starting index (included)
/// @param y2 y ending   index (excluded)
Canvas* canv_subCopy (Canvas* bas,int x1,int x2,int y1,int y2);

//Checks if a column is transparent
bool _transparentColumn(Canvas * c, int j);

//Splits the canvas in several canvases using transparent columns as delimiters
Canvas ** canv_VSplit(Canvas* src, int* nelem);

//If the given canvas is bigger than the dimensions given, it will crop it keeping it centered
//If it is smaller, it will add transparent rows and columns to keep the content centered
Canvas* canv_AdjustCrop(Canvas* src, int nwidth,int nheight);

Canvas* canv_backGrnd(int r, int g, int b, int a, int width, int height);
#endif /* Canvas_h */
