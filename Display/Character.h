//  Test
//	Character.h
//  Created by David del Val on 28/09/2019
//
//

#ifndef Character_h
#define Character_h

#include <stdio.h>
#include "Canvas.h"
typedef struct _Character Character;

Character* cha_ini(int hei, int wid, char c, Canvas* di);
Character* cha_copy(Character* c);
void cha_free(Character * c);
Canvas * cha_getCanvas(Character * c);



#endif /* Character_h */
