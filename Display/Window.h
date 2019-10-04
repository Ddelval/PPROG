/*	Window.h	*/

#ifndef Window_h
#define Window_h
#include "../Utility/Utility.h"
#include "Canvas.h"
#include "Wlabel.h"
#include "Welem.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _Window Window;

Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int weight, int ypos, int xpos);
void win_free(Window* win);
Window* win_render(Window* win, int pos);
Window* win_redraw(Window* win, int wid, int hei, int weight, int x, int y);

Window* win_setSelected(Window* win, int* selected_elem);
Welem** win_getSelected(Window* win);

Window* win_scrollDown(Window* win);
Window* win_scrollUp(Window* win);

Window* win_copy(Window* win);

Window* win_setMargins(Window *win, int lm, int rm, int tm, int bm);
int* win_getMargins(Window *win);

#endif
