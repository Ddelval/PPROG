//  PPROG
//	EntityDic.c
//  Created by David del Val on 20/10/2019
//
//

#include "EntityDic.h"

typedef struct{
    Entity** dat;
    int size;
} EntityDic;

char edic[]= "Dictionaries/entities.txt";
EntityDic* edic_data=NULL;
void edic_free(){
    if(!edic_data)return;
    if(edic_data->dat){
        for(int i=0;i<edic_data->size;++i){
            entity_free(edic_data->dat[i]);
        }
        free(edic_data->dat);
    }
    free(edic_data);
}

/*
 In the file, there should be a number with the amount of entities
 that will be read.

 */
EntityDic* edic_ini(){
    int siz;
    FILE*f=fopen(edic, "r");
    EntityDic* s= calloc(1, sizeof(EntityDic));
    if(!s)return NULL;

    fscanf(f,"%d", &siz);
    s->size=siz;
    s->dat=calloc(siz, sizeof(Entity*));
    if(!s->dat){
        free(s);
        return NULL;
    }
    for(int i=0;i<siz;++i){
        s->dat[i]=entity_load(f,NULL);
        if(!s->dat[i]){
            edic_free();
            return NULL;
        }
    }
    fclose(f);
    return s;
}


Entity* edic_lookup(int id, Display* dis){
    if(edic_data==NULL){
        edic_data=edic_ini();
    }
    for(int i=0;i<edic_data->size;++i){
        if(entity_getId(edic_data->dat[i])==id){
            Entity* e=entity_copy(edic_data->dat[i]);
            entity_addtoDisplay(e,dis);
            return e;
        }
    }
    return NULL;
}
