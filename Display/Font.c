//  Test
//	Font.c
//  Created by David del Val on 28/09/2019
//
//

#include "Font.h"
#define MAX_CHARS 256
struct _Font{
    Character** data;
    char *id;
    int hei;
    int wid;
    int size;
};
Font* font_ini(int hei, int wid, int size){
    Font* f = calloc(1, sizeof(Font));
    if(!f)return NULL;
    f->hei=hei;
    f->wid=wid;
    f->size=size;
    f->data= calloc(MAX_CHARS, sizeof(Character*));
    if(!f->data){
        free(f);
        return NULL;
    }
    return f;
}

/// Loads a font from a file
/// <name_length> <name>
/// <height> <width> <size>
/// characters a,b,...z, A,...Z, 1..., 9
/// Note: The caracters have to be rendered with a white font over a transparent background
/// @param f file with the font
/*Font* font_load(FILE* fil){
    int l;
    fscanf(fil,"%d",&l);
    char* id= calloc(l, sizeof(char));
    int he,wi,si;
    fscanf(fil,"%d %d %d",&he,&wi,&si);
    Canvas* whole=canv_load(fil);
}
*/
