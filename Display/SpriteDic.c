//  PPROG
//	SpriteDic.c
//  Created by David del Val on 20/10/2019
//
//

#include "SpriteDic.h"

typedef struct{
    Sprite** dat;
    int size;
} SpriteDic;

char sdic_c[]= "Sprites/dic.txt";
char* sdic_tr = "Dictionaries/trsp.txt";
SpriteDic* sdic_data=NULL;
void sdic_free(){
    if(!sdic_data)return;
    if(sdic_data->dat){
        for(int i=0;i<sdic_data->size;++i){
            spr_free(sdic_data->dat[i]);
        }
        free(sdic_data->dat);
    }
    free(sdic_data);
}

int cmpSprite(const void* s1,const void* s2){
    Sprite* spr1=(Sprite*)s1;
    Sprite* spr2=(Sprite*)s2;
    return spr_getId(spr1)-spr_getId(spr2);
}
/*
 In the file, there should be a number with the amount of sprites
 that will be read.

 */
SpriteDic* sdic_ini(){
    int siz;
    FILE*f=fopen(sdic_c, "r");
    SpriteDic* s= calloc(1, sizeof(SpriteDic));
    if(!s)return NULL;

    fscanf(f,"%d", &siz);
    s->size=siz;
    s->dat=calloc(siz, sizeof(Sprite*));
    if(!s->dat){
        free(s);
        return NULL;
    }
    for(int i=0;i<siz;++i){
        s->dat[i]=spr_load(f);
        if(!s->dat[i]){
            sdic_free(s);
            return NULL;
        }
    }
    fclose(f);
    f=fopen(sdic_tr,"r");
    if(!f){
        sdic_free(s);
        return NULL;
    }
    
    int num;
    fscanf(f,"%d", &num);
    for(int i=0;i<num;++i){
        int s_id, t_id, i1,i2,j1,j2;
        fscanf(f,"%d %d %d %d %d %d",&s_id, &t_id, &i1, &i2, &j1, &j2);
        for(int j=0;j<siz;++j){
            if(spr_getId(s->dat[i])==s_id){
                spr_addTrigger(t_id,i1,i2,j1,j2);
            }
        }
    }
    return s;
}


Sprite* sdic_lookup(int id){
    if(sdic_data==NULL){
        sdic_data=sdic_ini();
    }
    for(int i=0;i<sdic_data->size;++i){
        if(spr_getId(sdic_data->dat[i])==id){
            return spr_copy(sdic_data->dat[i]);
        }
    }
    return NULL;
}
