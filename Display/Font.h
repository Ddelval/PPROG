//  Test
//	Font.h
//  Created by David del Val on 28/09/2019
//
//

#ifndef Font_h
#define Font_h
#include "Character.h"
#include <stdio.h>
typedef struct _Font Font;


/*-----------------------------------------------------------------*/
/// Initalizes the font with these values
/// @param hei          Height of each character
/// @param wid          Widht of the characters
/// @param size         Amount of elements that will be stored
/// @param spacing      The gap left between each letter
/// @param whitespace   The gap left when a ' ' is encountered
Font* font_ini(int hei, int wid, int size, int spacing, int whitespace);


/*-----------------------------------------------------------------*/
/// Frees the memory allocated by this font
void font_free(Font* f);


/*-----------------------------------------------------------------*/
/// Loads a font from a file. The format should be:
///
/// <name_length> <name> <size> <width> <height> <spacing> <whitespace>
/// <List of the characters included>, one after the other
/// Canvas with all the characters one after the other
///
/// @param fil file with the font
///
/// @remark The caracters have to be rendered with a white font over a transparent background
Font* font_load(FILE* fil);


/*-----------------------------------------------------------------*/
/// Calculate the width that a
/// given string needs to be displayed with a font
int font_calcWidth(const Font* f,char* txt);


/*-----------------------------------------------------------------*/
/// Get the height of the font
int font_getHeight(const Font* f);


/*-----------------------------------------------------------------*/
/// Renders the given string in a canvas with the given font
/// @param f    Font to be used in the render
/// @param txt  String to be rendered
Canvas* font_renderText(const Font* f,char* txt);

Font* font_copy(const Font*f);

#endif /* Font_h */
