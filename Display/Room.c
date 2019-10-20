//  ppro
//	Room.c
//  Created by David del Val on 02/10/2019
//
//

#include "Room.h"
#define ret_free(r) {room_free(r); return NULL;}
#define MEM_INCREMENT 1.5
#define MEM_INI 5
struct _Room{
    int id;
    char* name;
    int hei, wid;

    Pixel* backcol;
    Sprite ** backg;
    int backgsiz;
    int backpos;
    Sprite ** overs;
    int overssiz;
    int overpos;
    Canvas * map;

    bool ** colision;
    bool ** shadows;
    int ** trigger;
};

Room* room_ini(int id, char* name,int hei, int wid, Pixel* backcol){
    Room* r=calloc(1, sizeof(Room));
    if(!r)return NULL;
    r->id =id;
    r->name=calloc(strlen(name)+1, sizeof(char));
    if(!r->name)ret_free(r)
    strcpy(r->name, name);
    r->hei=hei;
    r->wid=wid;

    r->backgsiz=MEM_INI;
    r->backg=calloc(MEM_INI, sizeof(Sprite*));
    if(!r->backg)ret_free(r);

    r->overssiz=MEM_INI;
    r->overs=calloc(MEM_INI, sizeof(Sprite*));
    if(!r->overs)ret_free(r);
    r->backpos=r->overpos=0;

    //Colision bool array
    r->colision=calloc(hei, sizeof(bool*));
    if(!r->colision)ret_free(r);
    for(int i=0;i<hei;++i){
        r->colision[i]=calloc(wid, sizeof(bool));
        if(!r->colision[i])ret_free(r);
    }
    //Shadow bool array
    r->shadows=calloc(hei, sizeof(bool*));
    if(!r->shadows)ret_free(r);
    for(int i=0;i<hei;++i){
        r->shadows[i]=calloc(wid, sizeof(bool));
        if(!r->shadows[i])ret_free(r);
    }
    //Trigger int array
       r->trigger=calloc(hei, sizeof(int*));
       if(!r->trigger)ret_free(r);
       for(int i=0;i<hei;++i){
           r->trigger[i]=calloc(wid, sizeof(int));
           if(!r->trigger[i])ret_free(r);
       }
    r->map=canv_backGrnd(pix_retR(backcol), pix_retG(backcol), pix_retB(backcol), pix_retA(backcol), wid, hei);
    return r;

}

/// Loads a map from a file, the format should be the following
/// id lengthOfName name
/// height width backgroudColor
/// numberOfSprites
/// id ipos jpos
/// @param f File that contains the description of the room
Room* room_load(FILE* f){
    int h,w,id;
    int len;
    int nsp;
    fscanf(f,"%d %d",&id,&len);
    char* idd= calloc(len+1, sizeof(char));
    if(!idd){
        return NULL;
    }
    fscanf(f, "%s %d %d",idd,&h,&w);
    Pixel* back=pix_load(f);
    if(!back){
        free(idd);
        return NULL;
    }
    Room* r=room_ini(id, idd, h, w, back);
    if(!r){
        free(idd);
        pix_free(back);
        return NULL;
    }
    pix_free(back);
    free(idd);
    fscanf(f, "%d",&nsp);
    int a,b,c;
    for(int i=0;i<nsp;++i){
        fscanf(f, "%d %d %d",&a,&b,&c);
        Sprite* s=sdic_lookup(a);
        if(!s){
            room_free(r);
            return NULL;
        }
        spr_setOI(s, b);
        spr_setOJ(s, c);
        if(room_addBSprite(r, s)==NULL){
            room_free(r);
            return NULL;
        }
    }
    return r;
}
Room* room_addBSprite(Room* r, Sprite* s){
    if(!r||!s)return NULL;
    if(r->backpos==r->backgsiz){
        int nsiz=(int)ceil(r->backgsiz*MEM_INCREMENT);
        Sprite** tmp=realloc(r->backg,nsiz);
        if(!tmp)return NULL;
        r->backg=tmp;
        r->backgsiz=nsiz;
    }
    r->backg[r->backpos]=spr_copy(s);
    r->backpos++;
    canv_addOverlay(r->map, spr_getDispData(s), spr_getOI(s), spr_getOJ(s));
    return r;
}
Room* room_addOSprite(Room* r, Sprite* s){
    if(!r||!s)return NULL;
    if(r->overpos==r->overssiz){
        int nsiz=(int)ceil(r->overssiz*MEM_INCREMENT);
        Sprite** tmp=realloc(r->overs,nsiz);
        if(!tmp)return NULL;
        r->overs=tmp;
        r->overssiz=nsiz;
    }
    r->overs[r->overpos]=spr_copy(s);
    r->overpos++;
    return r;
}
Canvas* room_getRender(Room* r){
    Canvas* canv=r->map;
    Canvas* tmp;
    for(int i=0;i<r->overpos;++i){
        tmp=canv_Overlay(canv, spr_getDispData(r->overs[i]), spr_getOI(r->overs[i]), spr_getOJ(r->overs[i]));
        if(canv!=r->map)canv_free(canv);
        canv=tmp;
    }
    return canv;
}
void room_free(Room* r){
    if(!r)return;
    free(r->name);

    //Background sprite array
    if(r->backg){
        for(int i=0;i<r->backpos;++i) {
            spr_free(r->backg[i]);
        }
        free(r->backg);
    }

    //Foreground sprite array
    if(r->overs){
        for(int i=0;i<r->overpos;++i)spr_free(r->overs[i]);
        free(r->overs);
    }

    //Colision bool array
    if(r->colision){
        for(int i=0;i<r->hei;++i)free(r->colision[i]);
        free(r->colision);
    }
    //Shadow bool array
    if(r->shadows){
        for(int i=0;i<r->hei;++i)free(r->shadows[i]);
        free(r->shadows);
    }
    //Trigger array
    if(r->trigger){
        for(int i=0;i<r->hei;++i)free(r->trigger[i]);
        free(r->trigger);
    }
    canv_free(r->map);
    free(r);
}
