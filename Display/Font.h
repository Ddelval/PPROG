//  PPROG
//	Font.h
//  Created by David del Val on 28/09/2019
//

/// Description:
/// The Font ADT holds the description of a font. It stores all the
/// characters that can be displayed on the screen. Additionally, it
/// provides all the required functionality to load this information
/// from a file and to render text to the screen. That is to say,
/// to convert an array of chars to a canvas that can be printed.

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
/// name_length  name   size   width   height   spacing   whitespace
/// List of chars (l)
/// Canvas with all the characters(c)
///
/// This elements represent:
/// name_length: the length of the name field of the font.
/// name:           Actual name of the font
/// size:           Amount of characters that will be described in
///                 the file. Therefore it is the amount of different
///                 characters that can be printed to the screen using
///                 this font
/// width:          Width of a single character (in pixels)
/// height:         Height of a line (in pixels)
/// spacing:        Space left between two characters (in pixels)
/// whitespace:     Space left when a whitespace is encountered
/// l               String with all the chars. E.g. abcde
/// c               Canvas with all the caracters. They have to be
///                 rendered with a black font over a transparent
///                 background
/// @param      fil file with the font
/// @return     New font with all the data loaded
///
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


/*-----------------------------------------------------------------*/
/// Creates a copy of a font.
/// @param f Font to be copied
Font* font_copy(const Font*f);

#endif /* Font_h */
