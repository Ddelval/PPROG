/**
 * @file TriggerDic.c
 * @author David del Val 
 * @date 2019-10-20
 * 
 * @brief Contains all the triggers that are used
 *        in the game
 * 
 * This ADT will contain every trigger so that they
 * can be identified only with their id or the 
 * entity that they reference without having to 
 * keep several copies on the same trigger in 
 * different modules
 * 
 */
#include "TriggerDic.h"
extern char* curr_world;

typedef struct{
    Trigger** dat;
    int size;
    int inserted;
} TriggerDic;

char trdic_c[]= "Dictionaries/trig.txt";
TriggerDic* trdic_data=NULL;

/*-----------------------------------------------------------------*/
/**
 * @brief Frees all the memory related to the trigger dictionary
 * 
 * This function has to be called before the game exits
 * 
 */
void trdic_free(){
    if(!trdic_data)return;
    if(trdic_data->dat){
        for(int i=0;i<trdic_data->size;++i){
            tr_free(trdic_data->dat[i]);
        }
        free(trdic_data->dat);
    }
    free(trdic_data);
}


/*-----------------------------------------------------------------*/
/**
 * @brief Initalises the trigger dictionary
 * 
 * In order to do so, ww read the file defined in trdic_c
 * This file includes the amount of triggers and each trigger, 
 * one after the other
 * 
 * @return new TriggerDic with all the triggers
 */
TriggerDic* trdic_ini(){
    int siz;
    FILE*f=fopen(trdic_c, "r");
    TriggerDic* s= calloc(1, sizeof(TriggerDic));
    if(!s||!f)return NULL;

    fscanf(f,"%d", &siz);
    s->size=siz;
    s->dat=calloc(siz, sizeof(Trigger*));
    if(!s->dat){
        free(s);
        return NULL;
    }
    for(int i=0;i<siz;++i){
        s->dat[i]=tr_load(f);
        if(!s->dat[i]){
            trdic_free();
            return NULL;
        }
    }
    //qsort(s->dat,siz,sizeof(Trigger*),cmpTrigger);
    return s;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Inserts a new trigger on the dictionary
 * 
 * This function should not be used for triggers
 * that will already be attached to each sprite
 * before the game starts
 * 
 * It is intended to be used to insert triggers
 * related to the allies and the enemies present in 
 * each world
 * 
 * @param t 
 * @return int 
 */
int trdic_insert(Trigger* t){
    if(trdic_data==NULL)trdic_data=trdic_ini();
    if(trdic_data==NULL)return 1;
    trdic_data->inserted++;
    tr_setId(t,-trdic_data->inserted);
    tr_setWorld(t,curr_world);
    Trigger ** aux=realloc(trdic_data->dat,(trdic_data->size+1)*sizeof(Trigger*));
    if(!aux)return 1;
    trdic_data->dat=aux;
    trdic_data->dat[trdic_data->size]=tr_copy(t);
    trdic_data->size++;
    return -trdic_data->inserted;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Look for a trigger with a particular id
 * 
 * @param id  Id of the trigger we are looking for
 * @return    Copy of the trigger
 */
Trigger* trdic_lookup(int id){
    if(trdic_data==NULL){
        trdic_data=trdic_ini();
    }
    for(int i=0;i<trdic_data->size;++i){
        if(tr_getId(trdic_data->dat[i])==id){
            return tr_copy(trdic_data->dat[i]);
        }
    }
    return NULL;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Search for the conversation trigger associated
 *        to this ally in this world
 * 
 * @param ally_id Position of the sprite of the ally in overs
 * @return        Id of the trigger
 */
int trdic_talksearch(int ally_id){
    if(trdic_data==NULL){
        return -1;
    }
    for(int i=0;i<trdic_data->size;++i){
        char* c=tr_getWorld(trdic_data->dat[i]);
        if(c&&tr_getAlly_id(trdic_data->dat[i])==ally_id&&strcmp(c,curr_world)==0){
            free(c);
            return tr_getId(trdic_data->dat[i]);
        }
        free(c);
    }
    return -1;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Search for the combat trigger associated
 *        to this enemy in this world
 * 
 * @param entity_id Position of the sprite of the enemy in overs
 * @return          Id of the trigger
 */
int trdic_attacksearch(int entity_id){
    if(trdic_data==NULL)return -1;
    for(int i=0;i<trdic_data->size;++i){
        char* c=tr_getWorld(trdic_data->dat[i]);
        if(c&&tr_getAlly_id(trdic_data->dat[i])==entity_id&&strcmp(c,curr_world)==0){
            return tr_getId(trdic_data->dat[i]);
        }
        free(c);
    }
    return -1;
}
