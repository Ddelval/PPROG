//  PPROG
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
#include "Entity.h"
#include "Inventory.h"

typedef struct _Display Display;
/*-----------------------------------------------------------------*/
/// Create a new Display object
/// @param wid  Total width of the display
/// @param hei  Total height of the display
/// @param room Room (map) which we will be using
Display* disp_ini(int wid, int hei, Room* room, int vdiv,char* tit, const Font* titf);

/*-----------------------------------------------------------------*/
/// Free the memory allocated for this display
void disp_free(Display* dat);

/*-----------------------------------------------------------------*/
/// Add a new window to the vertical stack of windows on the right
/// section of the display
/// @param dis  Display object in which the element will be added
/// @param w    New window to add
Display* disp_AddLWindow(Display*dis, Window* w);

/*-----------------------------------------------------------------*/
/// Remove the window in position index of the array of windows
/// that are displayed on the right column
/// @param dis Display object from which the window will be removed
/// @param index Index of the window to be removed
Display* disp_RemLwindow(Display* dis, int index);

/*-----------------------------------------------------------------*/
/// Add a pop-up window that will be displayed on top of the map
/// and the right column of windows
/// @param dis  Display object in which this window will be added
/// @param p    Window to be set as pop-up
Display* disp_SetPopup(Display* dis, Window* p);

/*-----------------------------------------------------------------*/
/// Remove the pop-up window from the display
/// @param dis Display whose pop-up we want to remove
void disp_RemPopup(Display* dis);

Display* disp_incSelIndex(Display* dis, int winIndex, int increment);

Display* print_Window(Display*dis, int index);


Canvas* disp_Render(Display* dis);
int disp_scroll(Display* dis,double i,double j);
Display* disp_DiaglogWindow(Display* dis, char * txt,const Font* f);

Room* disp_getrefRoom(Display* dis);
int disp_incPos(Display* d,int index, int i, int j, int* f_i, int *f_j);
Display* disp_InventoryWindow(Display* dis, Inventory* inv, Font* ftitle, Font* fsubtitle, Font* ftext, Font* fnumbers);
Display* disp_execute(Display* dis, int index, Entity* en);
#endif /* Display_h */
