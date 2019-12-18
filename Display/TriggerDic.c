//  PPROG
//	TriggerDic.c
//  Created by David del Val on 20/10/2019
//
//

#include "TriggerDic.h"

typedef struct{
    Trigger** dat;
    int size;
    int inserted;
} TriggerDic;

char trdic_c[]= "Dictionaries/trig.txt";
TriggerDic* trdic_data=NULL;

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

/*
 In the file, there should be a number with the amount of triggers
 that will be read.

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
int trdic_insert(Trigger* t){
    if(trdic_data==NULL)trdic_data=trdic_ini();
    if(trdic_data==NULL)return 1;
    tr_setId(t,-trdic_data->inserted);
    Trigger ** aux=realloc(trdic_data->dat,(trdic_data->size+1)*sizeof(Trigger*));
    if(!aux)return 1;
    trdic_data->dat=aux;
    trdic_data->dat[trdic_data->size]=tr_copy(t);
    trdic_data->size++;
    trdic_data->inserted++;
    return -trdic_data->inserted;
}

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
