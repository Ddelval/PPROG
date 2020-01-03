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
#define OBJ_TYPE_SIZE 3
typedef enum {WEAPON, CONSUMABLE, RESOURCE} obj_type;
#define OBJ_MAX_ATTACKS 4

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
int obj_cmp(const Object* ob1, const Object* ob2);

Sprite* obj_getIcon(Object* ob);
Object* obj_copy(Object* ob);
Sprite* obj_getSprite(Object* ob);

const char* obj_getName(Object* ob);
Attributes * obj_getAttributes(Object * ob);
obj_type obj_getType(const Object* ob);
Canvas* obj_render(const Object* ob, int number, const Font* ftext, const Font* fnum, int h,int w,bool selected);
int obj_getId(Object* ob);
Object* obj_renderDims(const Object* ob, int number, const Font* ftext, const Font* fnum,int*h,int*w);
Skill ** obj_getAttacks(Object * ob);

bool obj_toplace(Object* o);

#endif
