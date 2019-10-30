//  ppro
//	Room.c
//  Created by David del Val on 02/10/2019
//
//

#include "Room.h"
#define ret_free(r) {room_free(r); return NULL;}
#define MEM_INCREMENT 1.5
#define MEM_INI 5

typedef struct _box{
    int i,j,w,h;
}box;


struct _Room{
    int id;
    char* name;
    int hei, wid;

    Pixel* backcol;
    Sprite ** backg;
    int backgsiz;
    int backpos;
    Sprite ** overs;
    box * ov;
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
    
    r->ov=calloc(MEM_INI, sizeof(box));
    
    
    //Colission bool array
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
    if(!r->backg[r->backpos]){
        return NULL;
    }
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
        
        box* bb=realloc(r->ov, nsiz);
        if(!bb) return NULL;
        
    }
    r->overs[r->overpos]=spr_copy(s);
    if(!r->overs[r->overpos]){
        return NULL;
    }
    r->overpos++;
    return r;
}
Canvas* room_getRender(Room* r){
    Canvas* canv=canv_copy(r->map);
    Canvas* tmp;
    for(int i=0;i<r->overpos;++i){
        const Canvas * tmpc=spr_getDispData(r->overs[i]);
        tmp=canv_addOverlay(canv, spr_getDispData(r->overs[i]), spr_getOI(r->overs[i]), spr_getOJ(r->overs[i]));
        if(!tmp)return NULL;
        
        box b;
        b.i=spr_getOI(r->overs[i]);
        b.j=spr_getOJ(r->overs[i]);
        b.w=canv_getWidth(tmpc);
        b.h=canv_getHeight(tmpc);
        r->ov[i]=b;
    }
    return canv;
}
Canvas* room_getSubRender(Room* r, int i, int j, int wid, int hei){
    if(!r)return NULL;
    Canvas* c=room_getRender(r);
    if(i>=canv_getHeight(c)||j>=canv_getWidth(c)){
        canv_free(c);
        return NULL;
    }
    Canvas *cc=canv_subCopy(c, i, i+hei, j, j+wid);
    Canvas *ccc=canv_AdjustCrop(cc, wid, hei);
    canv_free(cc);
    canv_free(c);
    return ccc;
}
Room* room_modPos(Room* r, int index, int i, int j){
    if(!r||index>=r->overpos){
        return NULL;
    }
    spr_setOJ(r->overs[index], j);
    spr_setOI(r->overs[index], i);
    return r;
}
Room* room_printMod(Room* r,int disp_i, int disp_j, int i, int j, int wid, int hei){
    
    for(int i=0;i<r->overpos;++i){
        int i1,i2,j1,j2;
        i1=max(r->ov[i].i,i);
        j1=max(r->ov[i].j,j);
        
        i2=min(r->ov[i].i+r->ov[i].h,i+hei);
        j2=min(r->ov[i].j+r->ov[i].w,j+wid);
        if(i1>=i2||j1>=j2)continue;
        
        Canvas* c=canv_subCopy(r->map,i1,i2,j1,j2);
        canv_print(stdout, c, i1-i+disp_i, j1-j+disp_j+1);
    }
    for(int i=0;i<r->overpos;++i){
        /***
         
         ADD CHECK FOR OUT OF BOUNDS
         
         
         
         
         */
        const Canvas* torender=spr_getDispData(r->overs[i]);
        Canvas* bb=canv_subCopy(r->map, spr_getOI(r->overs[i]), spr_getOI(r->overs[i])+canv_getHeight(torender), spr_getOJ(r->overs[i]), spr_getOJ(r->overs[i])+canv_getWidth(torender));
        Canvas* rb=canv_Overlay(bb, torender, 0, 0);
        canv_print(stdout, rb, -i+disp_i+spr_getOI(r->overs[i]), -j+disp_j+spr_getOJ(r->overs[j])+1);
        //canv_print(stdout, rb, -i+disp_i+spr_getOI(r->overs[i]), 1000);
        box b;
        b.i=spr_getOI(r->overs[i]);
        b.j=spr_getOJ(r->overs[i]);
        b.w=canv_getWidth(spr_getDispData(r->overs[i]));
        b.h=canv_getHeight(spr_getDispData(r->overs[i]));
        r->ov[i]=b;
        canv_free(bb);
        canv_free(rb);
    }
    return r;
}
Room* room_incPos(Room* r, int index, int i, int j){
    if(!r||index>=r->overpos){
        return NULL;
    }
    spr_setOJ(r->overs[index], j+spr_getOJ(r->overs[index]));
    spr_setOI(r->overs[index], i+spr_getOI(r->overs[index]));
    return r;
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
