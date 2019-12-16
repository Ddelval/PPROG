//  ppro
//	Sprite.c
//  Created by David del Val on 06/10/2019
//
//

#include "Sprite.h"
#define ret_free(spr) {spr_free(spr); return NULL;}
typedef struct {
    int tr_id;
    int i1,i2,j1,j2;
}trigdata;
struct _Sprite{
  int id;
  int iPos, jPos;
  int width, height;
  trigdata* tr;
  int trsize;
  bool** collision;
  bool** shadow;
  Canvas* canvas;
};
Sprite* spr_ini(int id, int width, int height){
    Sprite* spr= calloc(1, sizeof(Sprite));
    if(!spr)return NULL;
    spr->id=id;
    spr->width=width;
    spr->height=height;

    spr->collision=calloc(height, sizeof(int*));
    if(!spr->collision)ret_free(spr);
    for(int i=0;i<height;++i){
        spr->collision[i]=calloc(width, sizeof(bool));
        if(!spr->collision[i])ret_free(spr);
    }

    spr->shadow=calloc(height, sizeof(int*));
    if(!spr->shadow)ret_free(spr);
    for(int i=0;i<height;++i){
        spr->shadow[i]=calloc(width, sizeof(bool));
        if(!spr->shadow[i])ret_free(spr);
    }
    spr->iPos=spr->jPos=-1;
    return spr;
}
void spr_setCoordinates(Sprite* sp,int i,int j){
    if(!sp)return;
    sp->iPos=i;
    sp->jPos=j;
}
void spr_free(Sprite* sp){
    if(!sp)return;
    if(sp->collision){
        for(int i=0;i<sp->height;++i){
            free(sp->collision[i]);
        }
        free(sp->collision);
    }
    free(sp->tr);
    canv_free(sp->canvas);
    free(sp);
}
Sprite* spr_copy(const Sprite* spr){
    if(!spr)return NULL;
    Sprite* sp=spr_ini(spr->id, spr->width, spr->height);
    if(!sp)ret_free(sp);
    sp->jPos=spr->jPos;
    sp->iPos=spr->iPos;
    for(int i=0;i<spr->height;++i){
        for(int j=0;j<spr->width;++j){
            sp->collision[i][j]=spr->collision[i][j];
            sp->shadow[i][j]=spr->shadow[i][j];
        }
    }
    canv_free(sp->canvas);
    sp->canvas=canv_copy(spr->canvas);
    sp->trsize=spr->trsize;
    sp->tr=calloc(sp->trsize,sizeof(trigdata));
    memcpy(sp->tr,spr->tr,sizeof(trigdata)*sp->trsize);
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
 The format of a trigger has to be: i1, i2, j1, j2 id
 where i1 is the initial row, i2 is the last row,
 j1 is the first row, j2 is the last row
 (All included)

 */
Sprite* spr_load(FILE* f){
    if(!f)return NULL;
    int w,h,id;
    int ff, sh;
    fscanf(f,"%d %d %d",&id,&ff,&sh);

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
    
    
    if(ff==1){
        Canvas* colldata=canv_load(f);
        for(int i=0;i<h;++i){
            for(int j=0;j<w;++j){
                if(((double)i)/h>((double)sh)/100)res->collision[i][j]=!pix_halfTransparent(canv_getPixel(colldata,i,j));
                else res->shadow[i][j]=!pix_halfTransparent(canv_getPixel(colldata,i,j));
                
            }
        }
        free(colldata);
    }
    else if(ff==0){
        for(int i=0;i<h;++i){
            
            for(int j=0;j<w;++j){
                if(i*100>h*sh)res->collision[i][j]=!pix_halfTransparent(canv_getPixel(c,i,j));
                else {
                    
                    res->shadow[i][j]=!pix_halfTransparent(canv_getPixel(c,i,j));
                    //fprintf(stderr,"-%d %d %d %d \n",id,i,j,res->shadow[i][j]);
                }
            }
        }
    }
    int n=0;
    fscanf(f, "%d",&n);
    if(n==0){
        return res;
    }
    int i1,i2,j1,j2;
    for(int i=0;i<n;++i){
        scanf("%d %d %d %d %d",&i1,&i2,&j1,&j2,&id);
        spr_addTrigger(res,id,i1,i2,j1,j2);
    }
    return res;
}
Sprite* spr_addTrigger(Sprite* s, int tr_id, int i1, int i2, int j1, int j2){
    if(!s)return NULL;
    if(!s->tr){
        s->tr=calloc(1,sizeof(trigdata));
        if(!s->tr)return NULL;
        s->trsize=0;
    }
    else{
        s->tr=realloc(s->tr,(s->trsize+1)*sizeof(trigdata));
        if(!s->tr)return NULL;
    }
    s->tr[s->trsize].tr_id=tr_id;
    s->tr[s->trsize].i1=i1;
    s->tr[s->trsize].i2=i2;
    s->tr[s->trsize].j1=j1;
    s->tr[s->trsize].j2=j2;
    s->trsize++;
    return s;

    /*
    int jj1;
    while(i1<=i2){
        jj1=j1;
        while(jj1<=j2){
            if(jj1>=s->width||i1>=s->height||i1<0||jj1<0){
                continue;
            }
            for(int w=0;w<SPR_NTRIGGERS;++w){
                if(s->trigger[i1][jj1][w]==-1){
                    s->trigger[i1][jj1][w]=tr_id;
                }
            }
            jj1++;
        }
        i1++;
    }
    return s;
    */
}
/*
Sprite* spr_printTrigger(Sprite* s){
    fprintf(stderr,"\n\n\n-----------\n%d\n",spr_getId(s));
    for(int i=0;i<s->height;++i){
        for(int j=0;j<s->width;++j){
            fprintf(stderr,"%d ",s->trigger[i][j][0]);
        }
        fprintf(stderr,"\n");
    }
*/
Sprite* spr_processCollisions(Sprite* s,bool** rarr,int rwid, int rhei){
    if(!rarr)return NULL;
    int i2,j2;
    for(int i=0;i<s->height;++i){
        i2=i+s->iPos;
        if(i2<0||i2>=rhei)continue;
        for(int j=0;j<s->width;++j){
            j2=j+s->jPos;
            if(j2<0||j2>=rwid)continue;
            rarr[i2][j2]= rarr[i2][j2]||s->collision[i][j];
        }
    }
    return s;
}
int spr_checkCollisions(Sprite*s,bool**rarr,int rwid,int rhei, int ni,int nj){
    if(!rarr)return -1;
    int i2,j2;
    for(int i=0;i<s->height;++i){
        i2=i+ni;
        if(i2>=rhei)break;
        for(int j=0;j<s->width;++j){
            j2=j+nj;
            if(j2>=rwid)break;
            if(!s->collision[i][j])continue;
            if(rarr[i2][j2]){
                
                return 1;
            }
        }
    }
    return 0;
}

Sprite* spr_processShadows(Sprite* s,Canvas* shad){
    if(!s||!shad)return NULL;
    int i2,j2;
    int rhei=canv_getHeight(shad);
    int rwid=canv_getWidth(shad);
    for(int i=0;i<s->height;++i){
        i2=i+s->iPos;
        if(i2<0||i2>=rhei)continue;
        for(int j=0;j<s->width;++j){
            j2=j+s->jPos;
            if(j2<0||j2>=rwid)continue;
                //fprintf(stderr,"%d %d %d %d \n",s->id,i,j,s->shadow[i][j]);
            if(s->shadow[i][j]||s->collision[i][j]){
                assert(canv_setPixel(shad,canv_getPixel(s->canvas,i,j),i2,j2)!=NULL);
            }
        }
    }
    return s;
}
const Canvas* spr_getDispData(Sprite* spr){
    if(!spr)return NULL;
    return spr->canvas;
}
int spr_getOI(Sprite* spr){
    if(!spr)return 0;
    return spr->iPos;
}
int spr_getOJ(Sprite* spr){
    if(!spr)return 0;
    return spr->jPos;
}
int spr_getWidth(Sprite* spr){
    if(!spr)return -1;
    return spr->width;
}
int spr_getHeight(Sprite* spr){
    if(!spr) return -1;
    return spr->height;
}
void spr_setOI(Sprite* spr,int ipos){
    if(!spr)return;
    spr->iPos=ipos;
}
void spr_setOJ(Sprite* spr, int jpos){
    if(!spr)return;
    spr->jPos=jpos;
}
int spr_getId(const Sprite* sp){
    if(!sp)return 0;
    return sp->id;
}
int spr_getTriginfo(Sprite*s, int** tr_id, int** i1,int** i2, int** j1, int** j2){
    if(!s)return -1;
    *tr_id=calloc(s->trsize,sizeof(int));
    *i1=calloc(s->trsize,sizeof(int));
    *i2=calloc(s->trsize,sizeof(int));
    *j1=calloc(s->trsize,sizeof(int));
    *j2=calloc(s->trsize,sizeof(int));
    for(int i=0;i<s->trsize;++i){
        *i1[i]=s->tr[i].i1;
        *j1[i]=s->tr[i].j1;
        *i2[i]=s->tr[i].i2;
        *j2[i]=s->tr[i].j2;
        *tr_id[i]=s->tr[i].tr_id;
    }
    return s->trsize;
}
