/*	Window.h	*/

#ifndef Window_h
#define Window_h
#include "Utility.h"
#include "Wlabel.h"
#include "Welem.h"
#include "Trigger.h"

typedef struct _Window Window;

/**
 * @brief 	The Window is a graphical structure that contains several 
 * 			selectable elements stacked vertically
 * 
 * A window may contain just labels or labels with icons (Labic)
 * The foreground color will be used for the selected elements while 
 * the background one will be used to render the rest
 */


/*----------------------------------------------------------------*/
/**
 * @brief Initalizes the window
 * 
 * This function will alocate all the memory for the window and
 * it will copy the Welems to the window structure 
 * 
 * @param title 	Text for the title of the screen
 * @param Win_elem 	Array with the elements of the window
 * @param num_elems Number of elements in the window
 * @param wid 		Width of the window
 * @param hei 		Height of the window
 * @param jpos 		J-coordinate of the top left corner
 * @param ipos 		I-coordinate of the top left corner
 * @param titlef 	Font for the title of the window
 * @return 			New window
 */
Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int jpos, int ipos, const Font* titlef);

/*-----------------------------------------------------------------*/
/**
 * @brief Frees all the memory allocated to this window 
 * 
 * @param win 	Window whose memory will be freed
 * @return 		Always NULL
 */
Window* win_free(Window* win);

/*-----------------------------------------------------------------*/
/**
 * @brief Copies the window
 * 
 * @param win 	Window to be copied
 * @return 		A copy of the window
 */
Window* win_copy(Window* win);

/*-----------------------------------------------------------------*/
/**
 * @brief Appends a Welem to the Window
 * 
 * @param win 	Window to which the element will be added
 * @param we 	Element that will be added
 * @return 		NULL in case of error
 */
Window* win_addWindowElement(Window* win, Welem* we);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds several elements to the window
 * 
 * @param w		Window to which the elements will be added
 * @param we 	Array of Welems
 * @param n 	Number of elements to add
 * @return 		NULL if error
 */
Window* win_addWelems(Window* w, Welem** we, int n);

/*-----------------------------------------------------------------*/
/**
 * @brief Removes an element from the window
 * 
 * @param win  	Window in which the element is
 * @param index Position of the element in the window
 * @return 		NULL in case of error
 */
Window* win_remWindowElement(Window* win, int index);

/*-----------------------------------------------------------------*/
/**
 * @brief Eliminates all the elements in the window
 * 
 * @param w Window whose elements will be deleted
 * @return 	NULL if error
 */
Window* win_clear(Window* w);

/*-----------------------------------------------------------------*/
/**
 * @brief Attaches an action to the element in position index
 * 
 * Actions are defined by a function and the type of trigger that
 * the function takes.
 * With this definition, we will be able to check for the triggers
 * that are stored in the map (Room) in a given position and
 * apply the function to them
 * 
 * @param win 	Window in wich the window element is
 * @param f 	Function that will be attached to the Welem
 * @param index Index of the element in the window
 * @param t 	Type of the trigger accepted by f
 * @return 		NULL in case of error
 */
Window* win_addAction(Window* win,func_trig f, int index,tr_type t);
/*-----------------------------------------------------------------*/
/**
 * @brief 	Gets the function that has to be executed when the 
 * 			selected element is "pressed"
 * 
 * @param win 	Window that has been selected
 * @return 		func_trig associated to the selected Welem
 */
func_trig win_getSelectedAction(Window* win);
/*-----------------------------------------------------------------*/
/**
 * @brief Gets the function that has to be executed when a given
 * 	      element of the window is "pressed"
 * 
 * @param win 	Window that contains the Welem
 * @param index Index of the element in the window
 * @return 		func_trig associated to the Welem
 */

func_trig win_getAction(Window* win, int index);
/*-----------------------------------------------------------------*/
/**
 * @brief Gets the type of the trigger that is attached to the 
 * 		  selected Welem
 * 
 * @param win 	Window that contains the Welem
 * @return 		tr_type associated to the Welem 
 */
tr_type win_getSelectedTrigType(Window * win);
/*-----------------------------------------------------------------*/
/**
 * @brief Renders the window
 * 
 * @param win 	Window to be rendered
 * @return 		Canvas* containing the graphical representation of 
 * 				the window
 */
Canvas* win_render(Window* win);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the index of the selected element in the window
 * 
 * @param win  Window whose selected element will be modified
 * @param selected_elem Index of the new selected element
 * @return 	NULL in case of error
 */
Window* win_setSelected(Window* win, int selected_elem);

/*-----------------------------------------------------------------*/
/**
 * @brief Increments the index of the selected element
 * 
 * @param win  	Window whose selected element will be modified
 * @param incr 	Increment of the selected element index
 * @return 		NULL in case of error
 */
Window* win_incrementSelected(Window* win, int incr);

/*-----------------------------------------------------------------*/
/**
 * @brief Returns a copy of the selected element	
 * 
 * @param win 	Window from which the element will be copied
 * @return 		New Welem identical to the selected one in win
 */
Welem* win_getSelected(Window* win);

/*-----------------------------------------------------------------*/
/**
 * @brief Returns the index of the selected element in the window
 * 
 * @param win 	Window whose selected element is being requested
 * @return 		Index of the selected element 
 */
int win_getSelectedIndex(Window* win);

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window down
 * 
 * Note that the canges will only be noticeable when the window
 * is rendered again
 * 
 * @param win 	Window that will be scrolled down
 * @return 		NULL if there is an error
 */
Window* win_scrollDown(Window* win);

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window up
 * 
 * Note that the canges will only be noticeable when the window
 * is rendered again
 * 
 * @param win 	Window that will be scrolled up
 * @return 		NULL if there is an error
 */
Window* win_scrollUp(Window* win);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the margins in the window
 * 
 * When the window is rendered, the margins will be transparent
 * 
 * @param win Window whose margins will be set
 * @param lm Left 	margin
 * @param rm Right 	margin
 * @param tm Top 	margin
 * @param bm Bottom margin
 * @return	 NULL if there was an error
 */
Window* win_setMargins(Window *win, int lm, int rm, int tm, int bm);

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the margins of the window
 * 
 * The order will be: top, right, bottom, left
 * 
 * @param win 	Window whose margins we are requesting
 * @return 		Pointer to an array of four integers containing the
 * 				size of the margins
 */
int* win_getMargins(Window *win);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a border to the window
 * 
 * 
 * @param win 	Window that will get the border
 * @param color Color of the border
 * @param width Vertical width of the border 
 * 				(the horizontal will be double this ammount)
 * @return 		NULL if error
 */
Window* win_addBorder(Window* win,Pixel* color, int width);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the background color of the window 
 * 
 * @param win 		Window whose background color will be set
 * @param backcol 	Color for the background
 * @return 			NULL if error
 */
Window* win_setBackColor(Window *win, Pixel* backcol);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the foreground color of the window 
 * 		  (color for the selected elements)
 * 
 * @param win 		Window whose foreground color will be set
 * @param forecol 	Color for the foreground
 * @return 			NULL if error
 */
Window* win_setForeColor(Window *win, Pixel* forecol);

/*-----------------------------------------------------------------*/
/// Return the background color
Pixel* win_getBackColor(Window *win);

/*-----------------------------------------------------------------*/
/// Return the foreground color
Pixel* win_getForeColor(Window *win);

/*-----------------------------------------------------------------*/
/// Return the widht of the canvas
int win_getWidth(const Window* w);

/*-----------------------------------------------------------------*/
/// Return the height of the canvas
int win_getHeight(const Window* w);


#endif
