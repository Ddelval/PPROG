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
#include "Inventory.h"
#include "Window.h"
#include "RecipeDic.h"
#include "FontCatalog.h"
#include "DialogMan.h"

#define W_DATA 0
#define H_DATA 1
#define VD_DATA 2

typedef struct _Display  Display;
/**
 * @brief Allocates all the memory required for a new Display ADT
 * 
 * @param wid   Width of the new display
 * @param hei   Height of the new display
 * @param room  Room that the display will have on its left half
 * @param vdiv  Position of the vertical dividor between both halves
 * @param tit   Title of the display
 * @param titf  Font for the tile
 * @return Display* 
 */
Display* disp_ini(int wid, int hei, Room* room, int vdiv,char* tit, const Font* titf);

/*-----------------------------------------------------------------*/
/**
 * @brief Frees all the memory allocated for this window
 * 
 * @param dat Window to free
 */
void disp_free(Display* dat);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a window to the right column
 * 
 * @param dis   Display where the window will be added.
 * @param w     Window to add to the display
 * @return      NULL if error
 */
Display* disp_AddLWindow(Display*dis, Window* w);

/*-----------------------------------------------------------------*/
/**
 * @brief Removes a window from the right column
 * 
 * @param dis       Display from which the window 
 * @param index     Position of the window that will be removed
 * @return          NULL if error
 */
Display* disp_RemLwindow(Display* dis, int index);

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the display by rendering all its components
 *        
 * 
 * @param dis    Display to be redered
 * @return       A new Canvas containing the graphical 
 *               representation of the display
 */
Canvas* disp_Render(Display* dis);

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window associated with this display
 * 
 * @param dis Display whose window has to be scrolled
 * @param i   Horizontal percentage of the room that will
 *            be scrolled
 * @param j   Vertical percentage of the room that will
 *            be scrolled
 *          
 * @return  -1 if there was an error
 *           0 if there was no scrolling to be done
 *           1 if the room scrolled
 */
int disp_scroll(Display* dis,double i,double j);

/*-----------------------------------------------------------------*/
/**
 * @brief Prints an updated render of the window in the <index>
 *        position of the right column. 
 * 
 * This function will calculate the position that this window will
 * have on the screen and print the new render of the window on
 * that position
 * 
 * @param dis   Display whose window will be rendered
 * @param index Position of the window in the right column
 * @return Display* 
 */
Display* print_Window(Display*dis, int index);


/*-----------------------------------------------------------------*/
/**
 * @brief Enters the dialog mode of the display
 * 
 * When this function is called, it will take control
 * of the entire game. 
 * Key presses will mean advancing the dialog except 'Q' or 'E',
 * those keys will end the dialog mode and return to 
 * the usual gameplay.
 * 
 * 
 * @param dis   Display 
 * @param dman 
 * @param ename 
 * @return Display* 
 */
Display* disp_DialogWindow(Display* dis, DialogMan* dman, char * ename);

/*-----------------------------------------------------------------*/
/**
 * @brief Enters the inventory mode of this display
 * 
 * When this function is called, the inventory window 
 * will appear and it will take control of the input. 
 * 
 * This mode will allow the player to
 * 
 * @param dis       Display that will enter the inventory mode
 * @param inv       Inventory that will be displayed
 * @param ftitle    Font for the "Inventory" text
 * @param fsubtitle Font for the title of sections
 * @param ftext     Font for the name of items
 * @param fnumbers  Font for the amount of items
 * @return          NULL if error
 */
Display* disp_InventoryWindow(Display* dis, Inventory* inv, Font* ftitle, Font* fsubtitle, Font* ftext, Font* fnumbers);


Display* disp_InventoryConsumWindow(Display* dis, Inventory* inv, Font* ftitle, Font* fsubtitle, Font* ftext, Font* fnumbers);

/*-----------------------------------------------------------------*/
/**
 * @brief Starts the crafting mode of the display
 * 
 * In this mode, the user will only be able to change
 * the selection until a recipe is crafted or the user
 * exits the mode
 * 
 * In the display, all the recipies that can be crafted
 * will be shown.
 * 
 * @param dis  Display whose crafting mode will be activated
 * @param inv  Inventory that determines whether or not the 
 *             player can craft a recipe
 * @return     NULL if error
 */
Display* disp_CraftingWindow(Display* dis,Inventory* inv);

/*-----------------------------------------------------------------*/
/**
 * @brief Opens a window to choose between several triggers
 * 
 * @param dis   Display to which the window will be attached
 * @param f     Function that will process the selected trigger
 * @param dat   Array of the triggers that can be selected
 * @param siz   Size of the array
 * @return      The selected index when the window closes
 *              -1 if there was any error
 */
int disp_chooseWindow(Display* dis, func_trig f, Trigger** dat, int siz);


/*-----------------------------------------------------------------*/
/**
 * @brief Opens the window that displays the quests.
 * 
 * @param dis       Display to which the window will be attached
 * @param amount    Number of quests to display
 * @param quests    Quests to display
 * @return          NULL if error
 */
Display* disp_QuestWindow(Display* dis, int amount, Quest** quests);

/*-----------------------------------------------------------------*/
/**
 * @brief Finds the selected element in the window that is in the 
 *        index position and executes the action associated with it
 * 
 * @param dis           Display that contains the window that
 *                      will execute the action
 * @param index         Index of the window in the display that
 *                      will execute the action
 * @param room_index    Index of the sprite in the window
 * @param en            Entity associated with the sprite
 * @return              NULL if error 
 */
Display* disp_execute(Display* dis, int index, int room_index, void* en);


/*-----------------------------------------------------------------*/
/**
 * @brief Sets the horizontal coordinate of the sprite in the
 *        position ind of the overs array of the room.
 * 
 * @param d     Display whose room we will modify
 * @param ind   Position of the sprite in the overs array
 * @param i     New i coordinate
 * @return Display* 
 */
Display* disp_setSpriteI(Display* d,int ind, int i);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the vertical coordinate of the sprite in the
 *        position ind of the overs array of the room.
 * 
 * @param d     Display whose room we will modify
 * @param ind   Position of the sprite in the overs array
 * @param i     New j coordinate
 * @return Display* 
 */
Display* disp_setSpriteJ(Display* d,int ind, int j);

/*-----------------------------------------------------------------*/
/**
 * @brief Increments the position of a sprite
 * 
 * @param d      Display in which we want to modify the spite
 * @param index  Index of the sprite in the room
 * @param i      Horizontal increment of the position of the sprite
 * @param j      Vertical increment of the position of the sprite
 * @param f_i    Variable in which the new horizontal coordinate will
 *               be stored
 * @param f_j    Variable in which the new horizontal coordinate will
 *               be stored
 * @param scroll Whether or not the room will screen if necessary
 * @return       -1 if error
 *                0 if there was no error
 */
int disp_incPos(Display* d,int index, int i, int j, int* f_i, int *f_j, bool scroll);

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the lateral window in position windex
 * 
 * @param d         Display that contains the window
 * @param windex    Index of the window in the display
 * @return          NULL if error
 */
 Window* disp_getLWindow(Display* d, int windex);

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the amount of windows on the right column
 * 
 * @param d     Display given
 * @return      -1 if error
 *              number of windows otherwise
 */
int disp_getNLatWindow(Display* d);

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the dimensions of the window.
 * 
 * These macros should be used when accessing this array:
 * W_DATA  -> width
 * H_DATA  -> height 
 * VD_DATA -> vertical divisor
 * 
 * @param d     Display whose dimensions will be returned
 * @return      Array with the dimensions in this order:
 *                  width
 *                  height
 *                  vertical divisor
 *  
 */
int* disp_getDimensions(Display* d);

/*-----------------------------------------------------------------*/
/**
 * @brief Returns a reference to the room of the display
 * 
 * @param dis   Display whose room we obtain
 * @return      Pointer to the room of the display.
 *              It should never be freed
 */
Room* disp_getrefRoom(Display* dis);

/*-----------------------------------------------------------------*/
/**
 * @brief Moves the selection focus of a window in the right column
 *        increment positions down
 * 
 * @param dis       Display whose selection will be changed
 * @param winIndex  Index of the window whose selection will be changed
 * @param increment Number of positions that the focus will go down
 *                  A negative number to make the focus go up
 * @return          NULL if error
 */
Display* disp_incSelIndex(Display* dis, int winIndex, int increment);

/*-----------------------------------------------------------------*/
/**
 * @brief Moves the selection focus of a window in the right column
 *        to selIndex
 * 
 * @param dis       Display whose selection will be changed
 * @param winIndex  Index of the window whose selection will be changed
 * @param selIndes  New selection element for the window
 * @return          NULL if error
 */
Display* disp_setSelIndex(Display* dis, int winIndex, int selIndex);

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the select index of a window in the right 
 *        column of the display
 * 
 * @param dis       Display in which the window is 
 * @param winIndex  Index of the window whose selected index will
 *                  be returned
 * @return          -1 if error 
 */
int disp_getSelIndex(Display* dis, int winIndex);

#endif /* Display_h */
