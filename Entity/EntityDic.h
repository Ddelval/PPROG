//  PPROG
//	SpriteDic.h
//  Created by David del Val on 20/10/2019
//
/// This module is dictionary of entities.
/// That is to say, we will use it to avoid fetching
/// the list of entities from the file every time that
/// we want to add one.
/// If, in the future, the list of possible entities to be
/// loaded got too long, we could just change this approach
/// to read the data from the file every time instead of
/// storing it on memory

#ifndef EntityDic_h
#define EntityDic_h

#include "Utility.h"
#include "Entity.h"
#include "Trigger.h"
/*-----------------------------------------------------------------*/
/// Free all the memory allocated to this structure.
/// It has to be called before the program exits
void edic_free();

/*-----------------------------------------------------------------*/
/// Fetch the entity with this id
Entity* edic_lookup(int id,Display* dis);

#endif /* EntityDic_h */
