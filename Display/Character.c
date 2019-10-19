//  Test
//	Character.c
//  Created by David del Val on 28/09/2019
//
//

#include "Character.h"

struct _Character{
    Canvas * dat;
    int wid, hei;
    int filled;
    char c;
};

/*-----------------------------------------------------------------*/
/// Creates a new character
/// @param hei 	Height of the caracter, in pixels
/// @param wid 	Width of the carachter, in pixels
/// @param c 	char represented by this Character
/// @param di 	Canvas that contains the draw of the character
/// Note that this function does NOT make a copy of the canvas, so this canvas should
/// not be freed ever.
Character* cha_ini(int hei, int wid, char c, Canvas* di){
    Character* d= calloc(1,sizeof(Character));
    if(!d)return NULL;
    d->wid=wid;
    d->hei=hei;
    d->c=c;
    d->dat=di;
    return d;
}

/*-----------------------------------------------------------------*/
/// Creates a new character eqal to the given one
/// @param c Character whose values will be copied
Character* cha_copy(Character* c){
    if(!c)return NULL;
    return cha_ini(c->hei, c->wid, c->c, c->dat);
}

/*-----------------------------------------------------------------*/
/// Frees the memory allocated to this character
/// @param c Character to be freed
void cha_free(Character * c){
    if(!c)return;
    if(c->dat)canv_free(c->dat);
    free(c);
}

/*-----------------------------------------------------------------*/
/// Returns the canvas stored in this Character
/// @param c Character whose canvas we want to retrieve
Canvas * cha_getCanvas(Character * c){
    if(c&&c->dat)return canv_copy(c->dat);
    return NULL;
}
