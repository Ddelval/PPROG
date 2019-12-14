/*
   File:   Crafting.c
   Author: Pedro Urbina
   Created on: 6 december 2019
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Recipe.h"


#define NAME_LENGTH 64
#define MARGIN 15
#define STROKE 4
struct _Recipe {
        int * quantities;
        int * elements;
        int result_id;
        int quant;
        int size;
        char name[NAME_LENGTH];
};


Recipe * rec_ini(){
        Recipe *r = (Recipe *) calloc (1, sizeof(Recipe));
        if(!r) return NULL;

        r->quantities = NULL;
        r->elements = NULL;
        r->quant=0;
        r->result_id = -1;
        r->size = 0;

        return r;
}

void rec_free(Recipe *r){
        if(!r) return;
        if(r->quantities) free(r->quantities);
        if(r->elements) free(r->elements);
        free(r);
        return;
}

Recipe * rec_copy(Recipe *r){
        if(!r) return NULL;
        Recipe * c = rec_ini();
        if(!c) return NULL;

        memcpy(c,r,sizeof(Recipe));
        c->elements   = calloc (r->size, sizeof(int));
        c->quantities = calloc (r->size, sizeof(int));

        memcpy(c->elements,r->elements,sizeof(int)*r->size);
        memcpy(c->quantities,r->quantities,sizeof(int)*r->size);

        return c;
}


/*
   Creates the array of objects and stores it in *obj. Also copies the array of
   quantities into *quant.
 */
Recipe* rec_getData(Recipe * r, Object *** obj, int ** quant){
        if(!r) return NULL;

        if(r->size == 0) return r;

        (*obj) = (Object **) calloc (sizeof(Object *), r->size);
        if(!(*obj)) return NULL;
        (*quant) = (int *) calloc (sizeof(int), r->size);
        if(!(*quant)) return NULL;

        for(int i=0;i<r->size;++i){
                (*obj)[i] = odic_lookup(r->elements[i]);
                (*quant)[i] = r->quantities[i];
        }
        return r;
}

//Following format:  (result_id) (quantity) 
//                   (name)
//                   (size)
//                   (elements_ids) 
//                   (list_of_quantites)
Recipe* rec_load(FILE *f){
        Recipe *r = rec_ini();
        int i = 0;
        if(!f || !r) return NULL;

        fscanf(f, "%d %d\n", &(r->result_id),&(r->quant));
        fgets(r->name, NAME_LENGTH ,f);
        r->name[strlen(r->name)-1]=0;
        fscanf(f, "%d", &(r->size));

        r->quantities = (int *) calloc (r->size,sizeof(int));
        r->elements   = (int *) calloc (r->size,sizeof(int));

        for(int i=0;i<r->size;++i) fscanf(f, "%d", &(r->elements[i]));
        for(int i=0;i<r->size;++i) fscanf(f, "%d", &(r->quantities[i]));

        return r;
}

/*
   Checks if this recipe can be prepared with the objects present in the inventory
   Note that it might be a great idea to add the following function to inventory:
   int inv_getQuantity(Inventory* inv, int obj_id)that returns -1 if the element
   is not present or the quantiy that the inventory has of it otherwise.
 */
bool rec_doable(Inventory* inv, Recipe* r){
        int i = 0;
        if(!inv || !r) return false;
        for(int i=0;i<r->size;++i)
                if(inv_getQuantity(inv, r->elements[i]) < r->quantities[i]) return false;

        return true;
}
Canvas* _rec_renderPlus(int size){
        Canvas* p1  =canv_backGrnd(255,255,255,255,size,STROKE/2);
        Canvas* p2  =canv_backGrnd(255,255,255,255,STROKE,size/2);
        Canvas* pp1 =canv_AdjustCrop(p1,size,size);
        Canvas* pp2 =canv_AdjustCrop(p2,size,size);
        Canvas* pl  =canv_Overlay(pp1,pp2,0,0);
        Canvas* plus=canv_AdjustCrop(pl,canv_getWidth(pl)+MARGIN*2,canv_getHeight(pl));
        canv_free(p1);  canv_free(p2);
        canv_free(pp1); canv_free(pp2);
        canv_free(pl);
        return plus;
}

Canvas* _rec_renderEqual(int size){
        int sep=min(STROKE*2,size-2*STROKE);
        Canvas* p1=canv_backGrnd(255,255,255,255,size,STROKE/2);
        Canvas* p2=canv_backGrnd(255,255,255,0,size,STROKE/2);
        Canvas* eq=canv_appendV(p1,p2);
        canv_appendVI(eq,p1);
        Canvas* equal=canv_AdjustCrop(eq,canv_getWidth(eq)+6*MARGIN,canv_getHeight(eq));
        return equal;
}
Canvas* _rec_renderLeft(Recipe* r,int obj_wid,int hei){
        if(!r)return NULL;
        Canvas* plus=_rec_renderPlus(hei/2);
        
        Object* ob=odic_lookup(r->elements[0]);
        Canvas* l=obj_render(ob,r->quantities[0],fcat_lookup(M4),fcat_lookup(M6),obj_wid,hei);
        Canvas* aux=NULL;
        obj_free(ob);

        for(int i=1;i<r->size;++i){
                canv_appendHI(l,plus);
                ob=odic_lookup(r->elements[i]);
                aux=obj_render(ob,r->quantities[i],fcat_lookup(M4),fcat_lookup(M6),obj_wid,hei);
                canv_appendHI(l,aux);
                canv_free(aux);
                obj_free(ob);
                aux=NULL;
        }
        canv_free(plus);
        return l;

}
Canvas* rec_render(Recipe* r, int obj_wid, int wid, int hei){
        if(!r)return NULL;
        
        
        Canvas* l2=_rec_renderLeft(r,obj_wid,hei);
        Object* ob=odic_lookup(r->result_id);
        Canvas* aux=obj_render(ob,r->quant,fcat_lookup(M4),fcat_lookup(M6),obj_wid,hei);
        Canvas* eq=_rec_renderEqual(hei/2);
        
        int lwid=wid-canv_getWidth(aux)-canv_getWidth(eq);
        Canvas* l= canv_backGrnd(0,0,0,0,lwid-canv_getWidth(l2),hei);
        canv_appendHI(l,l2);
        canv_appendHI(l,eq);
        
        canv_appendHI(l,aux);
        canv_free(aux);
        obj_free(ob);
        canv_free(l2);
        canv_free(eq);
        return l;
}
Recipe* rec_getObjDimensions(Recipe* r,int *ob_wid, int* hei){
        if(!r||!ob_wid||!hei)return NULL;
        int h,w;
        int he,wi;
        he=wi=0;
        for(int i=0;i<r->size;++i){
                Object* ob=odic_lookup(r->elements[i]);
                obj_renderDims(ob,r->quantities[i],fcat_lookup(M4),fcat_lookup(M6),&h,&w);
                obj_free(ob);
                he=max(he,h);
                wi=max(wi,w);
        }
        Object* ob=odic_lookup(r->result_id);
        obj_renderDims(ob,r->quant,fcat_lookup(M4),fcat_lookup(M6),&h,&w);
        obj_free(ob);
        he=max(he,h);
        wi=max(wi,w);
        *hei=he;
        *ob_wid=wi;
        return r;
}
int rec_getMinWidth(Recipe* r, int obj_wid, int hei){
        if(!r)return -1;
        Canvas* l=_rec_renderLeft(r,obj_wid,hei);
        Canvas* eq=_rec_renderEqual(hei/2);
        int s=canv_getWidth(l)+canv_getWidth(eq)+obj_wid;
        canv_free(l);
        canv_free(eq);
        return s;
}
int * rec_getQuantities(Recipe * r){
  if(!r) return NULL;
  return r->quantities;
}
int * rec_getElements(Recipe * r){
  if(!r) return NULL;
  return r->elements;
}
int rec_getResult_id(Recipe * r){
  if(!r) return -1;
  return r->result_id;
}
int rec_getSize(Recipe * r){
  if(!r) return -1;
  return r->size;
}
char *rec_getName(Recipe * r){
  if(!r) return NULL;
  return r->name;
}

