//  PPROG
//	Trigger.c
//  Created by David del Val on 09/12/2019
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
    int sprite_index;
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
Trigger* tr_setSpr(Trigger* tr, int i){
    if(!tr)return NULL;
    tr->sprite_index=i;
    return tr;
}
Trigger * tr_load(FILE* f){
    if(!f)return NULL;
    Trigger * t=tr_ini();
    if(!t)return NULL;
    fscanf(f,"%d %d\n",&t->trig_id,&t->type);
    fgets(t->name,MAX_NAME,f);
    fscanf(f,"%d %d %d %d %d", &t->obj_id, &t->quantity,&t->spr_remove, &t->room_id, &t->entity_id);
    return t;
}
Trigger* tr_copy(const Trigger * src){
    if(!src)return NULL;
    Trigger* t2=tr_ini();
    memcpy(t2,src,sizeof(Trigger));
    return t2;
}
int tr_getId(const Trigger* tr){
    return tr? tr->trig_id: -1;
}

trig_type tr_getType(const Trigger* tr){
    return tr? tr->type:-1;
}
int tr_getObj_id(const Trigger* tr){
    return tr? tr->obj_id:-1;
}
int tr_getSpr_index(const Trigger* tr){
    return tr? tr->sprite_index: -1;
}
bool tr_getSpr_remove(const Trigger* tr){
    return tr? tr->spr_remove:-1;
}
int tr_getQuantity(const Trigger* tr){
    return tr? tr->quantity:-1;
}
bool tr_needsTrigger(trig_type t){
    if(t==SHOW)return false;
    return true;
}