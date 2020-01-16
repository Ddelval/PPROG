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


/**
 * @brief Description of the position of a sprite
 * 
 * This structure is used to keep track of the position
 * that each sprite had when the Room was rendered for the last
 * time.
 * Thus, it allows us to update only the area that has canged
 * after every movement
 * 
 */
typedef struct _box{
    int i,j,w,h;
}box;

typedef struct _sprref{
    int i, j,id;
}sprref;
/**
 * @brief Information that we have to store to identify a trigger
 * 
 * This structure will allow us to keep track of a specific trigger
 * with just two integer values. The first one is the id that 
 * identifies the trigger in the dictionary of triggers. 
 * The second one is the index of the sprite that is causing it.
 * 
 */
typedef struct _trigger{
    int code;
    int spindex;
}trigger;

/**
 * @brief Information required to represent a map
 * 
 * The fields wid and hei contain the total width and 
 * height of the map while c_t, c_l, c_r and c_b are used to
 * limit the area that will be rendered on the screen. These
 * variables define:
 * c_t: top limit (included)
 * c_l: left limit (included)
 * c_r: right limit (excluded)
 * c_b: bottom limit (excluded)
 * 
 * The Room will store the sprites that are used to make
 * the background as well as all the static objects on backg.
 * The sprites that correspond to entities will be stored in 
 * overs. 
 * 
 * The structure also contains a Canvas (map) that stores the
 * result of rendering the map so that we can render the map 
 * less often. The other Canvas (shadows) contains the pixels
 * that should be rendered on top of the entities. That is to
 * say, the elements that cast a shadow on the entities.
 * 
 * 
 */
struct _Room{
    int id;
    char* name;
    int hei, wid;
    //Top and left are included, right and bottom are excluded
    int c_t, c_l, c_r, c_b;
    Pixel* backcol;
    sprref* backg;
    int backgsiz;
    int backpos;
    Sprite** overs;
    int* ent_typ;
    int overssiz;
    int overpos;
    box* ov;
    Canvas* map;
    Canvas* shadows;
    bool** colision;
    void* player;
    
    trigger*** trig;
};


Trigger** _room_getTriggersLoc(Room*r,trig_type tt, int i, int j, int* siz);

/*-----------------------------------------------------------------*/
/**
 * @brief Allocates all the memory required for the new room
 * 
 * @param id      Id of the room
 * @param name    Name of the room
 * @param hei     Total height of the room
 * @param wid     Total width of the room
 * @param backcol Background color of the room
 * @return        New room
 */
Room* room_ini(int id, const char* name,int hei, int wid, const Pixel* backcol){
    Room* r=calloc(1, sizeof(Room));
    if(!r)return NULL;
    
    r->name=calloc(strlen(name)+1, sizeof(char));
    if(!r->name)ret_free(r)
    strcpy(r->name, name);
    r->backcol=pix_copy(backcol);
    r->id =id;
    r->hei=hei;
    r->wid=wid;
    r->c_t=0;
    r->c_b=hei;
    r->c_l=0;
    r->c_r=wid;
    r->backgsiz=MEM_INI;

    r->backg=calloc(MEM_INI, sizeof(sprref));
    if(!r->backg)ret_free(r);

    r->overssiz=MEM_INI;
    r->overs=calloc(MEM_INI, sizeof(Sprite*));
    r->ent_typ=calloc(MEM_INI, sizeof(int));
    if(!r->overs||!r->ent_typ)ret_free(r);
    
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
               for(int w=0;w<MAX_TRIG;++w){
                   r->trig[i][j][w].code=-1;
                   r->trig[i][j][w].spindex=-1;
               }
           }
       }

    //Initalise backgroud
    r->map=canv_backGrnd(pix_retR(backcol), pix_retG(backcol), pix_retB(backcol), pix_retA(backcol), wid, hei);
    return r;

}

/*-----------------------------------------------------------------*/
/**
 * @brief Frees the memory allocated by the room
 * 
 * @param r Room to be freed
 */
void room_free(Room* r){
    if(!r)return;
    free(r->name);

    //Background sprite array
    if(r->backg){
        free(r->backg);
    }

    //Foreground sprite array
    if(r->overs){
        for(int i=0;i<r->overpos;++i)spr_free(r->overs[i]);
        free(r->overs);
    }
    free(r->ent_typ);

    //Colision bool array
    if(r->colision){
        for(int i=0;i<r->hei;++i)free(r->colision[i]);
        free(r->colision);
    }
    //Shadow 
    canv_free(r->shadows);
    //Trigger array
    if(r->trig){
        for(int i=0;i<r->hei;++i){
           for(int j=0;j<r->wid;++j){
               free(r->trig[i][j]);
           }
           free(r->trig[i]);
       }
    free(r->trig);
    }
    canv_free(r->map);
    free(r->ov);
    pix_free(r->backcol);
    free(r);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Loads a room from a file
 * 
 * The format of the room should be:
 * <id> <length of Name> <name>
 * <height> <width>
 * <background color> (as a Pixel)
 * <number of background sprites>
 * 
 * For each sprite:
 * <id> <ipos> <jpos>
 * 
 * @param f File from where the room will be read
 * @return  New room containing all the data
 */
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
        fscanf(f, "%d",&a);
        if(a==-1)break;
        fscanf(f, "%d %d",&b,&c);
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
        spr_free(s);
    }
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a sprite to the background layer
 * 
 * This is the function that should be called when we want to add
 * a sprite that does not proceed from an entity
 * 
 * @param r Room in which the sprite will be added
 * @param s Sprite to be added
 * @return  NULL in case of error
 */
Room* room_addBSprite(Room* r, const Sprite* s){
    if(!r||!s)return NULL;
    if(r->backpos==r->backgsiz){
        int nsiz=(int)ceil(r->backgsiz*MEM_INCREMENT);
        sprref* tmp=realloc(r->backg,nsiz*sizeof(sprref));
        if(!tmp)return NULL;
        r->backg=tmp;
        r->backgsiz=nsiz;
    }
    
    r->backg[r->backpos].i=spr_getOI(s);
    r->backg[r->backpos].j=spr_getOJ(s);
    r->backg[r->backpos].id=spr_getId(s);
    //spr_printTrigger(s);
    r->backpos++;
    canv_addOverlay(r->map, spr_getDispData(s), spr_getOI(s), spr_getOJ(s));
    spr_processCollisions(s,r->colision,r->wid,r->hei);
    spr_processShadows(s,r->shadows);
    room_processTriggers(r,s,r->backpos-1);
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Add a sprite to the top layer
 * 
 * This is the function that should be called to add sprites that
 * come from entities. In doing so, they will be able to interact
 * with the player in a different way (talk and combat)
 * 
 * @param r Room in which the sprite will be added
 * @param s Sprite to add
 * @return  -1 in case of error
 *          Otherwise, the index of the sprite in r->overs after 
 *          it has been added  
 */
int room_addOSprite(Room* r, const Sprite* s, int e_t){
    if(!r||!s)return -1;
    if(r->overpos==r->overssiz){
        int nsiz=(int)ceil(r->overssiz*MEM_INCREMENT);
        Sprite** tmp=realloc(r->overs,nsiz*sizeof(Sprite*));
        int* tmp2=realloc(r->ent_typ,nsiz*sizeof(int));
        if(!tmp||!tmp2)return -1;
        r->overs=tmp;
        r->ent_typ=tmp2;
        r->overssiz=nsiz;

        box* bb=realloc(r->ov, nsiz*sizeof(box));
        if(!bb) return -1;

    }

    r->overs[r->overpos]=spr_copy(s);
    if(!r->overs[r->overpos]){
        return -1;
    }
    r->ent_typ[r->overpos]=e_t;

    r->overpos++;
    return r->overpos-1;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the position of a background sprite
 * 
 * @param r     Room in which the sprite is
 * @param index Position of the sprite in r->backg
 * @param i     Pointer to a variable where the i-coordinate will be
 *              stored
 * @param j     Pointer to a variable where the j-coordinate will be
 *              stored
 * @return      NULL in case of error
 */
Room* room_getBSpritePos(const Room *r, int index, int* i, int *j){
    if(!r||index<0||index>=r->backpos||!i||!j)return NULL;
    *i=r->backg[index].i;
    *j=r->backg[index].j;
    return (Room*)r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the room
 * 
 * Note that this function will not redraw the base layer
 * 
 * @param r Room to be rendered
 * @return Graphical representation of the room
 */
Canvas* room_getRender(const Room* r){
    if(!r)return NULL;
    Canvas* canv=canv_copy(r->map);
    Canvas* tmp;
    for(int i=0;i<r->overpos;++i){
        if(!r->overs[i])continue;
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
    canv_addOverlay(canv,r->shadows,0,0);
    Canvas* res=canv_subCopy(canv,r->c_t,r->c_b,r->c_l,r->c_r);
    canv_free(canv);
    return res;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Modifies the position of a sprite of the top layer
 * 
 * Note that this function will change the position if possible 
 * and will renturn a value that represents the current position
 * of the sprite.
 * 
 * 
 * 
 * @param r      Room in which the sprite is
 * @param index  Index of the sprite in r->overs
 * @param i      New i-coordinate of the top left corner of the sprite
 * @param j      New j-coordinate of the top left corner of the sprite
 * @param scroll Whether or not the movement of this sprite should 
 *               affect the scrolling of the room
 * @return       Integer code:
 *               -1: error
 *                0: the position is completely free
 *                1: the sprite is in contact with the top border
 *                2: the sprite is in contact with the right border
 *                3: the sprite is in contact with the bottom border
 *                4: the sprite is in contact with the left border
 *                5: the sprite cannot move because of a collision 
 */
int room_modPos(Room* r, int index, int i, int j, bool scroll){
    if(!r||index>=r->overpos){
        return -1;
    }
    if(!scroll){
        //We only want to check against borders and collisions
        if(i+spr_getHeight(r->overs[index])>=r->hei||i<0||j<0||j+spr_getWidth(r->overs[index])>=r->wid) return 5;
        int a=spr_checkCollisions(r->overs[index],(const bool**)r->colision,r->wid,r->hei,i,j);
        if(a)return a;
        spr_setOJ(r->overs[index], j);
        spr_setOI(r->overs[index], i);
        return 0;
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
    aux=spr_checkCollisions(r->overs[index],(const bool**)r->colision,r->wid,r->hei,i,j);
    if(aux==-1)return -1;
    if(aux==1)return 5;
    spr_setOJ(r->overs[index], j);
    //fprintf(stderr,"%d, %d\n",i,j);
    fflush(stderr);
    spr_setOI(r->overs[index], i);
    return retval;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Increments the position of a sprite of the top layer
 * 
 * Note that this function will change the position if possible 
 * and will renturn a value that represents the current position
 * of the sprite.
 * 
 * 
 * 
 * @param r      Room in which the sprite is
 * @param index  Index of the sprite in r->overs
 * @param i      Increment in the i axis
 * @param j      Increment in the j axis
 * @param scroll Whether or not the movement of this sprite should 
 *               affect the scrolling of the room
 * @return       Integer code:
 *               -1: error
 *                0: the position is completely free
 *                1: the sprite is in contact with the top border
 *                2: the sprite is in contact with the right border
 *                3: the sprite is in contact with the bottom border
 *                4: the sprite is in contact with the left border
 *                5: the sprite cannot move because of a collision 
 */
int room_incPos(Room* r, int index, int i, int j,bool scroll){
    if(!r||index>=r->overpos){
        return -1;
    }
    return room_modPos(r,index,i+spr_getOI(r->overs[index]),j+spr_getOJ(r->overs[index]),scroll);
}
Trigger* room_checkCombat(Room* r,int index){
    if(!r)return NULL;
    int siz;
    Trigger* res;
    int i,j,w,h;
    i=spr_getOI(r->overs[index]);
    j=spr_getOJ(r->overs[index]);
    w=spr_getWidth(r->overs[index]);
    h=spr_getHeight(r->overs[index]);
    
    Trigger** t=_room_getTriggersLoc(r,COMBAT,i,j,&siz);
    if(siz) goto FOUND;
    t=_room_getTriggersLoc(r,COMBAT,i+h,j,&siz);
    if(siz) goto FOUND;
    t=_room_getTriggersLoc(r,COMBAT,i,j+w,&siz);
    if(siz) goto FOUND;
    t=_room_getTriggersLoc(r,COMBAT,i+h,j+w,&siz);
    if(siz) goto FOUND;

    return NULL;

FOUND:
    res=t[0];
    for(int i=1;i<siz;++i)tr_free(t[i]);
    free(t);
    return res;
}
/*-----------------------------------------------------------------*/
/**
 * @brief Sets the are of the window that will be rendered when 
 *        the rendering functions are called
 * 
 * This is used when the player moves to another section of the map
 * or when the room is set up initially.
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

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the modifications to the room, after the position 
 *        of an entity has changed
 * 
 * @param r      Room to be updated
 * @param index  Index of the sprite in overs that has changed
 * @param disp_i Vertical offset of the room (generally 0)
 * @param disp_j Horizontal offset of the room (generally 0)
 * @return Room* 
 */
Room* room_printMod(Room* r, int index, int disp_i, int disp_j){
    int i=index;
    int i1,i2,j1,j2;

    // Print the background where the sprite was before
    i1=max(r->ov[i].i,r->c_t);
    j1=max(r->ov[i].j,r->c_l);

    i2=min(r->ov[i].i+r->ov[i].h,r->c_b);
    j2=min(r->ov[i].j+r->ov[i].w,r->c_r);


    if(i1>=i2||j1>=j2)return r;
    Canvas* cbck=canv_copy(r->map);
    for(int i=0;i<r->overpos;++i){
        if(!r->overs[i])continue;
        if(i!=index) canv_addOverlay(cbck,spr_getDispData(r->overs[i]),spr_getOI(r->overs[i]),spr_getOJ(r->overs[i]));
    }
    Canvas* c=canv_subCopy(cbck,i1,i2,j1,j2);
    canv_print(stdout,c,i1-r->c_t+disp_i, j1-r->c_l+disp_j+1);
    canv_free(c);

    // Print the sprite on the screen
    i1=max(spr_getOI(r->overs[i]),r->c_t);
    j1=max(spr_getOJ(r->overs[i]),r->c_l);

    j2=min(spr_getOJ(r->overs[i])+spr_getWidth (r->overs[i]),r->c_r);
    i2=min(spr_getOI(r->overs[i])+spr_getHeight(r->overs[i]),r->c_b);

    Canvas* torender=canv_subCopy(spr_getDispData(r->overs[i]),i1-spr_getOI(r->overs[i]),i2-spr_getOI(r->overs[i]), j1-spr_getOJ(r->overs[i]), j2-spr_getOJ(r->overs[i]));
    Canvas* bc=canv_subCopy(cbck, i1, i2, j1, j2);
    Canvas* b2=canv_subCopy(r->shadows, i1, i2, j1, j2);
    canv_addOverlay(torender,b2,0,0);
    canv_printAllSolid(stdout,torender,bc,disp_i-r->c_t+i1, disp_j-r->c_l+j1+1);

    /**
     * Updates the position of the sprite in the array of boxes
     * In doing so, we will know the area that we have to cover the next
     * time that this function is called
     */
    box b;
    b.i=spr_getOI(r->overs[i]);
    b.j=spr_getOJ(r->overs[i]);
    b.w=canv_getWidth(spr_getDispData(r->overs[i]));
    b.h=canv_getHeight(spr_getDispData(r->overs[i]));
    r->ov[i]=b;


    canv_free(bc);
    canv_free(torender);
    canv_free(b2);
    canv_free(cbck);
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window by a factor of (i,j)
 * 
 * Usually the factors are (-1 <= i <= 1)
 * 
 * @param r Room to be scrolled
 * @param i   Horizontal percentage of the room that will
 *            be scrolled
 * @param j   Vertical percentage of the room that will
 *            be scrolled
 * 
 * @return  -1 if there was an error
 *           0 if there was no scrolling to be done
 *           1 if the room scrolled
 */
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


/*-----------------------------------------------------------------*/
/**
 * @brief Adds the trigges of sp to the room
 * 
 * @param r     Room to which the triggers are added
 * @param sp    Sprite that contains the triggers
 * @param index Index of sp in the corresponding array
 * @return      NULL in case of error
 */
Room* room_processTriggers(Room * r, const Sprite * sp, int index){
    if(!r||!sp)return NULL;
    int i0,j0;
    i0=spr_getOI(sp);
    j0=spr_getOJ(sp);
    int* tr_id;
    int* i1,*i2,*j1,*j2;
    int nsp=spr_getTriginfo(sp,&tr_id,&i1,&i2,&j1,&j2);
    for(int w=0;w<nsp;++w){
        for(int i=i0+i1[w];i<i0+i2[w];++i){
            if(i<0||i>=r->hei)continue;
            for(int j=j0+j1[w];j<j0+j2[w];++j){
                if(j<0||j>=r->wid)continue;
                int l=0;
                while(l<MAX_TRIG&&r->trig[i][j][l].spindex!=-1)l++;
                if(l!=MAX_TRIG){
                    r->trig[i][j][l].code=tr_id[w];
                    r->trig[i][j][l].spindex=index;
                }
            }
        }
    }
    free(i1);
    free(i2);
    free(j1);
    free(j2);
    free(tr_id);
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Get the trigers in position (i,j)
 * 
 * @param r   Room that contains the triggers
 * @param tt  Type of triggers that we are searching
 * @param i   I--coordinate
 * @param j   J-coordinate
 * @param siz Pointer to a variable where the size will of the
 *            resultin array will be stored
 * @return Trigger** 
 */
Trigger** _room_getTriggersLoc(Room*r,trig_type tt, int i, int j, int* siz){
    if(!r||i<0||j<0||i>=r->hei||j>=r->wid)return NULL;
    trigger* dat=r->trig[i][j];
    int cnt;
    for(cnt=0;cnt<MAX_TRIG&&dat[cnt].spindex!=-1;++cnt);
    Trigger** t=calloc(cnt+1,sizeof(Trigger*));
    int jj=0;
    for(int ii=0;ii<MAX_TRIG&&dat[ii].spindex!=-1;++ii){
        t[jj]=trdic_lookup(dat[ii].code);
        if(tr_getType(t[jj])==tt){
            tr_setSpr(t[jj],dat[ii].spindex);
            jj++;
        }
        else{
            tr_free(t[jj]);
        }
    }
    *siz=jj;
    return t;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Triggers associated to the position of the sprite
 *        r->overs[sp_index]
 * 
 * The trigger will be returned if it is associated to the
 * position of any of the four corners of the sprite
 * 
 * @param r         Room that contains the sprite
 * @param tt        Type of trigger that we are searching for
 * @param sp_index  Index of the sprite in the array overs
 * @param siz       Pointer to a variable where the size will of the
 *                  resultin array will be stored
 * @return          NULL if error
 *                  The array of triggers otherwise
 */
Trigger** room_getTriggers(Room*r,trig_type tt, int sp_index, int* siz){
    if(!r||sp_index>=r->overpos)return NULL;
    int cn[4];
    Trigger** tr[4];
    for(int i=0;i<4;++i)cn[i]=0;
    tr[0]=_room_getTriggersLoc(r,tt,spr_getOI(r->overs[sp_index]),spr_getOJ(r->overs[sp_index]),&cn[0]);
    tr[1]=_room_getTriggersLoc(r,tt,spr_getOI(r->overs[sp_index])+spr_getHeight(r->overs[sp_index]),spr_getOJ(r->overs[sp_index]),&cn[1]);
    tr[2]=_room_getTriggersLoc(r,tt,spr_getOI(r->overs[sp_index]),spr_getOJ(r->overs[sp_index])+spr_getWidth(r->overs[sp_index]),&cn[2]);
    tr[3]=_room_getTriggersLoc(r,tt,spr_getOI(r->overs[sp_index])+spr_getHeight(r->overs[sp_index]),spr_getOJ(r->overs[sp_index])+spr_getWidth(r->overs[sp_index]),&cn[3]);
    Trigger** tot=calloc(cn[0]+cn[1]+cn[2]+cn[3],sizeof(Trigger*));
    int index=0;
    for(int i=0;i<4;++i){
        if(!tr[i]) continue;
        for(int j=0;j<cn[i];++j){
            bool eq=false;
            for(int w=0;w<index;++w){
                if(tr_completeEqual(tr[i][j],tot[w])){
                    eq=true;
                    break;
                }
            }
            if(!eq){
                tot[index]=tr[i][j];
                index++;
            }
            
        }
    }
    for(int i=0;i<4;++i)free(tr[i]);
    *siz=index;
    return tot;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the map again
 * 
 * @param r Room to be recalculated
 * @return  NULL if error
 */
Canvas* room_redrawMap(Room*r){
    if(!r)return NULL;
    Canvas* b=canv_backGrnd(pix_retR(r->backcol),pix_retG(r->backcol),pix_retB(r->backcol),pix_retA(r->backcol),r->wid,r->hei);
    for(int i=0;i<r->backpos;++i){
        Sprite* sp=sdic_lookup(r->backg[i].id);
        if(canv_addOverlay(b,spr_getDispData(sp),r->backg[i].i,r->backg[i].j)==NULL){
            canv_free(b);
            return NULL;
        }
        spr_free(sp);
    }
    canv_free(r->map);
    r->map=b;
    return b;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Updates all the data in room after the deletion of a 
 *        sprite
 * 
 * @param r Room to be updated
 * @return  NULL if error
 */
Room* room_updateData(Room*r){

    // Clear the data in the arrays
    
    for(int i=0;i<r->hei;++i){
        for(int j=0;j<r->wid;++j){
            r->colision[i][j]=0;
            for(int w=0;w<MAX_TRIG;++w){
                   r->trig[i][j][w].code=-1;
                   r->trig[i][j][w].spindex=-1;
            }
        }
    }
    canv_free(r->shadows);
    r->shadows=canv_backGrnd(0,0,0,0,r->wid,r->hei);
    
    // Populates the data in the arrays
    for(int i=0;i<r->backpos;++i){
        Sprite* sp=sdic_lookup(r->backg[i].id);
        spr_setOI(sp,r->backg[i].i);
        spr_setOJ(sp,r->backg[i].j);
        spr_processCollisions(sp,r->colision,r->wid,r->hei);
        spr_processShadows(sp,r->shadows);
        room_processTriggers(r,sp,i);
        spr_free(sp);
    }
    for(int i=0;i<r->overpos;++i){
        if(!r->overs[i])continue;
        fprintf(stderr,"%d\n",r->ent_typ[i]);
        if(r->ent_typ[i]==1)continue; //It is the player
        if(r->ent_typ[i]==2){ //It is an enemy
            room_processTriggers(r,r->overs[i],i);
        }
        if(r->ent_typ[i]==3){ //It is an ally
            int oi,oj;
            int rad= ENTITY_TALK_RAD;
            oi=spr_getOI(r->overs[i]);
            oj=spr_getOJ(r->overs[i]);
            int tid=trdic_talksearch(i);
            if(tid>0)return NULL;
            for(int i2=oi-rad;i2<oi+rad+spr_getHeight(r->overs[i]);++i2){
                if(i2<0||i2>=r->hei)continue;
                for(int j=oj-rad;j<oj+rad*2+spr_getWidth(r->overs[i]);++j){
                    if(j<0||j>=r->wid)continue;
                    int l=0;
                    while(l<MAX_TRIG&&r->trig[i2][j][l].spindex!=-1)l++;
                    if(l!=MAX_TRIG){
                        r->trig[i2][j][l].code=tid;
                        r->trig[i2][j][l].spindex=i;
                    }
                }
            }
        }
        spr_processCollisions(r->overs[i],r->colision,r->wid,r->hei);
    }

    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the changes to the background after deletion of
 *        a sprite
 * 
 * @param r      Room to be updated
 * @param disp_i Horizontal offset of the screen
 * @param disp_j Vertical offset of the screen
 * @return       NULL if error
 */
Room* room_printModBackg(Room* r, int disp_i, int disp_j){
    if(!r)return NULL;
    Canvas* p=room_getRender(r);
    if(!room_updateData(r))return NULL;
    if(room_redrawMap(r)==NULL)return NULL;
    Canvas * c=room_getRender(r);
    canv_printDiff(stdout,c,p,disp_i,disp_j);
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Removes the sprite in position index from r->backg
 * 
 * @param r     Room to be updated
 * @param index Index of the sprite to be removed
 * @return      NULL if error
 */
Room* room_removeB(Room* r, int index){
    if(!r||index>=r->backpos)return NULL;
    for(int i=index;i<r->backpos-1;++i){
        r->backg[i]=r->backg[i+1];
    }
    r->backg[r->backpos-1].id=-1;
    r->backpos--;
    
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Removes a sprite from the array overs
 * 
 * @param r     Room from which the sprite will be deleted
 * @param index Index of the sprite in the array overs
 * @return      NULL if error
 */
Room* room_removeOver(Room* r, int index){
    if(!r||index>=r->overpos)return NULL;
    spr_free(r->overs[index]);
    r->overs[index]=NULL;
    room_updateData(r);
    Canvas* c=room_getRender(r);
    canv_print(stdout,c,0,0);
    canv_free(c);
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Displays the building interface
 * 
 * @param r      Room in which the building will be placed
 * @param spid   Sprite id of the building
 * @param ai     Initial vertical coordinate of the sprite
 * @param aj     Initial horizontal coordinate of the sprite
 * @param room_i Horizontal offset of the room
 * @param room_j Vertical offset of the room
 * @return       NULL if error
 */
Room* room_buildingInterface(Room*r, int spid,int ai, int aj,int room_i, int room_j){
    if(!r)return NULL;
    Canvas* aux=NULL;
    Canvas* fin=NULL;
    Sprite* s=sdic_lookup(spid);
    Canvas* base=room_getRender(r);
    bool buil=false;
    
    int ipos,jpos;
    ipos = ai-r->c_t;
    jpos = aj-r->c_l;

    Pixel*p;
    Pixel* red=pix_ini(255,150,150,255);
    Pixel* green=pix_ini(150,255,150,255);
    Pixel* rred=pix_ini(255,0,0,255);

    int pi=spr_getOI(r->overs[0])-r->c_t;
    int pj=spr_getOJ(r->overs[0])-r->c_l;
    int pw=spr_getWidth(r->overs[0]);
    int ph=spr_getWidth(r->overs[0]);

    if(!red||!green||!rred||!s||!base)goto END;

    int ** placement=calloc(r->hei,sizeof(int*));
    if(!placement){
        goto END;
    }
    for(int i=0;i<r->hei;++i){
        placement[i]=calloc(r->wid,sizeof(int));
        if(!placement[i]){
            goto END;
        }
        for(int j=0;j<r->wid;++j)placement[i][j]=!pix_transparent(canv_getPixel(r->shadows,i,j));
    }

    if(ipos<0)ipos=0;
    if(ipos+spr_getHeight(s)>=r->c_b-r->c_t)ipos=r->c_b-r->c_t-spr_getHeight(s)-1;
    if(jpos<0)jpos=0;
    if(jpos+spr_getWidth(s)>=r->c_r-r->c_l)jpos=r->c_r-r->c_l-spr_getWidth(s)-1;
    p= spr_checkCollisions(s,(const bool**)placement,r->wid,r->hei,ipos+r->c_t,jpos+r->c_l)? red:green;
    if((pi+ph>=ipos&&pi<=ipos+ spr_getHeight(s)) && (pj+pw>=jpos&&pj<=jpos+ spr_getWidth(s)))p=red;
    aux=canv_filter(spr_getDispData(s),p);
    canv_darken(aux,1.2);
    fin=canv_Overlay(base,aux,ipos,jpos);
    canv_print(stdout,fin,0,0);
    
    while(1){
        char ch=getch1();
        if(ch=='D'){
            jpos+=10;
        }
        if(ch=='A'){
            jpos-=10;
        }
        if(ch=='S'){
            ipos+=10;
        }
        if(ch=='W'){
            ipos-=10;
        }
        if(ch=='J'){
            bool b=(pi+ph>=ipos&&pi<=ipos+ spr_getHeight(s)) && (pj+pw>=jpos&&pj<=jpos+ spr_getWidth(s));
            if(!b&&!spr_checkCollisions(s,(const bool**)placement,r->wid,r->hei,ipos+r->c_t,jpos+r->c_l)){
                spr_setOI(s,ipos+r->c_t);
                spr_setOJ(s,jpos+r->c_l);
                room_addBSprite(r,s);
                buil=true;
                break;
            }
            else{
                p= rred;
                Canvas* aux=canv_filter(spr_getDispData(s),p);
                canv_darken(aux,1.2);
                Canvas*cc =canv_Overlay(base,aux,ipos,jpos);
                canv_printDiff(stdout,cc,fin,room_i,room_j);
                canv_free(fin);
                fin=cc;
            }
        }
        if(ch=='Q')break;
        if(ipos<0)ipos=0;
        if(ipos+spr_getHeight(s)>=r->c_b-r->c_t)ipos=r->c_b-r->c_t-spr_getHeight(s)-1;
        if(jpos<0)jpos=0;
        if(jpos+spr_getWidth(s)>=r->c_r-r->c_l)jpos=r->c_r-r->c_l-spr_getWidth(s)-1;
        p= spr_checkCollisions(s,(const bool**)placement,r->wid,r->hei,ipos+r->c_t,jpos+r->c_l)? red:green;
        if((pi+ph>=ipos&&pi<=ipos+ spr_getHeight(s)) && (pj+pw>=jpos&&pj<=jpos+ spr_getWidth(s)))p=red;
        canv_free(aux);
        aux=canv_filter(spr_getDispData(s),p);
        canv_darken(aux,1.2);
        Canvas* cc =canv_Overlay(base,aux,ipos,jpos);
        canv_printDiff(stdout,cc,fin,room_i,room_j);
        canv_free(fin);
        fin=cc;
    }
END:
    spr_free(s);
    pix_free(red);
    pix_free(rred);
    pix_free(green);
    canv_free(fin);
    canv_free(aux);

    //Print the initial room
    Canvas* res=room_getRender(r);
    canv_printDiff(stdout,res,fin,room_i,room_j);
    canv_free(res);
    
    return buil? r:NULL;

}

/*-----------------------------------------------------------------*/
/**
 * @brief Modifies the room when an ally has been added
 * 
 * @param r            Room to be updated 
 * @param e            Entity that represents the ally
 * @param s            Sprite of the ally
 * @param ally_index   Index of the ally in the sprite array
 * @param rad          Radius of the interactions with the player
 * @return             NULL if error
 */
Room* room_processAlly(Room* r, void *e,const Sprite* s,int ally_index, int rad){
    if(!r||!s)return NULL;
    int oi,oj;
    oi=spr_getOI(r->overs[ally_index]);
    oj=spr_getOJ(r->overs[ally_index]);

    Trigger *t =tr_createTalk(e,ally_index);
    if(!t)return NULL;
    int tid=trdic_insert(t);
    tr_free(t);
    if(tid>0)return NULL;


    for(int i=oi-rad;i<oi+rad+spr_getHeight(s);++i){
        if(i<0||i>=r->hei)continue;
        for(int j=oj-rad;j<oj+rad*2+spr_getWidth(s);++j){
            if(j<0||j>=r->wid)continue;
            int l=0;
            while(l<MAX_TRIG&&r->trig[i][j][l].spindex!=-1)l++;
            if(l!=MAX_TRIG){
                r->trig[i][j][l].code=tid;
                r->trig[i][j][l].spindex=ally_index;
            }
        }
    }
    spr_processCollisions(r->overs[ally_index],r->colision,r->wid,r->hei);
    return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Adds an enemy to the room
 * 
 * Note that the sprite will already be in the overs array sice it 
 * is added to that one as soon as the entity is initalized.
 * 
 * @param r             Room in which the enemy will be inserted
 * @param enemy_index   Index of the sprite of the enemy in the 
 *                      array overs
 * @return Room* 
 */
Room* room_processEnemy(Room* r,void* e,int enemy_index, int i1,int i2,int j1,int j2){
    if(!r)return NULL;
    Trigger* t=tr_createAttack(e,enemy_index);
    int id=trdic_insert(t);
    tr_free(t);
    spr_addTrigger(r->overs[enemy_index],id,i1,i2,j1,j2);
    room_processTriggers(r,r->overs[enemy_index],enemy_index);
    spr_processCollisions(r->overs[enemy_index],r->colision,r->wid,r->hei);
    return r;
}
/*-----------------------------------------------------------------*/
///Gets the name of the room
char* room_getName(Room* r){
    if(!r)return NULL;
    char * c=calloc(strlen(r->name)+1,sizeof(char));
    if(!c)return NULL;
    strcpy(c,r->name);
    return c;
}

/*-----------------------------------------------------------------*/
/// Gets the id of the room
int room_getId(Room* r){
    return r? r->id:-1;
}

/*-----------------------------------------------------------------*/
/// Gets the height of the room
int room_getHeight(Room*r){
    return r? r->hei:-1;
}

/*-----------------------------------------------------------------*/
/// Gets the width of the room
int room_getWidth(Room*r){
    return r? r->wid:-1;
}

/*-----------------------------------------------------------------*/
/// Gets a copy of the sprite in position index of the over sprites
Sprite* room_getSpriteO(Room* r, int index){
    if(!r||index>=r->overpos)return NULL;
    return spr_copy(r->overs[index]);
}
/*-----------------------------------------------------------------*/
/// Sets the i-coordinate of the sprite r->overs[index]
Room* room_setSpriteI(Room* r,int index, int i){
    if(!r||r->overpos<=index)return NULL;
    spr_setOI(r->overs[index],i);
    return r->overs[index]? r:NULL;
}

/*-----------------------------------------------------------------*/
/// Sets the j-coordinate of the sprite r->overs[index]
Room* room_setSpriteJ(Room* r,int index, int j){
    if(!r||r->overpos<=index)return NULL;
    spr_setOJ(r->overs[index],j);
    return r->overs[index]? r:NULL;
}

/*-----------------------------------------------------------------*/
/// Set height and width of the display area
Room* room_setHW(Room* r, int he,int wi){
    if(!r)return NULL;
    r->c_b=min(r->hei,r->c_t+he);
    r->c_r=min(r->wid,r->c_l+wi);
    return r;
}
/*-----------------------------------------------------------------*/
/// Set the player in the room
Room* room_setPlayer(Room* r, void* e){
    if(!r)return NULL;
    r->player=e;
    return r;
}
/*-----------------------------------------------------------------*/
/**
 * @brief Copies the room
 * 
 * @param r     Room to be copied
 * @return      Identical copy of r
 */
Room* room_copy(const Room* r){
    if(!r)return NULL;
    Room* r2=room_ini(r->id,r->name,r->hei,r->wid,r->backcol);
    r2->c_t=r->c_t;
    r2->c_l=r->c_l;
    r2->c_r=r->c_r;
    r2->c_b=r->c_b;
    r2->player=r->player;
    for(int i=0;i<r->backpos;++i){
        Sprite* ss=sdic_lookup(r->backg[i].id);
        spr_setOI(ss,r->backg[i].i);
        spr_setOJ(ss,r->backg[i].j);
        room_addBSprite(r2,ss);
        spr_free(ss);
    }
    for(int i=0;i<r->overpos;++i){
        room_addOSprite(r2,r->overs[i],r->ent_typ[i]);
        spr_processCollisions(r->overs[i],r2->colision,r2->wid,r2->hei);
    }
    for(int i=0;i<r->hei;++i){
        for(int j=0;j<r->wid;++j){
            memcpy(r2->trig[i][j],r->trig[i][j],MAX_TRIG*sizeof(trigger));
        }
    }
    return r2;
}


