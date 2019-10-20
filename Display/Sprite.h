//  PPROG
//	Sprite.h
//  Created by David del Val on 06/10/2019
//

/// Description:
/// This ADT holds the graphical representation of objects.
/// It can also store their position on the screen if they
/// are placed on it.
/// Furthermore, it stores all the data relative to collisions
/// and shadows.
/// We consider shadows the areas in which moving sprites
/// such as the player or other entities would not be drawn
/// because of another object in front of them.
/// It will also contain the triggers, which are the codes of actions
/// that will happen when someone steps on that position

#ifndef Sprite_h
#define Sprite_h

#include "Utility.h"
#include "Canvas.h"
typedef struct _Sprite Sprite;

/*-----------------------------------------------------------------*/
/// Creates a new sprite
/// @param id       Numerical identifier
/// @param width    Width of the sprite
/// @param height   Height of the sprite
Sprite* spr_ini(int id, int width, int height);

/*-----------------------------------------------------------------*/
/// Free all the memory allocated to this sprite
void spr_free(Sprite* sp);

/*-----------------------------------------------------------------*/
/// Create a copy of the Sprite
Sprite* spr_copy(const Sprite* spr);

/*-----------------------------------------------------------------*/
///Load the sprite from a file
/// File format:
/// id format (0 if the collision canvas is the canvas, 1 otherwise)
/// Canvas
/// Collision_canvas>
/// nº triggers
/// list of triggers
/// The format of a trigger has to be: i1, i2, j1, j2
/// where i1 is the initial row, i2 is the last row,
/// j1 is the first row, j2 is the last row
/// (All included)
Sprite* spr_load(FILE* f);

/*-----------------------------------------------------------------*/
/// Return a reference to the canvas that represents this sprite
/// @param spr Sprite whose canvas we are fetching
const Canvas* spr_getDispData(Sprite* spr);

/*-----------------------------------------------------------------*/
/// Set the point in the screen in which this sprite is going to
/// be printed
/// @param sp Sprite to be modified
/// @param i Top limit of its representation
/// @param j Left limit of its representation
void spr_setCoordinates(Sprite* sp,int i,int j);

/*-----------------------------------------------------------------*/
/// Get the row of the top left corner of the sprite
int spr_getOI(Sprite* spr);

/*-----------------------------------------------------------------*/
/// Get the column of the top left corner of the sprite
int spr_getOJ(Sprite* spr);

/*-----------------------------------------------------------------*/
/// Set the row of the top left corner of the sprite
void spr_setOJ(Sprite* spr, int jpos);

/*-----------------------------------------------------------------*/
/// Set the column of the top left corner of the sprite
void spr_setOI(Sprite* spr,int ipos);

/*-----------------------------------------------------------------*/
/// Get the id of the sprite
int spr_getId(const Sprite* sp);
#endif /* Sprite_h */
