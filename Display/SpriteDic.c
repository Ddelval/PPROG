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
char* sdic_dup="Dictionaries/spr_dup.txt";
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
            sdic_free();
            return NULL;
        }
    }
    fclose(f);
    f=fopen(sdic_tr,"r");
    if(!f){
        sdic_free();
        return NULL;
    }
    
    int num;
    fscanf(f,"%d", &num);
    for(int i=0;i<num;++i){
        int s_id, t_id, i1,i2,j1,j2;
        fscanf(f,"%d %d %d %d %d %d",&s_id, &t_id, &i1, &i2, &j1, &j2);
        for(int j=0;j<siz;++j){
            if(spr_getId(s->dat[j])==s_id){
                //fprintf(stderr,"%d\n",s_id);
                spr_addTrigger(s->dat[j],t_id,i1,i2,j1,j2);
            }
        }
    }
    fclose(f);
    f=fopen(sdic_dup,"r");
    fscanf(f,"%d",&num);
    s->dat=realloc(s->dat,sizeof(Sprite*)*(siz+num));
    for(int i=0;i<num;++i){
        int nid, pid;
        int ntrig;
        fscanf(f, "%d %d %d", &nid, &pid,&ntrig);
        //fprintf (stderr,"%d %d %d",nid,pid,ntrig);
        Sprite *spr;
        for(int j=0;j<siz;++j){
            if(spr_getId(s->dat[j])==pid){
                spr=spr_copy(s->dat[j]);
                spr_setId(spr,nid);
                spr_clearTrig(spr);
                break;
            }
        }
        for(int j=0;j<ntrig;++j){
            int t_id, i1,i2,j1,j2;
            fscanf(f,"%d %d %d %d %d", &t_id, &i1, &i2, &j1, &j2);
            spr_addTrigger(spr,t_id,i1,i2,j1,j2);
        }
        s->dat[siz+i]=spr;
    }
    s->size+=num;
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
