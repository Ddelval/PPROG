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
    int ent_id;
    void * entit;
};

/*-----------------------------------------------------------------*/
/**
 * @brief Allocates the memory for the new trigger
 * 
 * @return The new trigger
 */
Trigger* tr_ini(){
    Trigger* t=calloc(1,sizeof(Trigger));
    return t;
}

Trigger* tr_createTalk(void* e,int ally_id){
    Trigger* t=tr_ini();
    if(!t)return NULL;
    t->ent_id=ally_id;
    t->entit=e;
    t->type=TALK;
    return t;
}

Trigger* tr_createAttack(void*e, int enemy_id){
    Trigger* t=tr_ini();
    if(!t)return NULL;
    t->ent_id=enemy_id;
    t->entit=e;
    t->type=COMBAT;
    return t;
}

Trigger* tr_copy(const Trigger * src){
    if(!src)return NULL;
    Trigger* t2=tr_ini();
    memcpy(t2,src,sizeof(Trigger));
    return t2;
}

Trigger * tr_load(FILE* f){
    if(!f)return NULL;
    Trigger * t=tr_ini();
    if(!t)return NULL;
    fscanf(f,"%d %d\n",&t->trig_id,(int*)&t->type);
    fgets(t->name,MAX_NAME,f);
    if(strlen(t->name))t->name[strlen(t->name)-1]=0;
    if(t->type==OBTAIN){
        fscanf(f,"%d %d %d", &t->obj_id, &t->quantity,&t->spr_remove); 
    }
    if(t->type==ENTER){
        fscanf(f,"%d",&t->room_id);
    }
    if(t->type==TALK){
        fscanf(f,"%d",&t->ent_id);
    }
    return t;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Frees the Trigger
 * 
 * @param t Trigger to be freed
 */
void tr_free(Trigger* t){
    free(t);
}

bool tr_completeEqual(Trigger* t1,Trigger* t2){
    if(!t1||!t2)return -1;
    return memcmp(t1,t2,sizeof(Trigger))==0;
}


/** GETTERS */

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

void* tr_getEntityRef(Trigger* t){
    if(!t)return NULL;
    return t->entit;
}

char* tr_getDesc(Trigger* t){
    return t? strdup(t->name):NULL;
}

int tr_getAlly_id(Trigger * t){
    return t? t->ent_id:-1;
}
/** SETTERS **/
void tr_setId(Trigger * t, int id){
    if(!t)return;
    t->trig_id=id;
}

Trigger* tr_setSpr(Trigger* tr, int i){
    if(!tr)return NULL;
    tr->sprite_index=i;
    return tr;
}