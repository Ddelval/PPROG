//  PPROG
//	Trigger.c
//  Created by David del Val on 09/12/2019
//
//
#include "Trigger.h"
#define MAX_NAME 150
#define MAX_ROOM 100
struct _Trigger
{
    int trig_id;
    tr_type type;

    char name[MAX_NAME];
    
    int tier;

    /* Obtain resources */
    int obj_id;
    int sprite_index;
    int quantity;
    
    bool spr_remove;

    /* Change place */
    char next_room[MAX_ROOM];

    /* Engage in conversation */
    int ent_id;
    char world[MAX_ROOM];
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
    t->tier=-1;
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
    fscanf(f,"%d",&t->tier);
    if(t->type==OBTAIN){
        fscanf(f,"%d %d %d", &t->obj_id, &t->quantity,&t->spr_remove); 
    }
    if(t->type==ENTER){
        t->next_room[0]='\n';
        t->next_room[1]='\0';
        while(!strcmp(t->next_room,"\n"))fgets(t->next_room,MAX_ROOM,f);
        t->next_room[strlen(t->next_room)-1]=0;
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
    if(!t)return;
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

tr_type tr_getType(const Trigger* tr){
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
bool tr_needsTrigger(tr_type t){
    if(t==SHOW)return false;
    return true;
}

void* tr_getEntityRef(const Trigger* t){
    if(!t)return NULL;
    return t->entit;
}

char* tr_getDesc(const Trigger* t){
    return t? strdup(t->name):NULL;
}

int tr_getAlly_id(const Trigger * t){
    return t? t->ent_id:-1;
}
char* tr_getNWorld(const Trigger* t){
    if(!t)return NULL;
    return strdup(t->next_room);
}
/** SETTERS **/
void tr_setId(Trigger * t, int id){
    if(!t)return;
    t->trig_id=id;
}

void tr_setSpr(Trigger* tr, int i){
    if(tr) tr->sprite_index=i;
}
void tr_setWorld(Trigger* tr, char* wor){
    if(strlen(wor)+1>MAX_ROOM)return;
    if(tr)strcpy(tr->world,wor);
}
char* tr_getWorld(const Trigger* tr){
    return (tr)? strdup(tr->world):NULL;
}
int tr_getTier(const Trigger* tr){
    return tr? tr->tier: -1;
}
