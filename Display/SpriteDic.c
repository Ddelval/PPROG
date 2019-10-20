//  ppro
//	SpriteDic.c
//  Created by David del Val on 20/10/2019
//
//

#include "SpriteDic.h"

struct _SpriteDic{
    Sprite** dat;
    int size;
};

char c[]= "File in which the sprites are";
SpriteDic* data=NULL;
void sdic_free(SpriteDic* d){
    if(!d)return;
    if(d->dat){
        for(int i=0;i<d->size;++i){
            spr_free(d->dat[i]);
        }
        free(d->dat);
    }
    free(d);
}

int cmpSprite(const void* s1,const void* s2){
    Sprite* spr1=(Sprite*)s1;
    Sprite* spr2=(Sprite*)s2;
    return spr_getId(spr1)-spr_getId(spr2);
}
/*
 In the file, there should be a number with the amount of sprites
 that
 
 */
SpriteDic* sdic_ini(){
    int siz;
    FILE*f=fopen(c, "r");
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
        }
    }
    qsort(s->dat,siz,sizeof(Sprite*),cmpSprite);
    return s;
}


Sprite* sdic_lookup(int id){
    if(data==NULL){
        data=sdic_ini();
    }
    Sprite* ref=spr_ini(id, 100, 100);
    Sprite* res=(Sprite*)bsearch(ref, data->dat, data->size, sizeof(Sprite*), cmpSprite);
    free(ref);
    return spr_copy(res);
}

