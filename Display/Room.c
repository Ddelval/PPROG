//  ppro
//	Room.c
//  Created by David del Val on 02/10/2019
//
//

#include "Room.h"
#define ret_free(r) {room_free(r); return NULL;}
#define MEM_INCREMENT 1.5
#define MEM_INI 5
#define MAX_TRIG 20

typedef struct _box{
    int i,j,w,h;
}box;

typedef struct _trigger{
    int code;
    int spindex;
}trigger;

struct _Room{
    int id;
    char* name;
    int hei, wid;
    //Top and left are included, right and bottom are excluded
    int c_t, c_l, c_r, c_b;
    Pixel* backcol;
    Sprite** backg;
    int backgsiz;
    int backpos;
    Sprite** overs;
    box* ov;
    int overssiz;
    int overpos;
    Canvas* map;
    Canvas* shadows;
    bool** colision;
    
    trigger*** trig;
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
    r->c_t=0;
    r->c_b=hei;
    r->c_l=0;
    r->c_r=wid;
    r->backgsiz=MEM_INI;
    r->backg=calloc(MEM_INI, sizeof(Sprite*));
    if(!r->backg)ret_free(r);

    r->overssiz=MEM_INI;
    r->overs=calloc(MEM_INI, sizeof(Sprite*));
    if(!r->overs)ret_free(r);
    r->backpos=r->overpos=0;

    r->ov=calloc(MEM_INI, sizeof(box));
    if(!r->ov) ret_free(r);

    //Colission bool array
    r->colision=calloc(hei, sizeof(bool*));
    if(!r->colision)ret_free(r);
    for(int i=0;i<hei;++i){
        r->colision[i]=calloc(wid, sizeof(bool));
        if(!r->colision[i])ret_free(r);
    }
    //Shadow 
    r->shadows=canv_backGrnd(0,0,0,0,wid,hei);

    //Trigger array
       r->trig=calloc(hei, sizeof(trigger**));
       if(!r->trig)ret_free(r);
       for(int i=0;i<hei;++i){
           r->trig[i]=calloc(wid, sizeof(trigger*));
           if(!r->trig[i])ret_free(r);
           for(int j=0;j<wid;++j){
               r->trig[i][j]=calloc(MAX_TRIG,sizeof(trigger));
               if(!r->trig[i][j])ret_free(r);
           }
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
        Sprite** tmp=realloc(r->backg,nsiz*sizeof(Sprite*));
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
    spr_processCollisions(s,r->colision,r->wid,r->hei);
    spr_processShadows(s,r->shadows);
    return r;
}
int room_addOSprite(Room* r, Sprite* s){
    if(!r||!s)return -1;
    if(r->overpos==r->overssiz){
        int nsiz=(int)ceil(r->overssiz*MEM_INCREMENT);
        Sprite** tmp=realloc(r->overs,nsiz*sizeof(Sprite*));
        if(!tmp)return -1;
        r->overs=tmp;
        r->overssiz=nsiz;

        box* bb=realloc(r->ov, nsiz*sizeof(box));
        if(!bb) return -1;

    }
    r->overs[r->overpos]=spr_copy(s);
    if(!r->overs[r->overpos]){
        return -1;
    }
    r->overpos++;
    return r->overpos-1;
}
Room* room_getBSpritePos(Room *r, int index, int* i, int *j){
    if(!r||index<0||index>=r->backpos||!i||!j)return NULL;
    *i=spr_getOI(r->backg[index]);
    *j=spr_getOJ(r->backg[index]);
    return r;
}
Canvas* room_getRender(Room* r){
    if(!r)return NULL;
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
    Canvas* res=canv_subCopy(canv,r->c_t,r->c_b,r->c_l,r->c_r);
    canv_free(canv);
    return res;
}
Canvas* room_getSubRender(Room* r, int i, int j, int wid, int hei){
    if(!r)return NULL;
    Canvas* c=room_getRender(r);
    if(i>=canv_getHeight(c)||j>=canv_getWidth(c)){
        canv_free(c);
        return NULL;
    }
    Canvas *cc=canv_subCopy(c, i, i+hei, j, j+wid);
    //Canvas *ccc=canv_AdjustCrop(cc, wid, hei);
    //canv_free(cc);
    canv_free(c);
    return cc;
}
/*
-1:error
 0: ok
 1: top border
 2: right border
 3: bottom border
 4: left border
 5: collison 
 */
int room_modPos(Room* r, int index, int i, int j){
    if(!r||index>=r->overpos){
        return -1;
    }
    int aux;
    int retval=0;
    if(i<=r->c_t+1){
        i=r->c_t;
        if(spr_getOI(r->overs[index])!=r->c_t+1) i=r->c_t+1;
        else retval=1;
    }
    if(j<=r->c_l+1){
        j=r->c_l;
        if(spr_getOJ(r->overs[index])!=r->c_l+1) j=r->c_l+1;
        else retval=4;
    }
    aux=spr_getHeight(r->overs[index]);
    if(aux==-1)return -1;
    if(aux+i>=r->c_b-1){
        i=r->c_b-aux;
        if(spr_getOI(r->overs[index])+spr_getHeight(r->overs[index])!=r->c_b-1) i=r->c_b-aux-1;
        else retval= 3;
    }
    aux=spr_getWidth(r->overs[index]);
    if(aux==-1)return -1;
    if(aux+j>=r->c_r-1){
        j=r->c_r-aux;
        if(spr_getOJ(r->overs[index])+spr_getWidth(r->overs[index])!=r->c_r-1) j=r->c_r-aux-1;
        else retval= 2;
    }
    //Fits:
    aux=spr_checkCollisions(r->overs[index],r->colision,r->wid,r->hei,i,j);
    if(aux==-1)return -1;
    if(aux==1)return 5;
    spr_setOJ(r->overs[index], j);
    fprintf(stderr,"%d\n",j);
    spr_setOI(r->overs[index], i);
    return retval;
}
int room_incPos(Room* r, int index, int i, int j){
    if(!r||index>=r->overpos){
        return -1;
    }
    return room_modPos(r,index,i+spr_getOI(r->overs[index]),j+spr_getOJ(r->overs[index]));
}
/**
 * @brief Sets the are of the window that will be rendered when the rendering functions are called
 * This should be used when the player moves to another section of the map or when 
 * 
 * @param ro Room
 * @param t  Top limit,      included
 * @param l  Left limit,     included 
 * @param b  Bottom limit,   excluded
 * @param r  Right limit,    excluded
 * @return   NULL if error, the given room otherwise 
 */
Room* room_setBounds(Room*ro, int t, int l, int b, int r){
    if(!ro)return NULL;
    int wi=r-l;
    int he=b-t;
    if(t<0){
        t=0;
        b=min(t+he,ro->hei);
    }
    if(l<0){
        l=0;
        r=min(l+wi,ro->wid);
    }
    if(b>ro->hei){
        b=ro->hei;
        t=max(0,b-he);
    }
    if(r>ro->wid){
        r=ro->wid;
        l=max(0,r-wi);
    }
    ro->c_t=t;
    ro->c_l=l;
    ro->c_b=b;
    ro->c_r=r;
    return ro;
}

Room* room_printMod(Room* r,int disp_i, int disp_j){
    int wid=r->c_r-r->c_l;
    int hei=r->c_b-r->c_t;
    char * to_print=calloc(2*wid*hei, sizeof(char));
    int ipos=0;
    for(int i=0;i<r->overpos;++i){
        int i1,i2,j1,j2;
        i1=max(r->ov[i].i,r->c_t);
        j1=max(r->ov[i].j,r->c_l);

        i2=min(r->ov[i].i+r->ov[i].h,r->c_b);
        j2=min(r->ov[i].j+r->ov[i].w,r->c_r);
        if(i1>=i2||j1>=j2)continue;
        Canvas* c=canv_subCopy(r->map,i1,i2,j1,j2);
        canv_print(stdout,c,i1-r->c_t+disp_i, j1-r->c_l+disp_j+1);
        fflush(stdout);
        canv_free(c);
    }
    for(int i=0;i<r->overpos;++i){
        Canvas* torender=canv_copy(spr_getDispData(r->overs[i]));
        Canvas* bc=canv_subCopy(r->map, spr_getOI(r->overs[i]), spr_getOI(r->overs[i])+canv_getHeight(torender), spr_getOJ(r->overs[i]), spr_getOJ(r->overs[i])+canv_getWidth(torender));
        Canvas* b2=canv_subCopy(r->shadows, spr_getOI(r->overs[i]), spr_getOI(r->overs[i])+canv_getHeight(torender), spr_getOJ(r->overs[i]), spr_getOJ(r->overs[i])+canv_getWidth(torender));
        canv_addOverlay(torender,b2,0,0);
        canv_printSolid(stdout,torender,bc,disp_i-r->c_t+spr_getOI(r->overs[i]), disp_j-r->c_l+spr_getOJ(r->overs[i])+1);
        box b;
        b.i=spr_getOI(r->overs[i]);
        b.j=spr_getOJ(r->overs[i]);
        b.w=canv_getWidth(spr_getDispData(r->overs[i]));
        b.h=canv_getHeight(spr_getDispData(r->overs[i]));
        r->ov[i]=b;
        canv_free(bc);
        canv_free(torender);
        canv_free(b2);
    }
    return r;
}
int room_scroll(Room* r, double i, double j){
    if(!r)return -1;
    int di,dj;
    if(r->c_t==0&&i<0)       return 0;
    if(r->c_l==0&&j<0)       return 0;
    if(r->c_r==r->wid&&j>0)  return 0;
    if(r->c_b==r->hei&&i>0)  return 0;
    di=i*(r->c_b-r->c_t);
    dj=j*(r->c_r-r->c_l);
    room_setBounds(r,r->c_t+di,r->c_l+dj,r->c_b+di,r->c_r+dj);
    return 1;
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
    //Shadow 
    canv_free(r->shadows);
    //Trigger array
    if(r->trig){
        for(int i=0;i<r->hei;++i)free(r->trig[i]);
        free(r->trig);
    }

       for(int i=0;i<r->hei;++i){
           for(int j=0;j<r->wid;++j){
               free(r->trig[i][j]);
           }
           free(r->trig[i]);
       }
    free(r->trig);

    canv_free(r->map);
    free(r->ov);
    pix_free(r->backcol);
    free(r);
}
