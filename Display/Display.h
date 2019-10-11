//  ppro
//	Display.h
//  Created by David del Val on 02/10/2019
//
//

#ifndef Display_h
#define Display_h

#include "Utility.h"
#include "Room.h"
#include "Window.h"

typedef struct _Display Display;
Display* disp_ini(int wid, int hei, Room* room);
void disp_free(Display* dat);
Display* disp_AddLWindow(Display*dis, Window* w);
Display* disp_RemLwindow(Display* dis, int index);
Display* disp_SetPopup(Display* dis, Window* p);
void disp_RemPopup(Display* dis);
#endif /* Display_h */
