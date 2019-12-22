/* Canvas.c */

#include "Canvas.h"

struct _Canvas {
    Pixel*** data;
    int wid;
    int hei;
};

/* Prototypes*/
char** _canv_render(const Canvas* c,int wid, int hei);
bool _transparentColumn(const Canvas * c, int j);


/*-----------------------------------------------------------------*/
/// Free the allocated memory
void canv_free(Canvas* c){
    if(!c)return;
    if(c->data){
        for(int i=0;i<c->hei;++i){
            if(!(c->data[i]))continue;
            for(int j=0;j<c->wid;++j){
                pix_free(c->data[i][j]);
            }
        free(c->data[i]);
        }
        free(c->data);
    }
    free(c);

}

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new canvas
 * 
 * Note that this function will not initalise the array of pixels.
 * It will only allocate the memory for it.
 * 
 * @param wid Width of the new canvas
 * @param hei Height of the new canvas
 * @return New canvas
 */
Canvas* canv_ini(int wid,int hei){
    Canvas * c=calloc(1,sizeof(Canvas));
    if(!c)return NULL;
    c->wid=wid;
    c->hei=hei;
    c->data= calloc(hei,sizeof(Pixel**));
    if(!c->data){
        free(c);
        return NULL;
    }
    for(int i=0;i<hei;++i){
        c->data[i]=calloc(wid,sizeof(Pixel*));
        if(!c->data[i]){
            for(int j=0;j<i;++j){
                free(c->data[j]);
            }
            free(c->data);
            free(c);
            return NULL;
        }
    }
    return c;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Loads a canvas from a file
 * 
 * The format should be the following:
 * (width) (height)
 * [r,g,b,a] [r,g,b,a]...
 * 
 * @param f File from which the data will be read
 * @return Pointer to the new canvas
 */
Canvas* canv_load(FILE* f){
    if(!f)return NULL;
    int wid,hei;
    fscanf(f,"%d %d",&wid,&hei);

    Canvas* c = canv_ini(wid,hei);
    if(!c)return NULL;

    for(int i=0;i<c->hei;++i){
        for(int j=0;j<c->wid;++j){
            char c1='.';
            while(c1!='['){
                fscanf(f,"%c",&c1);
            }
            c->data[i][j]=pix_load(f);
            if(!(c->data[i][j])){
                canv_free(c);
                return NULL;
            }
        }
    }

    return c;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new canvas of a given color and dimensions
 * 
 * @param r         red component
 * @param g         green component
 * @param b         blue component
 * @param a         alpha value
 * @param width     width of the canvas
 * @param height    height of the canvas
 * @return Canvas*  to the new canvas
 */
Canvas* canv_backGrnd(int r, int g, int b, int a, int width, int height){
    Canvas* canres= canv_ini(width, height);
    if(!canres){
        return NULL;
    }
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            canres->data[i][j]=pix_ini(r, g, b, a);
            if(!(canres->data[i][j])){
                canv_free(canres);
                return NULL;
            }
        }
    }
    return canres;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Copies the given canvas
 * 
 * @param bas   Canvas to be copied
 * @return      New anvas with exactly the same data
 */
Canvas* canv_copy (const Canvas* bas){
    if(!bas)return NULL;
    Canvas* res=canv_ini(bas->wid, bas->hei);
    if(!res)return NULL;
    for(int i=0;i<bas->hei;++i){
        for(int j=0;j<bas->wid;++j){
            res->data[i][j]=pix_copy(bas->data[i][j]);
            if(!(res->data[i][j])){
                canv_free(res);
                return NULL;
            }
        }
    }
    return res;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Returns a copy of a section of the given Canvas
 * 
 * @param bas   Source canvas
 * @param i2    i starting index (included)
 * @param i1    i ending   index (excluded) 
 * @param j1    j starting index (included)
 * @param j2    j ending   index (excluded)
 * @return      New canvas containing a copy of the given section
 */
Canvas* canv_subCopy (const Canvas* bas,int i1,int i2,int j1,int j2){
    if(!bas)return NULL;
    int hei,wid;
    hei=i2-i1;
    wid=j2-j1;
    int i0,j0;
    j0=i0=0;
    if(i1<0)i0=-i1;
    if(j1<0)j0=-j1;
    if(i1+hei>bas->hei)hei=bas->hei-i1;
    if(j1+wid>bas->wid)wid=bas->wid-j1;
    Canvas* res=canv_ini(wid, hei);
    if(!res) return NULL;
    for(int i=i0;i<hei;++i){
        for(int j=j0;j<wid;++j){
            res->data[i][j]=pix_copy(bas->data[i1+i][j1+j]);
            if(!(res->data[i][j])){
                canv_free(res);
                return NULL;
            }
        }
    }
    return res;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases horizontally
 * 
 * That is to say the resulting canvas looks like:
 * 
 * WWWWWWW
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWW
 * 
 * Where the 'W' repesent the west canvas and the 'E'
 * represents the east canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered vertically.
 * 
 * @param west  Canvas to the left
 * @param east  Canvas to the right
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendH(const Canvas* west, const Canvas* east){
    if(!west||!east)return NULL;
    int hei=max(west->hei, east->hei);
    Canvas * l1=canv_AdjustCrop(west, west->wid, hei);
    if(!l1){
        return NULL;
    }
    Canvas* r1=canv_AdjustCrop(east, east->wid, hei);
    if(!r1){
        canv_free(l1);
        return NULL;
    }
    Canvas* res=canv_ini(east->wid+west->wid, hei);
    if(!res){
        canv_free(l1);
        canv_free(r1);
        return NULL;
    }
    for(int i=0;i<hei;++i){
        for(int j=0;j<west->wid;++j){
            res->data[i][j]=pix_copy(l1->data[i][j]);
            if(!res->data[i][j]){
                canv_free(l1);
                canv_free(r1);
                canv_free(res);
                return NULL;

            }
        }
        for(int j=0;j<east->wid;++j){
            res->data[i][j+west->wid]=pix_copy(r1->data[i][j]);
            if(!res->data[i][j+west->wid]){
                canv_free(l1);
                canv_free(r1);
                canv_free(res);
                return NULL;

            }
        }
    }
    canv_free(r1);
    canv_free(l1);
    return res;

}

/*-----------------------------------------------------------------*/
/**
 * @brief   Appends two canvases horizontally without aligning
 *          the smaller one
 * 
 * That is to say the resulting canvas looks like:
 * 
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWW
 * WWWWWWW
 * 
 * Where the 'W' repesent the west canvas and the 'E'
 * represents the east canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be aligned relative to its top.
 * 
 * @param west  Canvas to the left
 * @param east  Canvas to the right
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendHNL(const Canvas* west, const Canvas* east){
    if(!west||!east)return NULL;
    int hei=max(west->hei, east->hei);
    Canvas* res=canv_ini(east->wid+west->wid, hei);
    if(!res) return NULL;
    
    for(int i=0;i<hei;++i){
        for(int j=0;j<west->wid;++j){
            if(i<west->hei)res->data[i][j]=pix_copy(west->data[i][j]);
            else res->data[i][j]=pix_newTransparent();
            if(!res->data[i][j]){
                canv_free(res);
                return NULL;
            }
        }
        for(int j=0;j<east->wid;++j){
            if(i<east->hei)res->data[i][j+west->wid]=pix_copy(east->data[i][j]);
            else res->data[i][j+west->wid]=pix_newTransparent();
            if(!res->data[i][j+west->wid]){
                canv_free(res);
                return NULL;

            }
        }
    }
    return res;

}

/*-----------------------------------------------------------------*/
/**
 * @brief Appends the east canvas to the right of the west one.
 *        In this case the west canvas will reflect the changes
 * 
 * That is to say the resulting canvas looks like:
 * 
 * WWWWWWW
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWWEEEEE
 * WWWWWWW
 * 
 * Where the 'W' repesent the west canvas and the 'E'
 * represents the east canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered vertically.
 * 
 * @remark  The result of the operation will be stored in the west
 *          canvas. The return value is only used to report errors.
 * 
 * @param west  Canvas to the left
 * @param east  Canvas to the right
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendHI(Canvas* west, const Canvas* east){
    if(!east||!west){
        return NULL;
    }
    Canvas* res= canv_appendH(west, east);
    for(int i=0;i<west->hei;++i){
        if(!(west->data[i]))continue;
        for(int j=0;j<west->wid;++j){
            pix_free(west->data[i][j]);
        }
        free(west->data[i]);
    }
    free(west->data);
    west->data=res->data;
    res->data=NULL;
    west->hei=res->hei;
    west->wid=res->wid;
    canv_free(res);
    return west;
}




/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases vertically
 * 
 * That is to say the resulting canvas looks like:
 * 
 *    NNNNNN
 *    NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 * 
 * Where the 'N' repesent the north canvas and the 'S'
 * represents the south canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered horizontally.
 * 
 * @param north  Canvas on the top
 * @param south  Canvas on the bottom
 * @return      New canvas with the result of the operation
 */
Canvas* canv_appendV(const Canvas* north, const Canvas* south){
    if(!north||!south)return NULL;
    int wid=max(north->wid, south->wid);
    Canvas * n1=canv_AdjustCrop(north, wid,north->hei);
    if(!n1){
        return NULL;
    }
    Canvas* s1=canv_AdjustCrop(south,wid,south->hei);
    if(!s1){
        canv_free(n1);
        return NULL;
    }
    Canvas* res=canv_ini(wid,north->hei+south->hei);
    if(!res){
        canv_free(n1);
        canv_free(s1);
        return NULL;
    }
    for(int i=0;i<north->hei;++i){
        for(int j=0;j<wid;++j){
            res->data[i][j]=pix_copy(n1->data[i][j]);
            if(!res->data[i][j]){
                canv_free(n1);
                canv_free(s1);
                canv_free(res);
                return NULL;

            }
        }
    }
    for(int i=0;i<south->hei;++i){
        for(int j=0;j<wid;++j){
            res->data[north->hei+i][j]=pix_copy(s1->data[i][j]);
            if(!res->data[north->hei+i][j]){
                canv_free(n1);
                canv_free(s1);
                canv_free(res);
                return NULL;

            }
        }
    }
    canv_free(s1);
    canv_free(n1);
    return res;

}

/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases vertically and stores the result
 *        in the top one
 * 
 * That is to say the resulting canvas looks like:
 * 
 *    NNNNNN
 *    NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 * 
 * Where the 'N' repesent the north canvas and the 'S'
 * represents the south canvas.
 * 
 * If they do not have the same height, the smaller one will 
 * be centered horizontally.
 * 
 * @param north  Canvas on the top
 * @param south  Canvas on the bottom
 * @return       A NULL pointer in case of error, north otherwise
 */
Canvas* canv_appendVI(Canvas* north, const Canvas* south){
    if(!north||!south){
        return NULL;
    }
    Canvas*res=canv_appendV(north, south);

    // Get rid of the pixels in north
    for(int i=0;i<north->hei;++i){
        if(!(north->data[i]))continue;
        for(int j=0;j<north->wid;++j){
            pix_free(north->data[i][j]);
        }
        free(north->data[i]);
    }
    free(north->data);
    north->data=res->data;
    north->hei=res->hei;
    north->wid=res->wid;
    res->data=NULL;
    canv_free(res);
    return north;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Appends two canvases vertically using the given alignment 
 *        and stores the result in the top one
 * 
 * When al=LEFT, the resulting canvas looks like:
 * 
 *  NNNNNN
 *  NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 *
 * When al=RIGHT, the resulting canvas looks like:
 * 
 *      NNNNNN
 *      NNNNNN
 *  SSSSSSSSSS  
 *  SSSSSSSSSS
 *  SSSSSSSSSS
 * Where the 'N' repesent the north canvas and the 'S'
 * represents the south canvas.
 * 
 * 
 * @param north Canvas on the top
 * @param south Canvas on the bottom
 * @param al    Alignement of the canvases
 * @return      A NULL pointer in case of error, north otherwise
 */
Canvas* canv_appendVIA(Canvas* north,const Canvas* south,CAlign al){
    if(!north||!south)return NULL;
    int wid=max(north->wid,south->wid);

    Canvas* gn=canv_backGrnd(0,0,0,0,wid-north->wid,north->hei);
    Canvas* gs=canv_backGrnd(0,0,0,0,wid-south->wid,south->hei);
    Canvas* nn,*ns;
    if(al==RIGHT){
        nn=canv_appendH(gn,north);
        ns=canv_appendH(gs,south);
    }
    
    if(al==LEFT){
        nn=canv_appendH(north,gn);
        ns=canv_appendH(south,gs);
    }
    if(!nn||!ns){
        canv_free(gn);
        canv_free(gs);
        return NULL;
    }
    Canvas* res=canv_appendV(nn,ns);
    if(!res){
        canv_free(gn);
        canv_free(gs);
        canv_free(nn);
        canv_free(ns);
        return NULL;
    }
    for(int i=0;i<north->hei;++i){
        for(int j=0;j<north->wid;++j){
            pix_free(north->data[i][j]);
        }
    }
    free(north->data);
    north->wid=res->wid;
    north->hei=res->hei;
    north->data=res->data;
    res->data=NULL;
    canv_free(gn);
    canv_free(nn);
    canv_free(gs);
    canv_free(ns);
    canv_free(res);
    return north;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a margin to a canvas. The padding will be transparent.
 * 
 * @param src       Canvas that will be placed in the center
 * @param top       Margin in the top
 * @param right     Margin in the right
 * @param bottom    Margin in the bottom
 * @param left      Margin in the left
 * @return          New canvas with the given canvas and the margins
 */
Canvas* canv_addMargin (const Canvas *src, int top, int right, int bottom, int left){
    if(!src)return NULL;
    Canvas* ret=canv_backGrnd(0, 0, 0, 0, src->wid+right+left, src->hei+top+bottom);
    canv_addOverlay(ret, src, top, left);
    return ret;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Split a canvas into smaller sub-canvases making vertical separations
 * 
 * This function will split the canvas into pieces that are separated by 
 * transparent horizontal gaps.
 * This can be particularly useful when we want to load more than one 
 * canvas in a single file. 
 * 
 * For instance, if a picture contains several leters:
 * a b c d e
 * This function will return each letter in a separete canvas
 * 
 * @param src       Canvas to be splitted
 * @param nelem     Pointer to an integer where the number
 *                  of returned elements will be stored
 * @return          An array of Canvas* that contains all non-transparent
 *                  sub-sections of the given canvas separated.
 */
Canvas ** canv_VSplit(const Canvas* src, int* nelem){
    if(!src||!nelem)return NULL;
    /* Count the ammount of sub elements*/
    int cnt=0;
    int prev=true;
    for(int j=0;j<src->wid;++j){
        bool b=_transparentColumn(src, j);
        if(!b&&prev){

            cnt++;
        }
        prev=b;
    }
    *nelem=cnt;
    /* Splits the sub elements and stores them in elements */
    Canvas** elements=calloc(cnt+2, sizeof(Canvas*));
    int lindex=0;
    int pindex=0;
    for(int j=0;j<src->wid;++j){
        if(_transparentColumn(src, j)){
            if(pindex<j-1){
                elements[lindex]=canv_subCopy(src, 0, src->hei, pindex+1, j);
                //canv_print(stdout, elements[lindex], (lindex/20)*40, (lindex%20)*40);
                if(!elements[lindex]){
                    for(int i=0;i<j;++i){
                        canv_free(elements[i]);
                    }
                    free(elements);
                    return NULL;
                }
                lindex++;

            }
            pindex=j;
        }
    }
    return elements;
}

/*-----------------------------------------------------------------*/
/**
 * @brief   Adjust a canvas to a given dimensions.
 * 
 * 
 * If the canvas is smaller, it will be centered and the rest
 * of the pixels will be transparent
 * If the canvas is bigger, it will be centered and cropped
 * 
 * @param src       Original canvas
 * @param nwidth    New width
 * @param nheight   New height
 * @return          New canvas with the given dimensions
 */
Canvas* canv_AdjustCrop(const Canvas* src, int nwidth,int nheight){
    Canvas* crop;
    int x1,y1;
    int x2,y2;
    if(src->wid>nwidth){
        if(src->hei>nheight){
            x1=(int)ceil((src->hei-nheight)/2);
            x2=x1+nheight;

            y1=(int)ceil((src->wid-nwidth)/2);
            y2=y1+nwidth;

        }
        else{
            x1=0;
            x2=src->hei;

            y1=(int)ceil((src->wid-nwidth)/2);
            y2=y1+nwidth;
        }
    }
    else{
        if(src->hei>nheight){
            x1=(int)ceil((src->hei-nheight)/2);
            x2=x1+nheight;

            y1=0;
            y2=src->wid;
        }
        else{
            x1=0;
            x2=src->hei;

            y1=0;
            y2=src->wid;
        }
    }
    crop=canv_subCopy(src,x1,x2,y1,y2);
    if(!crop){
        return NULL;
    }
    Canvas* work=canv_backGrnd(0, 0, 0, 0, nwidth, nheight);
    if(!work){
        canv_free(crop);
        return NULL;
    }
    int o_x,o_y;
    o_x=(nheight-crop->hei)/2;
    o_y=(nwidth-crop->wid)/2;
    canv_addOverlay(work, crop, o_x, o_y);
    canv_free(crop);
    return work;
}

/*-----------------------------------------------------------------*/
/**
 * @brief       Merges two canvases into one
 * 
 * The last two parameters indicate the coordinates of
 * the top-left corner of over when taking the top-left corner
 * of base as a reference frame.
 * 
 * @param base  The canvas that will act as background
 * @param over  The canvas that will act as foreground
 * @param o_i   The offset in the vertical axis
 * @param o_j   The offset in the horizontal axis
 * @return      New canvas with the result of overlaying one canvas
 *              on top of the other
 */
Canvas* canv_Overlay(const Canvas* base, const Canvas* over, int o_i, int o_j){
    if(!base||!over)return NULL;
    Canvas* res=canv_ini(base->wid, base->hei);
    if(!res)return NULL;
    int owid,ohei;
    owid=over->wid;
    ohei=over->hei;
    if(owid+o_j>base->wid)owid=base->wid-o_j;
    if(ohei+o_i>base->hei)ohei=base->hei-o_i;
    for(int i=0;i<base->hei;++i){
        for(int j=0;j<base->wid;++j){
            if(i>=o_i&&j>=o_j&&i<(o_i+ohei)&&j<(o_j+owid)){
                res->data[i][j]=pix_overlay(base->data[i][j], over->data[i-o_i][j-o_j]);
            }
            else{
                res->data[i][j]=pix_copy(base->data[i][j]);
            }
            if(!res->data[i][j]){
                canv_free(res);
                return NULL;

            }
        }
    }
    return res;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Merges a new canvas in the base one
 * 
 * @param base  The canvas that will act as background
 * @param over  The canvas that will act as foreground
 * @param o_i   The top limit of the foreground canvas
 * @param o_j   The left limit of the foreground canvas
 * @return      A NULL pointer if an error ocurred.
 *              base otherwise
 */
Canvas* canv_addOverlay(Canvas* base, const Canvas* over, int o_i, int o_j){
    if(!base||!over)return NULL;
    
    for(int i=o_i;i<o_i+over->hei;++i){
        if(i<0||i>=base->hei)continue;
        for(int j=o_j;j<o_j+over->wid;++j){
            if(j<0||j>=base->wid)continue;
            Pixel* t=pix_overlay(base->data[i][j], over->data[i-o_i][j-o_j]);
            pix_free(base->data[i][j]);
            base->data[i][j]=t;
            t=NULL;
            if(!base->data[i][j]){
                return NULL;
            }
        }
    }
    return base;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the canvas to the file
 * 
 * Note that this function will not check if the canvas fits on the screen
 * 
 * @param f File in which the canvas will be printed
 * @param c Canvas to be printed
 * @param i x-coordinate of of the canvas when it is displayed on the screen
 * @param j Left limit of the canvas when it is displayed on the screen
 */
void canv_print(FILE* f, Canvas* c,int i,int j){
    if(!f||!c)return;
    bool tofree=false;
    if(i<0&&j<0){
        if(-i>canv_getHeight(c)||-j>canv_getWidth(c))return;
        tofree=true;
        c=canv_subCopy(c, -i, canv_getHeight(c)+i, -j, canv_getWidth(c)+j);
    }
    else if(i<0){
        if(-i>canv_getHeight(c)||-j>canv_getWidth(c))return;
        tofree=true;
        c=canv_subCopy(c, -i, canv_getHeight(c)+i, 0, canv_getWidth(c));
    }
    else if(j<0){
        if(-i>canv_getHeight(c)||-j>canv_getWidth(c))return;
        tofree=true;
        c=canv_subCopy(c, 0, canv_getHeight(c), -j, canv_getWidth(c)+j);
    }
    char ** da=_canv_render(c,c->wid,c->hei);
    if(!da)return;
    for(int w=0;w<c->hei;++w){
        char * aux=movecur(i+w, j);
        fprintf(f,"%s",aux);
        //puts(aux);
        free(aux);
        fprintf(f, "%s",da[w]);
        //puts(da[w]);
        free(da[w]);
    }
    if(tofree) canv_free(c);
    fflush(f);
    free(da);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Returns the characters that have to be echoed to the terminal
 *        in order to print this canvas.
 * 
 * The characters include the movement of the cursor to the required
 * positions
 * 
 * 
 * @param c Canvas to be printed
 * @param i x-coordinate of of the canvas when it is displayed on the screen
 * @param j Left limit of the canvas when it is displayed on the screen
 * 
 * @returns An array of characters with the information required
 *          to print the Canvas
 */
char * canv_StorePrint(Canvas* c, int i, int j){
    if(!c)return NULL;
    bool tofree=false;
    
    if(i<0&&j<0){
        if(-i>canv_getHeight(c)||-j>canv_getWidth(c))return NULL;
        tofree=true;
        c=canv_subCopy(c, -i, canv_getHeight(c)+i, -j, canv_getWidth(c)+j);
    }
    else if(i<0){
        if(-i>canv_getHeight(c)||-j>canv_getWidth(c))return NULL;
        tofree=true;
        c=canv_subCopy(c, -i, canv_getHeight(c)+i, 0, canv_getWidth(c));
    }
    else if(j<0){
        if(-i>canv_getHeight(c)||-j>canv_getWidth(c))return NULL;
        tofree=true;
        c=canv_subCopy(c, 0, canv_getHeight(c), -j, canv_getWidth(c)+j);
    }
    char ** da=_canv_render(c,c->wid,c->hei);
    if(!da)return NULL;
    char * res=calloc(25*canv_getWidth(c)*canv_getHeight(c), sizeof(char));
    if(!res){
        free(da);
        return NULL;
    }
    int pos=0;
    for(int w=0;w<c->hei;++w){
        char * aux=movecur(i+w, j);
        appendf(res, &pos, aux);
        appendf(res,&pos,da[w]);
    }
    if(tofree)canv_free(c);
    return res;
}

/**
 * @brief Prints only the sections that are different between the new 
 *        and the old canvas
 * 
 * Note that for this function to work correctly, old should be the 
 * exact canvas that is now being displayed
 * 
 * @param f     File to which the canvas will be printed
 * @param new   New canvas
 * @param old   Previous canvas
 * @param oi    Vertical offset
 * @param oj    Horizontal offset
 * @return      A NULL pointer if there was an error. 
 *              canv otherwise
 */
Canvas* canv_printDiff(FILE* f,const Canvas* new,const Canvas* old,int oi, int oj){
    if(!f||!new||!old)return NULL;
    if(canv_getWidth(new)!=canv_getWidth(old))return NULL;
    if(canv_getHeight(new)!=canv_getHeight(old))return NULL;
    Canvas* canv=canv_backGrnd(0,0,0,0,canv_getWidth(new),canv_getHeight(new));
    for(int i=0;i<canv_getHeight(new);++i){
        for(int j=0;j<canv_getWidth(new);++j){
            if(!pix_equals(new->data[i][j],old->data[i][j])){
                pix_free(canv->data[i][j]);
                canv->data[i][j]=pix_copy(new->data[i][j]);
            }
        }
    }
    canv_printAllSolid(f,canv,old,oi,oj);
    canv_free(canv);
    return canv;
}


/*-----------------------------------------------------------------*/
/**
 * @brief Prints the solid section of the canvas
 * 
 * Note that all pixel that are completely transparent will not be printed
 * The pixels that are partilly transparent will be overlaied on top
 * of the background
 * 
 * @param c     Canvas to be printed
 * @param backg Background canvas, it will be used when pixels are not completely opaque.
 *              It must have the same width and height
 * @param oi    Top row
 * @param oj    Left column
 */

void canv_printAllSolid(FILE* f, const Canvas* c,const Canvas* backg,int oi,int oj){
    if(!c||!f||!backg)return;
    Pixel** buff=calloc(canv_getWidth(c),sizeof(Pixel*));
    char** str=calloc(canv_getWidth(c)*canv_getHeight(c),sizeof(char*));
    int strpos=0;
    if(!str||!buff){
        free(str);
        free(buff);
        return;
    }
    int l=0;
    int jj=0;
    for(int i=0;i<canv_getHeight(c);++i){
        int j=0;
        while(j<canv_getWidth(c)){
            jj=-1;
            while(j<canv_getWidth(c)&&pix_retA(c->data[i][j])){
                if(jj==-1)jj=j;
                if(pix_retA(c->data[i][j])==255){
                    buff[l]=pix_copy(c->data[i][j]);
                }
                else{
                    buff[l]=pix_overlay(backg->data[i][j],c->data[i][j]);
                }
                l++;
                j++;
            }
            if(!l){
                j++;
                continue;
            }
            char* c=pix_renderLine((const Pixel**)buff,l);
            l=0;
            char* c2=movecur(oi+i,oj+jj);
            char* segment=calloc(strlen(c)+strlen(c2)+1,sizeof(char));
            if(!c||!c2||!segment){
                free(c);
                free(c2);
                free(segment);
                free(buff);
                return;
            }
            int pos=0;
            append(segment,&pos,c2);
            append(segment,&pos,c);
            free(c);
            free(c2);
            str[strpos]=segment;
            strpos++;
            j++;
        }
    }
    int flen=0;
    for(int i=0;i<strpos;++i){
        flen+=strlen(str[i]);
    }
    char* rend=calloc(flen+1,sizeof(char*));
    int rpos=0;
    for(int i=0;i<strpos;++i)appendf(rend,&rpos,str[i]);
    free(str);
    fprintf(f,"%s",rend);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Prints the section that is not completely transparent
 *        of the canvas
 * 
 * Note that all pixel that not completely transparent will be printed
 * 
 * @param c     Canvas to be printed
 * @param oi    Top row
 * @param oj    Left column
 */
void canv_printAllNonTransparent(FILE* f, const Canvas* c,int oi,int oj){
    if(!c||!f)return;
    Pixel** buff=calloc(canv_getWidth(c),sizeof(Pixel*));
    char** str=calloc(canv_getWidth(c)*canv_getHeight(c),sizeof(char*));
    int strpos=0;
    if(!str||!buff){
        free(str);
        free(buff);
        return;
    }
    int l=0;
    int jj=0;
    for(int i=0;i<canv_getHeight(c);++i){
        int j=0;
        while(j<canv_getWidth(c)){
            jj=-1;
            while(j<canv_getWidth(c)&&pix_retA(c->data[i][j])){
                if(jj==-1)jj=j;
                buff[l]=pix_copy(c->data[i][j]);
                l++;
                j++;
            }
            if(!l){
                j++;
                continue;
            }
            char* c=pix_renderLine((const Pixel**)buff,l);
            l=0;
            char* c2=movecur(oi+i,oj+jj+1);
            char* segment=calloc(strlen(c)+strlen(c2)+1,sizeof(char));
            if(!c||!c2||!segment){
                free(c);
                free(c2);
                free(segment);
                free(buff);
                return;
            }
            int pos=0;
            append(segment,&pos,c2);
            append(segment,&pos,c);
            free(c);
            free(c2);
            str[strpos]=segment;
            strpos++;
            j++;
        }
    }
    int flen=0;
    for(int i=0;i<strpos;++i){
        flen+=strlen(str[i]);
    }
    char* rend=calloc(flen+1,sizeof(char*));
    int rpos=0;
    for(int i=0;i<strpos;++i)appendf(rend,&rpos,str[i]);
    free(str);
    fprintf(f,"%s",rend);
    fflush(stdout);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the value of a specific pixel in the 
 *        canvas
 * 
 * @param c Canvas that contains the pixel that has to be changed
 * @param p New pixel
 * @param i I coordinate of the pixel in the canvas
 * @param j J coordinate of the pixel in the canvas
 * @return Canvas* 
 */
Canvas* canv_setPixel(Canvas* c,const Pixel* p, int i,int j){
    if(!p||!c)return NULL;
    if(i>canv_getHeight(c)||j>canv_getWidth(c)){
        return NULL;
    }
    pix_copyVals(c->data[i][j],p);
    return c;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Returns a blurred version of the canvas
 * 
 * @param c     Canvas that will be blurred
 * @param rad   Radious of the blur effect
 * @return      New canvas with the blur applied
 */
Canvas* canv_blur(Canvas* c,int rad){
    if(!c)return NULL;
    Canvas* c2=canv_ini(canv_getWidth(c),canv_getHeight(c));
    for(int i=0;i<canv_getHeight(c);++i){
        for(int j=0;j<canv_getWidth(c);++j){
            int r,g,b,a;
            r=g=a=b=0;
            for(int k=-rad;k<=rad;++k){
                if(j+k>=0&&j+k<canv_getWidth(c)){
                    r+=pix_retR(c->data[i][j+k]);
                    g+=pix_retG(c->data[i][j+k]);
                    b+=pix_retB(c->data[i][j+k]);
                    a+=pix_retA(c->data[i][j+k]);
                }
            }
            c2->data[i][j]=pix_ini(r/(2*rad),g/(2*rad),b/(2*rad),a/(2*rad));
            if(!c2->data[i][j]){
                canv_free(c2);
                return NULL;
            }
        }
    }
    int rad2=rad/2;
    Canvas* c3=canv_copy(c2);
    for(int i=0;i<canv_getWidth(c);++i){
        for(int j=0;j<canv_getHeight(c);++j){
            int r,g,b,a;
            r=g=a=b=0;
            for(int k=-rad2;k<=rad2;++k){
                if(j+k>=0&&j+k<canv_getHeight(c)){
                    r+=pix_retR(c2->data[j+k][i]);
                    g+=pix_retG(c2->data[j+k][i]);
                    b+=pix_retB(c2->data[j+k][i]);
                    a+=pix_retA(c2->data[j+k][i]);
                }
            }
            c3->data[j][i]=pix_ini(r/(2*rad2),g/(2*rad2),b/(2*rad2),a/(2*rad2));
            if(!c3->data[j][i]){
                canv_free(c2);
                canv_free(c3);
                return NULL;
            }
        }
    }
    canv_free(c2);
    return c3;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Darkens the pixels in the canvas
 * It will not create a new canvas, the Canvas* return value
 * will only indicate wheter or not the process was successful
 * 
 * @param c     Canvas to be darken
 * @param light Percentage of light that the canvas will have 
 *              relative to its current brightness
 * @return  c if the process was successful
 *          NULL if there was any error
 */
Canvas* canv_darken(Canvas* c,double light){
    if(!c)return NULL;
    for(int i=0;i<c->hei;++i){
        for(int j=0;j<c->wid;++j){
            pix_darken(c->data[i][j],light);
        }
    }
    return c;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Applies a color filter to the Canvas
 * 
 * This filter works by reducing the values of some of the channels
 * of the pixels.
 * 
 * @param c     Canvas that will be filter
 * @param p     Color that will be applied in the filter
 * @return      A new canvas with the filter applied
 */
Canvas* canv_filter(Canvas* c,Pixel* p){
    if(!c||!p)return NULL;
    Canvas* cc=canv_copy(c);
    int r,g,b;
    r=pix_retR(p);
    g=pix_retG(p);
    b=pix_retB(p);
    int m=max(r,max(g,b));
    for(int i=0;i<cc->hei;++i){
        for(int j=0;j<cc->wid;++j){
            int rr,gg,bb,aa;
            rr=pix_retR(c->data[i][j]);
            gg=pix_retG(c->data[i][j]);
            bb=pix_retB(c->data[i][j]);
            aa=pix_retA(c->data[i][j]);
            rr*=((double)r)/m;
            gg*=((double)g)/m;
            bb*=((double)b)/m;
            pix_free(cc->data[i][j]);
            cc->data[i][j]=pix_ini(rr,gg,bb,aa);
        }
    }
    return cc;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Draws a circle of this color
 * 
 * @param p     The color that will fill the circle
 * @param rad   The radious of the requested circle
 * @return      New canvas containing the circle
 */
Canvas* canv_circle(Pixel* p,int rad){
    if(!p)return NULL;
    rad-=rad%2;
    Canvas* res=canv_ini(2*rad,2*rad);
    for(int i=0;i<2*rad;++i){
        for(int j=0;j<2*rad;++j){
            int cnt=0;
            for(int i2=i;i2<i+2;++i2)for(int j2=j;j2<j+2;++j2){
                cnt+=(i2-rad)*(i2-rad)+(j2-rad)*(j2-rad)<rad*rad;
            }
            res->data[i][j]=pix_copy(p);
            pix_setA(res->data[i][j],(int)((cnt/4.0)*255*pix_retA(p)));
        }
    }
    Canvas* r2=canv_ini(2*rad,rad);
    for(int i=0;i<rad;++i){
        for(int j=0;j<2*rad;++j){
            r2->data[i][j]=pix_average(res->data[2*i][j],res->data[2*i+1][j]);
        }
    }
    canv_free(res);
    return r2;
}

/*-----------------------------------------------------------------*/
/// Returns the width of the canvas
int canv_getWidth(const Canvas* c){
  if(!c)return -1;
  return c->wid;
}

/*-----------------------------------------------------------------*/
/// Returns the height of the canvas
int canv_getHeight(const Canvas* c){
  if(!c)return -1;
  return c->hei;
}

/*-----------------------------------------------------------------*/
/// Returns the pixel of the canvas in the given position
const Pixel* canv_getPixel(const Canvas* c,int i,int j){
    return c->data[i][j];
}


/* ##################################################################### */
/* ######################### PRIVATE FUNCTIONS ######################### */
/* ##################################################################### */


/*-----------------------------------------------------------------*/
/// Create the array of strings that contain the ansi scape sequences
/// required to display the colors on the screen.
///
/// @param c    Canvas to be processed
/// @param wid  width of the canvas that will be rendered
/// @param hei  height of the canvas that will be rendered
///
/// @remark     If wid or hei are less than the dimensions of the canvas,
///             only the area defined by wid and hei will be rendered
char** _canv_render(const Canvas* c,int wid, int hei){
    if(!c)return NULL;
    if(!c)return NULL;
    if(wid>c->wid)wid=c->wid;
    if(hei>c->hei)hei=c->hei;
    if(!c)return NULL;
    Canvas* cop=canv_backGrnd(0, 0, 0, 0, wid, hei);
    if(!cop)return NULL;
    if(!canv_addOverlay(cop, c, 0, 0)){
        canv_free(cop);
        return NULL;
    }
    char** ch=(char**) malloc(sizeof(char*)*c->hei);
    if(!ch){
        canv_free(cop);
        return NULL;
    }
    for(int i=0;i<hei;++i){
        ch[i]=pix_renderLine((const Pixel**)c->data[i], wid);
        if(!ch[i]){
            for(int j=0;j<i;++j){
                free(ch[j]);
                free(ch);
                canv_free(cop);
            }
            return NULL;
        }
    }
    canv_free(cop);
    return ch;
}


/*-----------------------------------------------------------------*/
/// Checks if the j-th column of this canvas is transparent
///
/// @param c Canvas to be check
/// @param j The index of the column (0-indexed)
bool _transparentColumn(const Canvas * c, int j){
    if(!c)return false;
    if(j>=c->wid) return false;
    bool b=true;
    for(int i=0;i<c->hei;++i)b*=pix_transparent(c->data[i][j]);
    return b;

}