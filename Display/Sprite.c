//  ppro
//	Sprite.c
//  Created by David del Val on 06/10/2019
//
//

#include "Sprite.h"
#define ret_free(spr) {spr_free(spr); return NULL;}

/**
 * @brief Stores the information related with the triggers
 * 
 * Triggers will only affect rectangles of the canvas
 * 
 * (i1,j1) top left corner of the box
 * (i2,j2) bottom right corner of the box
 * tr_id   id of the trigger associated with this ara
 * 
 * The id of the trigger will be used to get the trigger
 * from the trigger dictionary
 * 
 */
typedef struct {
    int tr_id;
    int i1,i2,j1,j2;
}trigdata;


/**
 * @brief A object that can move on the screen
 * 
 * A sprite has the graphical representation of an 
 * object or entity, the collision and shadow data
 * associated with it and the position on the screen
 * 
 * Some of this fields will be unused when 
 * sprites are used as icons. 
 * However, this implementation will allow us to
 * avoid having a separate dictionary for sprites
 * and canvases.
 * 
 */
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

/*-----------------------------------------------------------------*/
/**
 * @brief Initialises a sprite
 * 
 * All the memory will be allocated, however,
 * the canvas will still be NULL
 * 
 * @param id        Id of the new sprite
 * @param width     Width of the new sprite
 * @param height    Height of the sprite
 * @return          NULL if error
 *                  The new sprite otherwise 
 */
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

/*-----------------------------------------------------------------*/
/**
 * @brief Copies the sprite
 * 
 * @param spr   Source sprite
 * @return      Copy of the sprite
 */
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

/*-----------------------------------------------------------------*/
/**
 * @brief Loads a sprite from the file
 * 
 * The format is the following:
 * <id> <format>
 * <canvas>
 * 
 * If format is 1, the file will also contain:
 * <collision_canvas>
 * 
 * If format is 0, the collision canvas will 
 * be the canvas.
 * 
 * If format is 2, the collision and shadow
 * will be empty 
 * 
 * @param f 
 * @return Sprite* 
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
                if(i*100>h*sh)res->collision[i][j]=!pix_halfTransparent(canv_getPixel(colldata,i,j));
                else          res->shadow[i][j]   =!pix_halfTransparent(canv_getPixel(colldata,i,j));
            }
        }
        free(colldata);
    }
    else if(ff==0){
        for(int i=0;i<h;++i){
            for(int j=0;j<w;++j){
                if(i*100>h*sh)res->collision[i][j]=!pix_halfTransparent(canv_getPixel(c,i,j));
                else          res->shadow[i][j]   =!pix_halfTransparent(canv_getPixel(c,i,j));
            }
        }
    }
    //Compatibility
    fscanf(f, "%*d");
    return res;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Frees the memory allocated for the sprite
 * 
 * @param sp Sprite to be freed
 */
void spr_free(Sprite* sp){
    if(!sp)return;
    if(sp->collision){
        for(int i=0;i<sp->height;++i){
            free(sp->collision[i]);
        }
        free(sp->collision);
    }
    if(sp->shadow){
        for(int i=0;i<sp->height;++i){
            free(sp->shadow[i]);
        }
        free(sp->shadow);
    }
    free(sp->tr);
    canv_free(sp->canvas);
    free(sp);
}
/*-----------------------------------------------------------------*/
/**
 * @brief Adds a trigger to the sprite
 * 
 * @param s       Sprite to which the trigger will be added
 * @param tr_id   Id of the trigger that will be stored in
 *                the trigger
 * @param i1      Horizontal starting position
 * @param i2      Horizontal ending position
 * @param j1      Vertical starting position
 * @param j2      Vertical ending position
 * @return        NULL if error
 */
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

}

/*-----------------------------------------------------------------*/
/**
 * @brief Adds the collition data of the sprite to rarr
 * 
 * @param s     Sprite whose collition data will be added to arr
 * @param rarr  Array of collision data in the room
 * @param rwid  Height of the room
 * @param rhei  Width of the room
 * @return      NULL if error
 */
Sprite* spr_processCollisions(const Sprite* s,bool** rarr,int rwid, int rhei){
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
    return (Sprite*)s;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Adds the shadow data of the sprite to shad
 * 
 * @param s     Sprite whose shadow data will be added to shad
 * @param shad  Canvas with the shadow data
 * @return      NULL if error
 */
Sprite* spr_processShadows(const Sprite* s,Canvas* shad){
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
            if(s->shadow[i][j]||s->collision[i][j]){
                assert(canv_setPixel(shad,canv_getPixel(s->canvas,i,j),i2,j2)!=NULL);
            }
        }
    }
    return (Sprite*)s;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Checks if the moving the sprite to the new position will
 *        make it collide with another object
 * 
 * @param s     Sprite that we are moving
 * @param rarr  Array of collision data of the room
 * @param rwid  Width of the room
 * @param rhei  Height of the room
 * @param ni    New horizontal position of the sprite
 * @param nj    New vertical position of the sprite
 * @return      -1 if error
 *               0 if there is no collition
 *               1 if there is a  collition 
 */
int spr_checkCollisions(const Sprite*s, const bool**rarr,int rwid,int rhei, int ni,int nj){
    if(!rarr)return -1;
    int i2,j2;
    for(int i=0;i<s->height;++i){
        i2=i+ni;
        if(i2>=rhei)break;
        for(int j=0;j<s->width;++j){
            j2=j+nj;
            if(j2>=rwid)break;
            if(!s->collision[i][j])continue;
            
            if(rarr[i2][j2]) return 1;
            
        }
    }
    return 0;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Deletes all the triggers in the sprite
 * 
 * @param sp Sprite whose triggers will be deleted
 */
void spr_clearTrig(Sprite* sp){
    if(!sp)return;
    sp->trsize=0;
}

/*-----------------------------------------------------------------*/
/** GETTERS **/
const Canvas* spr_getDispData(const Sprite* spr){
    if(!spr)return NULL;
    return spr->canvas;
}
int spr_getOI(const Sprite* spr){
    if(!spr)return 0;
    return spr->iPos;
}
int spr_getOJ(const Sprite* spr){
    if(!spr)return 0;
    return spr->jPos;
}
int spr_getWidth(const Sprite* spr){
    if(!spr)return -1;
    return spr->width;
}
int spr_getHeight(const Sprite* spr){
    if(!spr) return -1;
    return spr->height;
}

int spr_getId(const Sprite* sp){
    if(!sp)return 0;
    return sp->id;
}
int spr_getTriginfo(const Sprite*s, int** tr_id, int** i1,int** i2, int** j1, int** j2){
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

/*-----------------------------------------------------------------*/
/** SETTERS **/
void spr_setCoordinates(Sprite* sp,int i,int j){
    if(!sp)return;
    sp->iPos=i;
    sp->jPos=j;
}
void spr_setOI( Sprite* spr,int ipos){
    if(!spr)return;
    spr->iPos=ipos;
}
void spr_setOJ(Sprite* spr, int jpos){
    if(!spr)return;
    spr->jPos=jpos;
}
void spr_setId(Sprite* sp,int id){
    if (sp)sp->id=id;
}