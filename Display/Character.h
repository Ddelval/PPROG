//  PPROG
//	Character.h
//  Created by David del Val on 28/09/2019
//

/// Description:
/// The Character ADT stores a character of a Font. It keeps the
/// dimensions of it, the Canvas that represents its graphical
/// representation and the ASCII code of the character represented.

#ifndef Character_h
#define Character_h

#include <stdio.h>
#include "Canvas.h"
typedef struct _Character Character;

/*-----------------------------------------------------------------*/
/// Creates a new character
/// Note that this function does NOT make a copy of the canvas,
/// so this canvas should not be freed ever.
/// @param hei 	Height of the caracter, in pixels
/// @param wid 	Width of the carachter, in pixels
/// @param c 	char represented by this Character
/// @param di 	Canvas that contains the draw of the character
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
