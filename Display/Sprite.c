//  ppro
//	Sprite.c
//  Created by David del Val on 06/10/2019
//
//

#include "Sprite.h"
#define ret_free(spr) {spr_free(spr); return NULL;}
struct _Sprite{
  int id;
  int iPos, jPos;
  int width, height;
  bool existstrigger;
  int ** trigger;
  bool** collision;
  Canvas* canvas;
};
Sprite* spr_ini(int id, int width, int height){
    Sprite* spr= calloc(1, sizeof(Sprite));
    if(!spr)return NULL;
    spr->id=id;
    spr->width=width;
    spr->height=height;
    
    spr->trigger=calloc(height, sizeof(int*));
    if(!spr->trigger)ret_free(spr);
    for(int i=0;i<height;++i){
        spr->trigger[i]=calloc(width, sizeof(int));
        if(!spr->trigger[i])ret_free(spr);
    }
    
    spr->collision=calloc(height, sizeof(int*));
    if(!spr->collision)ret_free(spr);
    for(int i=0;i<height;++i){
        spr->collision[i]=calloc(width, sizeof(bool));
        if(!spr->collision[i])ret_free(spr);
    }
    spr->iPos=spr->jPos=-1;
    return spr;
}
void spr_setCoodrinates(Sprite* sp,int i,int j){
    if(!sp)return;
    sp->iPos=i;
    sp->jPos=j;
}
void spr_free(Sprite* sp){
    if(!sp)return;
    if(sp->trigger){
        for(int i=0;i<sp->width;++i){
            free(sp->trigger[i]);
        }
        free(sp->trigger);
    }
    if(sp->collision){
        for(int i=0;i<sp->width;++i){
            free(sp->collision[i]);
        }
        free(sp->collision);
    }
}
Sprite* spr_copy(const Sprite* spr){
    Sprite* sp=spr_ini(spr->id, spr->width, spr->height);
    if(!sp)ret_free(sp);
    sp->jPos=spr->jPos;
    sp->iPos=spr->iPos;
    sp->existstrigger=spr->existstrigger;
    for(int i=0;i<spr->height;++i){
        for(int j=0;j<spr->width;++j){
            sp->trigger[i][j]=spr->trigger[i][j];
            sp->collision[i][j]=spr->collision[i][j];
        }
    }
    canv_free(sp->canvas);
    sp->canvas=canv_copy(spr->canvas);
    if(!sp->canvas)ret_free(sp);
    return sp;
}

/**
 File format:
 <id> <format> (0 if the collision canvas is the canvas, 1 otherwise)
 <Canvas>
 <Collision_canvas>
 <nº triggers>
 <list of triggers>
 The format of a trigger has to be: i1, i2, j1, j2
 where i1 is the initial row, i2 is the last row,
 j1 is the first row, j2 is the last row
 (All included)
 
 */
Sprite* spr_load(FILE* f){
    if(!f)return NULL;
    int w,h,id;
    int ff;
    fscanf(f,"%d %d",&id,&ff);
    
    Canvas* c=canv_load(f);
    if(!c){
        return NULL;
    }
    w=canv_getWidth(c);
    h=canv_getHeight(c);
    Sprite* res=spr_ini(id, w, h);
    if(!res){
        canv_free(c);
        return NULL;
    }
    res->canvas=c;
    Canvas* colldata=NULL;
    if(ff){
        colldata=canv_load(f);
    }
    else{
        colldata=canv_copy(c);
    }
    
    for(int i=0;i<h;++i){
        for(int j=0;j<w;++j){
            res->collision[i][j]=pix_transparent(canv_getPixel(colldata,i,j));
        }
    }
    
    int n=0;
    fscanf(f, "%d",&n);
    if(n==0){
        res->existstrigger=false;
        return res;
    }
    int i1,i2,j1,j2;
    for(int i=0;i<n;++i){
        scanf("%d %d %d %d %d",&i1,&i2,&j1,&j2,&id);
        int jj1;
        while(i1<=i2){
            jj1=j1;
            while(jj1<=j2){
                if(jj1>=res->width||i1>=res->height){
                    spr_free(res);
                    return NULL;
                }
                res->trigger[i1][jj1]=id;
            }
        }
    }
    return res;
}
const Canvas* spr_getDispData(Sprite* spr){
    return spr->canvas;
}
int spr_getOI(Sprite* spr){
    return spr->iPos;
}
int spr_getOJ(Sprite* spr){
    return spr->jPos;
}