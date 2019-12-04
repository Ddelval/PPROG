//  PPROG
//	SpriteDic.h
//  Created by David del Val on 20/10/2019
//
/// This module is dictionary of sprites.
/// That is to say, we will use it to avoid fetching
/// the list of sprites from the file every time that
/// we want to add one.
/// If, in the future, the list of possible sprites to be
/// loaded got too long, we could just change this approach
/// to read the data from the file every time instead of
/// storing it on memory

#ifndef SpriteDic_h
#define SpriteDic_h

#include "Utility.h"
#include "Sprite.h"
/*-----------------------------------------------------------------*/
/// Free all the memory allocated to this structure.
/// It has to be called before the program exits
void sdic_free();

/*-----------------------------------------------------------------*/
/// Fetch the sprite with this id
Sprite* sdic_lookup(int id);

#endif /* SpriteDic_h */
