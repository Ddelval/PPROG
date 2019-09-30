/*	Window.h	*/

#ifndef Window_h
#define Window_h
#include "Welem.h"

typedef struct _Window Window;

Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int weight);
void win_free(Window* win);
Window* win_redraw(Window* win, int wid, int hei, int weight);

Window* win_setSelected(Window* win, int* selected_elem);
Welem** win_getSelected(Window* win);

Window* win_scrollDown(Window* win);
Window* win_scrollUp(Window* win);

Window* win_copy(Window* win);

#endif
