//  PPROG
//	SpriteDic.c
//  Created by David del Val on 05/12/2019
//
//
#include "Trigger.h"

struct _Trigger
{
    int id;
    trig_type typ;
    /* Talking */
    int entity_id;

    /* Enable*/
    int spr_id;
    bool remove_spr;
    int object_id;
};


Trigger* tr_ini(){
    Trigger * tr=calloc(1,sizeof(Trigger));
    if(!tr)return NULL;
    tr->id=tr->spr_id=tr->entity_id=tr->object_id=-1;
    return tr;
}
Trigger* tr_free(Trigger* tr){
    free(tr);
}

Trigger* tr_load(FILE* f){
    Trigger * t=tr_ini();
    fscanf(f,"%d %d",&t->id,&t->typ);
    if(t->typ=ALLOW){
        fscanf("%d %d",&t->object_id, &t->remove_spr);
    }
    return t;
}
Trigger* tr_copy(Trigger* t){
    if(!t)return NULL;
    Trigger* t2=tr_ini();
    memcpy(t2,t,sizeof(Trigger));
    return t2;
}
int tr_getId(Trigger* t){
    return t? t->id: -1;
}