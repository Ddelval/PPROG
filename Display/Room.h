//  PPROG
//	Room.h
//  Created by David del Val on 02/10/2019
//
/// Description:
/// The Room ADT is designed to hold the information relative to the map
/// and the sprites that we are placing on top of it

#ifndef Room_h
#define Room_h

#include "Utility.h"
#include "Canvas.h"
#include "Sprite.h"
#include "SpriteDic.h"
typedef struct _Room Room;
/*-----------------------------------------------------------------*/
/// Creates a new room with a solid background
/// @param name     Name of the room
/// @param hei      Height of the map
/// @param wid      Width of the map
/// @param backcol  Background color
Room* room_ini(int id, char* name,int hei, int wid, Pixel* backcol);

/*-----------------------------------------------------------------*/
/// Free the memory allocated for the room
void room_free(Room* r);

/*-----------------------------------------------------------------*/
/// This function adds a sprite to the background of the map.
/// Objects that are not supposed to move should be aded to this
/// background layer. That includes trees, buildings,...
/// @param r    Room in which the sprite will be added
/// @param s    Sprite to add
Room* room_addBSprite(Room* r, Sprite*s);

/*-----------------------------------------------------------------*/

/// Add a sprite to the foreground layer.
/// This layer is used for sprites that change frequently
/// @param r    Room in which the sprite will be added
/// @param s    Sprite to be added
Room* room_addOSprite(Room* r, Sprite*s);

/*-----------------------------------------------------------------*/
/// Generates the canvas that represents the map
/// In the future, this function will return a subsection of the map
/// @param r Room to be rendered
Canvas* room_getRender(Room* r);

/**
 Some functions such as the ones that remove sprites from the layers
 are still being designed and the implementation is still not
 completely clear so they have not been included in this file */

#endif /* Room_h */
