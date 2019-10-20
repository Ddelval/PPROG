//  ppro
//	Display.h
//  Created by David del Val on 02/10/2019
//

/// Description:
/// The Display ADT is meant to be the data structure that
/// encapsulates all the information regarding the graphics.
/// It contains the room in which we will be moving the player,
/// the Windows that will be shown in the right part of the screen,
/// and the pop-up windows that may contain dialog or other menus.

#ifndef Display_h
#define Display_h

#include "Utility.h"
#include "Room.h"
#include "Window.h"

typedef struct _Display Display;
/*-----------------------------------------------------------------*/
/// <#Description#>
/// @param wid <#wid description#>
/// @param hei <#hei description#>
/// @param room <#room description#>
Display* disp_ini(int wid, int hei, Room* room);
void disp_free(Display* dat);
Display* disp_AddLWindow(Display*dis, Window* w);
Display* disp_RemLwindow(Display* dis, int index);
Display* disp_SetPopup(Display* dis, Window* p);
void disp_RemPopup(Display* dis);
#endif /* Display_h */
