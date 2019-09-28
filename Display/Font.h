//  Test
//	Font.h
//  Created by David del Val on 28/09/2019
//
//

#ifndef Font_h
#define Font_h
#include "Character.h"
#include <stdio.h>
typedef struct _Font Font;
Font* font_load(FILE* fil);
Canvas* font_renderText(Font* f,char* txt);
#endif /* Font_h */
