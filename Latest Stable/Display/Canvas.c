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



/// Free the allocated memory
void canv_free(Canvas* c){
    if(!c)return;
    for(int i=0;i<c->hei;++i){
        if(!(c->data[i]))continue;
        for(int j=0;j<c->wid;++j){
            pix_free(c->data[i][j]);
        }
	free(c->data[i]);
    }
    free(c->data);
    free(c);
    
}


/// Create an empty canvas with the especified width and height
/// Note that the pixel pointers are still not allocated
/// @param wid Width of the canvas
/// @param hei Height of the canvas
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

/// Read a canvas from a file.
/// The format has to be the following:
/// <width> <height>
/// [r,g,b,a] [r,g,b,a]...
///
/// @param f File from which the data will be read
Canvas* canv_load(FILE* f){
    if(!f)return NULL;
    Canvas* c = calloc(1,sizeof(Canvas));
    if(!c)return NULL;
    fscanf(f,"%d %d",&c->wid,&c->hei);
    c->data=(Pixel***)calloc(c->hei,sizeof(Pixel**));
    if(!(c->data)){
        free(c);
        return NULL;
    }
    for(int i=0;i<c->hei;++i){
        c->data[i]=(Pixel**)calloc(c->wid,sizeof(Pixel**));
        if(!(c->data[i])){
            canv_free(c);
            return NULL;
        }
        for(int j=0;j<c->wid;++j){
            int r,g,b,a;
            char c1='.';
            while(c1!='['){
                fscanf(f,"%c",&c1);
            }
            fscanf(f,"%d, %d, %d, %d]",&a,&r,&g,&b);
            c->data[i][j]=pix_ini(r, g, b, a);
            if(!(c->data[i][j])){
               canv_free(c);
                return NULL;
            }
        }
    }
    
    return c;
}


/// Generate a canvas of a given color and dimensions
///
/// @param r        red component of this background
/// @param g        green component
/// @param b        blue component
/// @param a        alpha component. 255=opaque, 0=transparent
/// @param width    width of the generated canvas
/// @param height   height of the generated canvas
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
/// Get a new canvas that contains both canvases appended horizontally
/// @param west Canvas that will be in the western region of the result
/// @param east Canvas that will be in the eastern region of the result
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
        for(int j=0;j<west->wid;++j){
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

/// Get a new canvas that contains both canvases appended vertically
/// @param north Canvas that will be in the northern region of the result
/// @param south Canvas that will be in the southern region of the result
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

/// Returns a new Canvas that contains the original one and the margins defined in the input parameters
Canvas* canv_addMargin (const Canvas *src, int top, int right, int bottom, int left){
    if(!src)return NULL;
    Canvas* ret=canv_backGrnd(0, 0, 0, 0, src->wid+right+left, src->hei+top+bottom);
    canv_addOverlay(ret, src, top, left);
    return ret;
}


/// Return a copy of the given canvas
///
/// @param bas Canvas to be coppied
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


/// Split a canvas into smaller sub-canvases making vertical separations
/// @param src      Canvas to be splitted
/// @param nelem    Pointer to an integer where the number
///                 of returned elements will be stored
///
/// @return   An array of Canvas* that contains all non-transparent
///           sub-sections of the given canvas separated.
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


/// Returns a copy of a section of the given Canvas
///
/// @param bas Source canvas
/// @param x1 x starting index (included)
/// @param x2 x ending   index (excluded)
/// @param y1 y starting index (included)
/// @param y2 y ending   index (excluded)
Canvas* canv_subCopy (const Canvas* bas,int x1,int x2,int y1,int y2){
    if(!bas)return NULL;
    int hei,wid;
    hei=x2-x1;
    wid=y2-y1;
    Canvas* res=canv_ini(wid, hei);
    if(!res) return NULL;
    for(int i=0;i<hei;++i){
        for(int j=0;j<wid;++j){
            res->data[i][j]=pix_copy(bas->data[x1+i][y1+j]);
            if(!(res->data[i][j])){
                canv_free(res);
                return NULL;
            }
        }
    }
    return res;
}



/// Adjust a canvas to a given dimensions.
/// If the canvas is smaller, it will be centered and the rest
/// of the pixels will be transparent
/// If the canvas is bigger, it will be centered and cropped
///
/// @param src Original canvas
/// @param nwidth new width value
/// @param nheight new height value
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
    int o_x,o_y;
    o_x=(nheight-work->hei)/2;
    o_y=(nwidth-work->wid)/2;
    canv_addOverlay(work, crop, o_x, o_y);
    canv_free(crop);
    return work;
}


/// Merge two canvases into one
///
/// @param base The canvas that will act as background
/// @param over The canvas that will act as foreground
/// @param o_x  The top limit of the foreground canvas
/// @param o_y  The left limit of the foreground canvas
///
/// @remark     (o_x,o_y) are the coordinates of the top-left corner of the foreground canvas
///             with respecto to the background canvas
///
/// @return A new canvas containing the merge of the other two
Canvas* canv_Overlay(const Canvas* base, const Canvas* over, int o_x, int o_y){
    if(!base||!over)return NULL;
    Canvas* res=canv_ini(base->wid, base->hei);
    if(!res)return NULL;
    int owid,ohei;
    owid=over->wid;
    ohei=over->hei;
    if(owid+o_y>base->wid)owid=base->wid-o_y;
    if(ohei+o_x>base->hei)ohei=base->hei-o_x;
    for(int i=0;i<base->hei;++i){
        for(int j=0;j<base->wid;++j){
            if(i>=o_x&&j>=o_y&&i<(o_x+ohei)&&j<(o_y+owid)){
                res->data[i][j]=pix_overlay(base->data[i][j], over->data[i-o_x][j-o_y]);
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

/// Merge a new canvas in the base one
///
/// @param base The canvas that will act as background
/// @param over The canvas that will act as foreground
/// @param o_x  The top limit of the foreground canvas
/// @param o_y  The left limit of the foreground canvas
///
/// @remark     In this case, no new canvas is created,
///             the changes are applied to the background canvas.
Canvas* canv_addOverlay(Canvas* base, const Canvas* over, int o_x, int o_y){
    if(!base||!over)return NULL;
    Canvas* c=canv_Overlay(base, over, o_x, o_y);
    if(!c)return NULL;
    for(int i=0;i<base->hei;++i){
        for(int j=0;j<base->wid;++j){
            pix_copyVals(base->data[i][j], c->data[i][j]);
        }
    }
    canv_free(c);
    return base;
}



/// Print the canvas to the file
///
/// @param f    File in which the canvas will be printed
/// @param c    Canvas to be printed
/// @param x    Top limit of the canvas when it is displayed on the screen
/// @param y    Left limit of the canvas when it is displayed on the screen
///
/// @remark     The caller must know that the canvas will fit in the screen
void canv_print(FILE* f, const Canvas* c,int x,int y){
    if(!f||!c)return;
    char ** da=_canv_render(c,c->wid,c->hei);
    if(!da)return;
    for(int i=0;i<c->hei;++i){
        char * aux=movecur(x+i, y);
        fprintf(f,"%s",aux);
        free(aux);
        fprintf(f, "%s",da[i]);
        free(da[i]);
    }
    free(da);
}
/// Print the canvas to the file
///
/// @param f    File in which the canvas will be printed
/// @param c    Canvas to be printed
/// @param x    Top limit of the canvas when it is displayed on the screen
/// @param y    Left limit of the canvas when it is displayed on the screen
/// @param wid  Maximum width that will be displayed
/// @param hei  Maximum height that will be displayed

void canv_printR(FILE* f, const Canvas* c,int x,int y,int wid,int hei){
    if(!f||!c)return;
    if(wid>c->wid)wid=c->wid;
    if(hei>c->hei)hei=c->hei;
    char ** da=_canv_render(c,wid,hei);
    if(!da)return;
    for(int i=0;i<c->hei;++i){
        char * aux=movecur(x+i, y);
        fprintf(f,"%s",aux);
        free(aux);
        fprintf(f, "%s",da[i]);
        free(da[i]);
    }
    free(da);
}


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
    Canvas* cop=canv_backGrnd(0, 0, 0, 255, c->wid, c->hei);
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
        ch[i]=pix_renderLine(cop->data[i], wid);
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
