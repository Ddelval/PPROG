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

Sprite* spr_ini(int id, int width, int height);

void spr_setCoordinates(Sprite* sp,int i,int j);

void spr_free(Sprite* sp);

Sprite* spr_copy(const Sprite* spr);

/**
 File format:
 <id> <format> (0 if the collision canvas is the canvas, 1 otherwise)
 <Canvas>
 <Collision_canvas>
 <nº triggers>
 <list of triggers>
 The format of a trigger has to be: i1, i2, j1, j2 id
 where i1 is the initial row, i2 is the last row,
 j1 is the first row, j2 is the last row
 (All included)

 */
Sprite* spr_load(FILE* f);

Sprite* spr_addTrigger(Sprite* s, int tr_id, int i1, int i2, int j1, int j2);


Sprite* spr_processCollisions(const Sprite* s,bool** rarr,int rwid, int rhei);

int spr_checkCollisions(const Sprite*s, const bool**rarr,int rwid,int rhei, int ni,int nj);


Sprite* spr_processShadows(const Sprite* s,Canvas* shad);

const Canvas* spr_getDispData(const Sprite* spr);

int spr_getOI(const Sprite* spr);

int spr_getOJ(const Sprite* spr);

int spr_getWidth(const Sprite* spr);

int spr_getHeight(const Sprite* spr);

void spr_setOI( Sprite* spr,int ipos);

void spr_setOJ(Sprite* spr, int jpos);

int spr_getId(const Sprite* sp);

int spr_getTriginfo(const Sprite*s, int** tr_id, int** i1,int** i2, int** j1, int** j2);

#endif /* Sprite_h */
