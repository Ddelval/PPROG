/*
   File:   Crafting.c
   Author: Pedro Urbina
   Created on: 6 december 2019
 */


#include "RecipeDic.h"
#define NAME_LENGTH 50


typedef struct {
        Recipe** rec;
        int size;
} RecipeDic;

RecipeDic* rdic=NULL;
char* rec_dic = "Dictionaries/c_recipies.txt";


void rdic_free(){
        if(!rdic) return;
        if(rdic->rec) {
                for(int i=0; i<rdic->size; ++i) {
                        rec_free(rdic->rec[i]);
                }
                free(rdic->rec);
        }
        free(rdic);
}

void rdic_ini(){
        FILE* f=fopen(rec_dic,"r");
        rdic=calloc(1,sizeof(RecipeDic));
        if(!rdic) return;

        fscanf(f,"%d",&rdic->size);
        rdic->rec=calloc(rdic->size,sizeof(Recipe*));
        if(!rdic->rec) {
                rdic=NULL;
                return;
        }
        for(int i=0; i<rdic->size; ++i) {
                rdic->rec[i]=rec_load(f);
                if(!rdic->rec[i]) {
                        rdic_free();
                        rdic=NULL;
                        return;
                }
        }
}




Recipe* rdic_lookup(int id){
  if(!rdic)rdic_ini();
  if(!rdic)return NULL;
  for(int i=0;i<rdic->size;++i){
      if(rec_getResult_id(rdic->rec[i])==id){
          Recipe* r=rec_copy(rdic->rec[i]);
          return r;
      }
  }
  return NULL;
}

Recipe** rdic_getAllDoable(Inventory* inv, int * size){
        Recipe **r=NULL;

        if(!rdic) rdic_ini();
        if(!rdic) return NULL;
        for(int i=0; i < rdic->size; ++i) {
                if(rec_doable(inv, rdic->rec[i]) == true) {
                        (*size)++;
                        if(r) r = realloc(r, (*size) * sizeof(Recipe*));
                        if(!r) r = (Recipe **) calloc ((*size), sizeof(Recipe*));
                        r[i] = rec_copy(rdic->rec[i]);
                }
        }
        return r;
}
