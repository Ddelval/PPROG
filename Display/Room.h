//  PPROG
//	Room.h
//  Created by David del Val on 02/10/2019
//
/// Description:
/// The Room ADT is designed to hold the information relative to the map
/// and the sprites that we are placing on top of it

#ifndef Room_h
#define Room_h

#include "Utility.h"
#include "Canvas.h"
#include "Sprite.h"
#include "SpriteDic.h"
#include "TriggerDic.h"
#include "Trigger.h"
typedef struct _Room Room;


/*-----------------------------------------------------------------*/
/**
 * @brief Allocates all the memory required for the new room
 * 
 * @param id      Id of the room
 * @param name    Name of the room
 * @param hei     Total height of the room
 * @param wid     Total width of the room
 * @param backcol Background color of the room
 * @return        New room
 */
Room* room_ini(int id, char* name,int hei, int wid, Pixel* backcol);

/*-----------------------------------------------------------------*/
/**
 * @brief Loads a room from a file
 * 
 * The format of the room should be:
 * <id> <length of Name> <name>
 * <height> <width>
 * <background color> (as a Pixel)
 * <number of background sprites>
 * 
 * For each sprite:
 * <id> <ipos> <jpos>
 * 
 * @param f File from where the room will be read
 * @return  New room containing all the data
 */
Room* room_load(FILE* f);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a sprite to the background layer
 * 
 * This is the function that should be called when we want to add
 * a sprite that does not proceed from an entity
 * 
 * @param r Room in which the sprite will be added
 * @param s Sprite to be added
 * @return  NULL in case of error
 */
Room* room_addBSprite(Room* r, Sprite* s);

/*-----------------------------------------------------------------*/
/**
 * @brief Add a sprite to the top layer
 * 
 * This is the function that should be called to add sprites that
 * come from entities. In doing so, they will be able to interact
 * with the player in a different way (talk and combat)
 * 
 * @param r Room in which the sprite will be added
 * @param s Sprite to add
 * @return  -1 in case of error
 *          Otherwise, the index of the sprite in r->overs after 
 *          it has been added  
 */
int room_addOSprite(Room* r, Sprite* s);

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the position of a background sprite
 * 
 * @param r     Room in which the sprite is
 * @param index Position of the sprite in r->backg
 * @param i     Pointer to a variable where the i-coordinate will be
 *              stored
 * @param j     Pointer to a variable where the j-coordinate will be
 *              stored
 * @return      NULL in case of error
 */
Room* room_getBSpritePos(Room *r, int index, int* i, int *j);

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the room
 * 
 * Note that this function will not redraw the base layer
 * 
 * @param r Room to be rendered
 * @return Graphical representation of the room
 */
Canvas* room_getRender(Room* r);

/*-----------------------------------------------------------------*/
/**
 * @brief Modifies the position of a sprite of the top layer
 * 
 * Note that this function will change the position if possible 
 * and will renturn a value that represents the current position
 * of the sprite.
 * 
 * 
 * 
 * @param r      Room in which the sprite is
 * @param index  Index of the sprite in r->overs
 * @param i      New i-coordinate of the top left corner of the sprite
 * @param j      New j-coordinate of the top left corner of the sprite
 * @param scroll Whether or not the movement of this sprite should 
 *               affect the scrolling of the room
 * @return       Integer code:
 *               -1: error
 *                0: the position is completely free
 *                1: the sprite is in contact with the top border
 *                2: the sprite is in contact with the right border
 *                3: the sprite is in contact with the bottom border
 *                4: the sprite is in contact with the left border
 *                5: the sprite cannot move because of a collision 
 */
int room_modPos(Room* r, int index, int i, int j, bool scroll);

/*-----------------------------------------------------------------*/
/**
 * @brief Increments the position of a sprite of the top layer
 * 
 * Note that this function will change the position if possible 
 * and will renturn a value that represents the current position
 * of the sprite.
 * 
 * 
 * 
 * @param r      Room in which the sprite is
 * @param index  Index of the sprite in r->overs
 * @param i      Increment in the i axis
 * @param j      Increment in the j axis
 * @param scroll Whether or not the movement of this sprite should 
 *               affect the scrolling of the room
 * @return       Integer code:
 *               -1: error
 *                0: the position is completely free
 *                1: the sprite is in contact with the top border
 *                2: the sprite is in contact with the right border
 *                3: the sprite is in contact with the bottom border
 *                4: the sprite is in contact with the left border
 *                5: the sprite cannot move because of a collision 
 */
int room_incPos(Room* r, int index, int i, int j,bool scroll);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the are of the window that will be rendered when 
 *        the rendering functions are called
 * 
 * This is used when the player moves to another section of the map
 * or when the room is set up initially.
 * 
 * @param ro Room
 * @param t  Top limit,      included
 * @param l  Left limit,     included 
 * @param b  Bottom limit,   excluded
 * @param r  Right limit,    excluded
 * @return   NULL if error, the given room otherwise 
 */
Room* room_setBounds(Room*ro, int t, int l, int b, int r);

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the modifications to the room, after the position 
 *        of an entity has changed
 * 
 * @param r      Room to be updated
 * @param index  Index of the sprite in overs that has changed
 * @param disp_i Vertical offset of the room (generally 0)
 * @param disp_j Horizontal offset of the room (generally 0)
 * @return Room* 
 */
Room* room_printMod(Room* r, int index, int disp_i, int disp_j);

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window by a factor of (i,j)
 * 
 * Usually the factors are (-1 <= i <= 1)
 * 
 * @param r Room to be scrolled
 * @param i Horizontal factor 
 * @param j Vertical factor
 * @return  -1 if there was an error
 *           0 if there was no scrolling to be done
 *           1 if the room scrolled
 */
int room_scroll(Room* r, double i, double j);

/*-----------------------------------------------------------------*/
/**
 * @brief Frees the memory allocated by the room
 * 
 * @param r Room to be freed
 */
void room_free(Room* r);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds the trigges of sp to the room
 * 
 * @param r     Room to which the triggers are added
 * @param sp    Sprite that contains the triggers
 * @param index Index of sp in the array overs
 * @return      NULL in case of error
 */
Room* room_processTriggers(Room * r, Sprite * sp, int index);

/*-----------------------------------------------------------------*/
/**
 * @brief Get the trigers in position (i,j)
 * 
 * @param r   Room that contains the triggers
 * @param tt  Type of triggers that we are searching
 * @param i   I--coordinate
 * @param j   J-coordinate
 * @param siz Pointer to a variable where the size will of the
 *            resultin array will be stored
 * @return Trigger** 
 */
Trigger** _room_getTriggersLoc(Room*r,trig_type tt, int i, int j, int* siz);

/*-----------------------------------------------------------------*/
/**
 * @brief Triggers associated to the position of the sprite
 *        r->overs[sp_index]
 * 
 * @param r         Room that contains the sprite
 * @param tt        Type of trigger that we are searching for
 * @param sp_index  Index of the sprite in the array overs
 * @param siz       Pointer to a variable where the size will of the
 *                  resultin array will be stored
 * @return Trigger** 
 */
Trigger** room_getTriggers(Room*r,trig_type tt, int sp_index, int* siz);

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the map again
 * 
 * @param r Room to be recalculated
 * @return  NULL if error
 */
Canvas* room_redrawMap(Room*r);

/*-----------------------------------------------------------------*/
/**
 * @brief Updates all the data in room after the deletion of a 
 *        sprite
 * 
 * @param r Room to be updated
 * @return  NULL if error
 */
Room* room_updateData(Room*r);

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the changes to the background after deletion of
 *        a sprite
 * 
 * @param r      Room to be updated
 * @param disp_i Horizontal offset of the screen
 * @param disp_j Vertical offset of the screen
 * @return       NULL if error
 */
Room* room_printModBackg(Room* r, int disp_i, int disp_j);

/*-----------------------------------------------------------------*/
/**
 * @brief Removes the sprite in position index from r->backg
 * 
 * @param r     Room to be updated
 * @param index Index of the sprite to be removed
 * @return      NULL if error
 */
Room* room_removeB(Room* r, int index);

/*-----------------------------------------------------------------*/
/**
 * @brief Displays the building interface
 * 
 * @param r      Room in which the building will be placed
 * @param spid   Sprite id of the building
 * @param ai     Initial vertical coordinate of the sprite
 * @param aj     Initial horizontal coordinate of the sprite
 * @param room_i Horizontal offset of the room
 * @param room_j Vertical offset of the room
 * @return       NULL if error
 */
Room* room_buildingInterface(Room*r, int spid,int ai, int aj,int room_i, int room_j);

/*-----------------------------------------------------------------*/
/// Set height and width of the display area
Room* room_setHW(Room* r, int he,int wi);

/*-----------------------------------------------------------------*/
/**
 * @brief Modifies the room when an ally has been added
 * 
 * @param r            Room to be updated 
 * @param e            Entity that represents the ally
 * @param s            Sprite of the ally
 * @param ally_index   Index of the ally in the sprite array
 * @param rad          Radius of the interactions with the player
 * @return             NULL if error
 */
Room* room_processAlly(Room* r, void *e,Sprite* s,int ally_index, int rad);

/*-----------------------------------------------------------------*/
///Gets the name of the room
char* room_getName(Room* r);

/*-----------------------------------------------------------------*/
/// Gets the id of the room
int room_getId(Room* r);

/*-----------------------------------------------------------------*/
/// Gets the i-coordinate of the sprite r->overs[index]
Room* room_setSpriteI(Room* r,int index, int i);

/*-----------------------------------------------------------------*/
/// Gets the j-coordinate of the sprite r->overs[index]
Room* room_setSpriteJ(Room* r,int index, int j)

#endif /* Room_h */
