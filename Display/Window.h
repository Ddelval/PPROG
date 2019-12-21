/*	Window.h	*/

/// Description:
/// The window is an UI element that has a title and contains
/// several different window elements stacked vertically one after
/// the other. It will probably include some sort of scrolling.
/// Therefore, the height dimension will only be used to specify
/// the part of the window that will be visible.

#ifndef Window_h
#define Window_h
#include "../Utility/Utility.h"
#include "Canvas.h"
#include "Wlabel.h"
#include "Welem.h"
#include "Trigger.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _Window Window;

/*-----------------------------------------------------------------*/
/// Create a new window
/// @param title        Text that will appear the top, centered
/// @param Win_elem     Elements contained in the window
/// @param num_elems    Amoun of Welem that are given to this constuctor
/// @param wid          Width of the window
/// @param hei          Height of the portion that will be rendered
/// @param jpos         Column of the top-left corner of thewindow
///                     on the screen
/// @param ipos         Row of the top-left corner of thewindow
///                     on the screen
Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int jpos, int ipos, const Font* titlef);


/*-----------------------------------------------------------------*/
/// Free the window
void win_free(Window* win);

/*-----------------------------------------------------------------*/
/// Add a new window element to the Window.
/// There is no limit in the amount of elmenets that a window can
/// store. As more elements are added the allocated space will increase
/// @param win  Window in which the element has to be added
/// @param we   Element to be added
Window* win_addWindowElement(Window* win, Welem* we);

/*-----------------------------------------------------------------*/
/// Render the window and prints it in the correct position on the
/// screen
/// @param win Window to be processed
Canvas* win_render(Window* win);

/*-----------------------------------------------------------------*/
/// Render the window modifying the parameters before doing so
/// @param win      Window to be renderd
/// @param wid      New width value
/// @param hei      New height value
/// @param i        New top limit
/// @param j        New left limit
Window* win_redraw(Window* win, int wid, int hei, int i, int j);

/*-----------------------------------------------------------------*/
/// Set the focus on an element
/// @param win           Window that contains
/// @param selected_elem Index of the element that has to be selected
Window* win_setSelected(Window* win, int selected_elem);

/*-----------------------------------------------------------------*/
/// Increment the focused element
/// @param win           Window that contains
/// @param incr          Increment of the selection count
Window* win_incrementSelected(Window* win, int selected_elem);

/*-----------------------------------------------------------------*/
/// Returns the index of the selected element
Welem* win_getSelected(Window* win);


Window* win_remWindowElement(Window* win, int index);


/*-----------------------------------------------------------------*/
/// Returns the index of the selected element, or -1 if nothing is
/// selected or in case of error
int win_getSelectedIndex(Window* win);

/*-----------------------------------------------------------------*/
/// Moves the scroll position to the next section of the window
Window* win_scrollDown(Window* win);
/*-----------------------------------------------------------------*/
/// Moves the scroll position to the previous section of the window
Window* win_scrollUp(Window* win);

/*-----------------------------------------------------------------*/
/// Creates a copy of the window with the same data
Window* win_copy(Window* win);

/*-----------------------------------------------------------------*/
/// Sets the padding for the window. The elements of the window
/// will be drawn in the area defined by the dimensions of the window
/// minus the margins
/// @param win  The window
/// @param lm   Left margin for the window
/// @param rm   Right margin for the window
/// @param tm   Top margin for the window
/// @param bm   Bottom margin for the window
Window* win_setMargins(Window *win, int lm, int rm, int tm, int bm);
/*-----------------------------------------------------------------*/

/// Returns the margins of a given window.
/// An array with four integers will be returned in the following
/// order: top, right, bottom, left
/// @param win Window whose margins are being fetched
int* win_getMargins(Window *win);

/// Returns the width of a given window.
/// Will return a negative number if error
/// @param w Window whose margins are being fetched
int win_getWidth(const Window* w);


/// Returns the height of a given window.
/// Will return a negative number if error
/// @param w Window whose margins are being fetched
int win_getHeight(const Window* w);

Window* win_setBackColor(Window *win, Pixel* backcol);

Window* win_setForeColor(Window *win, Pixel* forecol);

Pixel* win_getBackColor(Window *win);

Pixel* win_getForeColor(Window *win);

Window* win_addAction(Window* win,func_trig f, int index, trig_type t);

func_trig win_getAction(Window* win, int index);

func_trig win_getSelectedAction(Window* win);

trig_type win_getSelectedTrigType(Window * win);

#endif
