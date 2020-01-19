//  Created by David del Val on 05/12/2019
//  PPROG
//	Object.h
//
//
#ifndef OBJECT_H
#define OBJECT_H

#include "Utility.h"
#include "Attributes.h"
#include "SpriteDic.h"
#include "Sprite.h"
#include "Welem.h"
#include "Skill.h"
#include "Font.h"
typedef struct _Object Object;
#define OBJ_TYPE_SIZE 4
typedef enum {WEAPON, CONSUMABLE, RESOURCE, BUILDING} obj_type;
#define OBJ_MAX_ATTACKS 4

char** obj_type_def();
/**
 * @brief Creates and arranges memory for an object structure
 *
 * @param object Object structure pointer
 * @return  NULL if there has been any error
 *          Inventory pointer if everything was ok
 *           
 */
Object* obj_ini();
/**
 * @brief Frees an object structure
 *
 * @param object Object structure pointer
 * @return  void
 *           
 */
void obj_free(Object* ob);
/**
 * @brief Loads an object from a file
 * The format is the following:
 * id
 * name
 * icon_id
 * sprite_id
 * type
 * attributes
 * number of attacks
 * attacks
 * destroyable
 * @param f File from where the object will be read
 * @return Object* New object with the data
 */
Object* obj_load(FILE* f);
/**
 * @brief Compares two objects
 *
 * @param ob1 First object to be compared
 * @param ob2 Second object to be compared
 * @return int  >0 if ob1>ob2
 *              =0 if ob1=ob2
 *              <0 if ob1<ob2
 */
int obj_cmp(const Object* ob1, const Object* ob2);

/**
 * @brief Object getters and setters
 *
 * @param ob Object pointer
ç
 * @return NULL if there was an error
 *         The desired element from the structure
 */
Sprite* obj_getIcon(Object* ob);

Object* obj_copy(const Object* ob);

Sprite* obj_getSprite(Object* ob);

obj_type obj_getType(const Object* ob);

const char* obj_getName(const Object* ob);

const Attributes * obj_getAttributesRef(const Object * ob);

int obj_getId(Object* ob);

Skill ** obj_getAttacks(Object * ob);


/**
 * @brief Returns the formed canvas of an object
 *
 * @param object Object structure pointer
 * @param integer number
 * @param font pointer ftext
 * @param font pointer fnum
 * @param integer height
 * @param integer width
 * @param bool selected
 * @return  NULL if there has been any error
 *          Canvas  pointer if everything was ok
 *           
 */

Canvas* obj_render(const Object* ob, int number, const Font* ftext, const Font* fnum, int h,int w,bool selected);

/**
 * @brief Returns the object with its attached dims created
 *
 * @param object Object structure pointer
 * @param integer number
 * @param font pointer ftext
 * @param font pointer fnum
 * @param integer pointer height
 * @param integer pounter width
 * @return  NULL if there has been any error
 *          Object  pointer if everything was ok
 *           
 */

Object* obj_renderDims(const Object* ob, int number, const Font* ftext, const Font* fnum,int*h,int*w);

bool obj_toplace(Object* o);

#endif
