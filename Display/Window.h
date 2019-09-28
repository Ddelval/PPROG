/*	Window.h	*/

#ifndef Window_h
#define Window_h
#include "Welem.h"

typedef struct _Window Window;

Window* win_ini(char* title, Welem** Win_elem, int wid, int hei);
void win_free(Window* win);
Window* win_redraw(Window* win, int wid, int hei);

Window* win_setSelected(Window* win, int* selected_elem);
Welem* win_getSelected(Window* win);

Window* win_scrollDown(Window* win);
Window* win_scrollUp(Window* win);

Window* win_copy(Window* win);

#endif
