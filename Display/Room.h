//  ppro
//	Room.h
//  Created by David del Val on 02/10/2019
//
//

#ifndef Room_h
#define Room_h

#include "Utility.h"
#include "Canvas.h"
#include "Sprite.h"
#include "SpriteDic.h"
typedef struct _Room Room;
Room* room_ini(int id, char* name,int hei, int wid, Pixel* backcol);
void room_free(Room* r);
Room* room_addBSprite(Room* r, Sprite*s);
Room* room_addOSprite(Room* r, Sprite*s);
Canvas* room_getRender(Room* r);
#endif /* Room_h */
