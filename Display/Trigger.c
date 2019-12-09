//  PPROG
//	SpriteDic.c
//  Created by David del Val on 05/12/2019
//
//
#include "Trigger.h"
#define MAX_NAME 150
struct _Trigger
{
    int trig_id;
    trig_type type;

    char name[MAX_NAME];
    
    /* Obtain resources */
    int obj_id;
    int quantity;
    bool spr_remove;

    /* Change place */
    int room_id;

    /* Engage in conversation */
    int entity_id;
};
Trigger* tr_ini(){
    Trigger* t=calloc(1,sizeof(Trigger));
    return t;
}

void tr_free(Trigger* t){
    free(t);
}

Trigger * tr_load(FILE* f){
    if(!f)return NULL;
    Trigger * t=trig_ini();
    if(!t)return NULL;
    fscanf(f,"%d %d\n",t->trig_id,t->type);
    fgets(t->name,MAX_NAME,f);
    fscanf(f,"%d %d %d %d %d", t->obj_id, t->quantity,t->spr_remove, t->room_id, t->entity_id);
    return t;
}
Trigger* tr_copy(const Trigger * src){
    if(!src)return NULL;
    Trigger* t2=tr_ini();
    memcpy(t2,src,sizeof(Trigger));
    return t2;
}

