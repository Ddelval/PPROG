//  PPROG
//	Object.h
//  Created by David del Val on 05/12/2019
//
//
#ifndef OBJECT_H
#define OBJECT_H

#include "Utility.h"
#include "Attributes.h"
#include "SpriteDic.h"
#include "Sprite.h"
#include "Skill.h"
#include "Window.h"
typedef struct _Object Object;
#define OBJ_TYPE_SIZE 2
typedef enum {WEAPON, CONSUMABLE} obj_type;

char** obj_type_def();

Object* obj_ini();

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
int obj_cmp(Object* ob1, Object* ob2);

Sprite* obj_getIcon(Object* ob);
Object* obj_copy(Object* ob);
Sprite* obj_getSprite(Object* ob);

char* obj_getName(Object* ob);
obj_type obj_getType(Object* ob);
Canvas* obj_render(Object* ob, int number,Font* ftext, Font* fnum);

#endif