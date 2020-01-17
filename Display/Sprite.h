//  PPROG
//	Sprite.h
//  Created by David del Val on 06/10/2019
//

/// Description:
/// This ADT holds the graphical representation of objects.
/// It can also store their position on the screen if they
/// are placed on it.
/// Furthermore, it stores all the data relative to collisions
/// and shadows.
/// We consider shadows the areas in which moving sprites
/// such as the player or other entities would not be drawn
/// because of another object in front of them.
/// It will also contain the triggers, which are the codes of actions
/// that will happen when someone steps on that position

#ifndef Sprite_h
#define Sprite_h
#define SPR_NTRIGGERS 15
#include "Utility.h"
#include "Trigger.h"
#include "Canvas.h"
typedef struct _Sprite Sprite;

/*-----------------------------------------------------------------*/
/**
 * @brief Initialises a sprite
 * 
 * All the memory will be allocated, however,
 * the canvas will still be NULL
 * 
 * @param id        Id of the new sprite
 * @param width     Width of the new sprite
 * @param height    Height of the sprite
 * @return          NULL if error
 *                  The new sprite otherwise 
 */
Sprite* spr_ini(int id, int width, int height);


/*-----------------------------------------------------------------*/
/**
 * @brief Copies the sprite
 * 
 * @param spr   Source sprite
 * @return      Copy of the sprite
 */
Sprite* spr_copy(const Sprite* spr);


/*-----------------------------------------------------------------*/
/**
 * @brief Loads a sprite from the file
 * 
 * The format is the following:
 * <id> <format>
 * <canvas>
 * 
 * If format is 1, the file will also contain:
 * <collision_canvas>
 * 
 * If format is 0, the collision canvas will 
 * be the canvas.
 * 
 * If format is 2, the collision and shadow
 * will be empty 
 * 
 * @param f 
 * @return Sprite* 
 */
Sprite* spr_load(FILE* f);


/*-----------------------------------------------------------------*/
/**
 * @brief Frees the memory allocated for the sprite
 * 
 * @param sp Sprite to be freed
 */
void spr_free(Sprite* sp);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a trigger to the sprite
 * 
 * @param s       Sprite to which the trigger will be added
 * @param tr_id   Id of the trigger that will be stored in
 *                the trigger
 * @param i1      Horizontal starting position
 * @param i2      Horizontal ending position
 * @param j1      Vertical starting position
 * @param j2      Vertical ending position
 * @return        NULL if error
 */
Sprite* spr_addTrigger(Sprite* s, int tr_id, int i1, int i2, int j1, int j2);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds the collition data of the sprite to rarr
 * 
 * @param s     Sprite whose collition data will be added to arr
 * @param rarr  Array of collision data in the room
 * @param rwid  Height of the room
 * @param rhei  Width of the room
 * @return      NULL if error
 */
Sprite* spr_processCollisions(const Sprite* s,bool** rarr,int rwid, int rhei);

/*-----------------------------------------------------------------*/
/**
 * @brief Adds the shadow data of the sprite to shad
 * 
 * @param s     Sprite whose shadow data will be added to shad
 * @param shad  Canvas with the shadow data
 * @return      NULL if error
 */
Sprite* spr_processShadows(const Sprite* s,Canvas* shad);

/*-----------------------------------------------------------------*/
/**
 * @brief Checks if the moving the sprite to the new position will
 *        make it collide with another object
 * 
 * @param s     Sprite that we are moving
 * @param rarr  Array of collision data of the room
 * @param rwid  Width of the room
 * @param rhei  Height of the room
 * @param ni    New horizontal position of the sprite
 * @param nj    New vertical position of the sprite
 * @return      -1 if error
 *               0 if there is no collition
 *               1 if there is a  collition 
 */
int spr_checkCollisions(const Sprite*s, const bool**rarr,int rwid,int rhei, int ni,int nj);

/*-----------------------------------------------------------------*/
/**
 * @brief Deletes all the triggers in the sprite
 * 
 * @param sp Sprite whose triggers will be deleted
 */
void spr_clearTrig(Sprite* sp);


/*-----------------------------------------------------------------*/
/** GETTERS **/
const Canvas* spr_getDispData(const Sprite* spr);

int spr_getId(const Sprite* sp);

int spr_getOI(const Sprite* spr);

int spr_getOJ(const Sprite* spr);

int spr_getWidth(const Sprite* spr);

int spr_getHeight(const Sprite* spr);

int spr_getTriginfo(const Sprite*s, int** tr_id, int** i1,int** i2, int** j1, int** j2);


/*-----------------------------------------------------------------*/
/** SETTERS **/

void spr_setCoordinates(Sprite* sp,int i,int j);

void spr_setOI( Sprite* spr,int ipos);

void spr_setOJ(Sprite* spr, int jpos);

void spr_setId(Sprite* sp,int id);
#endif /* Sprite_h */
