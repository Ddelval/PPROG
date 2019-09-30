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


Character* cha_ini(int hei, int wid, char c, Canvas* di){
    Character* d= calloc(1,sizeof(Character));
    if(!d)return NULL;
    d->wid=wid;
    d->hei=hei;
    d->c=c;
    d->dat=di;
    return d;
}
Character* cha_copy(Character* c){
    if(!c)return NULL;
    return cha_ini(c->hei, c->wid, c->c, c->dat);
}
void cha_free(Character * c){
    if(!c)return;
    if(c->dat)canv_free(c->dat);
    free(c);
}
Canvas * cha_getCanvas(Character * c){
    if(c&&c->dat)return canv_copy(c->dat);
    return NULL;
}
