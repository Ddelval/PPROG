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



/// Render a portion of the window
/// Since usually we will not be able to show the entire map in the screen we will have to crop
/// it and show only a section of it at a time.
/// @param r           Room to be rendered
/// @param i           First row that will be rendered
/// @param j           First column that will be rendered
/// @param wid      Width of the portion of the window that will be rendered
/// @param hei      Height of the portion of the window that will be rendered
/// @return The canvas that results from rendering the selected section of the room
Canvas* room_getSubRender(Room* r, int i, int j, int wid, int hei);


/// Prints the modifications that have been made to the sprites stored in the top layer since
/// the last time that the room was rendered.
/// It should be called after the position of a sprite in the top layer is modified
/// The first two arguments refer to the position of the top left corner of the graphical representation
/// of the room in the display.
/// The next four arguments will describe the subsection of the room that is being displayed
/// @param r               Room that we want to display
/// @param disp_i    First row that the section of the room that is displayed occupies on the screen
/// @param disp_j    First column that the section of the room that is displayed occupies on the screen
/// @param i               First row of the room that is displayed
/// @param j               First column of the room that is displayed
/// @param wid          Width of the displayed portion of the room
/// @param hei          Height of the displayed portion of the room
Room* room_printMod(Room* r,int disp_i, int disp_j);


/// Modifies the position of a sprite in the top layer
/// @param r            Room in which the sprites are
/// @param index   Index of the spritre in the array of sprites in the top layer
/// @param i            New row index for the sprite
/// @param j            New column index for the sprite
int room_modPos(Room* r, int index, int i, int j);

int room_incPos(Room* r, int index, int i, int j);
Room* room_load(FILE* f);

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



Room* room_setBounds(Room*ro, int t, int l,int b,int r);
int room_scroll(Room* r, double i, double j);
#endif /* Room_h */
