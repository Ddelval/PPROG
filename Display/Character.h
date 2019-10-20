//  Test
//	Character.h
//  Created by David del Val on 28/09/2019
//
//

#ifndef Character_h
#define Character_h

#include <stdio.h>
#include "Canvas.h"
typedef struct _Character Character;

/*-----------------------------------------------------------------*/
/// Creates a new character
/// @param hei 	Height of the caracter, in pixels
/// @param wid 	Width of the carachter, in pixels
/// @param c 	char represented by this Character
/// @param di 	Canvas that contains the draw of the character
/// Note that this function does NOT make a copy of the canvas, so this canvas should
/// not be freed ever.
Character* cha_ini(int hei, int wid, char c, Canvas* di);

/*-----------------------------------------------------------------*/
/// Creates a new character eqal to the given one
/// @param c Character whose values will be copied
Character* cha_copy(Character* c);

/*-----------------------------------------------------------------*/
/// Frees the memory allocated to this character
/// @param c Character to be freed
void cha_free(Character * c);

/*-----------------------------------------------------------------*/
/// Returns the canvas stored in this Character
/// @param c Character whose canvas we want to retrieve
Canvas * cha_getCanvas(Character * c);



#endif /* Character_h */
