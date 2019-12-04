//  PPROG
//	ObjectDic.c
//  Created by David del Val on 05/12/2019
//
//

#include "ObjectDic.h"
#include "Object.h"
typedef struct 
{
    Object** obj;
    int size;
}ObjectDic;

char* c="Objects/dic.txt";
ObjectDic* odic=NULL;

void odic_free(){
    if(!odic)return;
    if(odic->obj){
        for(int i=0;i<odic->size;++i){
            obj_free(odic->obj[i]);
        }
        free(odic->obj);
    }
    free(odic);
}
void odic_ini(){
    FILE* f=fopen(c,"r");
    odic=calloc(1,sizeof(ObjectDic));
    if(!odic)return;

    fscanf(f,"%d",&odic->size);
    odic->obj=calloc(odic->size,sizeof(Object*));
    if(!odic->obj){
        odic=NULL;
            return;
    }
    for(int i=0;i<odic->size;++i){
        odic->obj[i]=obj_load(f);
        if(!odic->obj[i]){
            odic_free();
            odic=NULL;
            return;
        }
    }
}
Object* odic_lookup(int id){
    if(!odic)odic_ini();
    if(!odic)return NULL;
    for(int i=0;i<odic->size;++i){
        if(obj_getId(odic->obj[i])==id){
            return obj_copy(odic->obj[i]);
        }
    }
    return NULL;
}
