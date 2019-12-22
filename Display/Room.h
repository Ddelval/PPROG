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
#include "TriggerDic.h"
#include "Trigger.h"
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






int room_modPos(Room* r, int index, int i, int j, bool scroll);

int room_incPos(Room* r, int index, int i, int j,bool scroll);
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
int room_addOSprite(Room* r, Sprite* s);

/*-----------------------------------------------------------------*/
/// Generates the canvas that represents the map
/// In the future, this function will return a subsection of the map
/// @param r Room to be rendered
Canvas* room_getRender(Room* r);

/**
 Some functions such as the ones that remove sprites from the layers
 are still being designed and the implementation is still not
 completely clear so they have not been included in this file */


Room* room_printMod(Room* r, int index,int disp_i, int disp_j);
Room* room_setBounds(Room*ro, int t, int l,int b,int r);
int room_scroll(Room* r, double i, double j);
Room* room_getBSpritePos(Room *r, int index, int* i, int *j);
Room* room_processTriggers(Room * r, Sprite * sp, int index);
Trigger** room_getTriggers(Room*r,trig_type tt, int sp_index, int* siz);
Room* room_removeB(Room* r, int index);
Room* room_printModBackg(Room* r, int disp_i, int disp_j);
Room* room_buildingInterface(Room*r, int spid,int ai, int aj,int room_i, int room_j);
Room* room_setHW(Room* r, int he,int wi);
char* room_getName(Room* r);
int room_getId(Room* r);
Room* room_setSpriteJ(Room* r,int index, int j);
Room* room_setSpriteI(Room* r,int index, int i);
Room* room_processAlly(Room* r, void* e, Sprite* s,int ally_index, int rad);
#endif /* Room_h */
