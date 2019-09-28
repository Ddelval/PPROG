//  Test
//	Canvas.c
//  Created by David del Val on 27/09/2019
//
//

#include "Canvas.h"

struct _Canvas {
    Pixel*** data;
    int wid;
    int hei;
};


/// Create an empty canvas with the especified width and height
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

/// Free the allocated memory
void canv_free(Canvas* c){
    if(!c)return;
    for(int i=0;i<c->hei;++i){
        if(!(c->data[i]))return;
        for(int j=0;j<c->wid;++j){
            pix_free(c->data[i][j]);
        }
    }
    
}

/// Create the array of strings that contain the ansi scape sequences
/// required to display the colors on the screen.
/// @param c Canvas to be processed
/// @param wid width of the canvas that will be rendered
/// @param hei height of the canvas that will be rendered
/// @remark If wid or hei are less than the dimensions of the canvas,
/// only the area defined by wid and hei will be rendered
char** canv_render(Canvas* c,int wid, int hei){
    if(!c)return NULL;
    if(wid>c->wid)wid=c->wid;
    if(hei>c->hei)hei=c->hei;
    if(!c)return NULL;
    Canvas* cop=canv_backGrnd(0, 0, 0, 255, c->wid, c->hei);
    canv_addOverlay(cop, c, 0, 0);
    char** ch=(char**) malloc(sizeof(char*)*c->hei);
    if(!ch)return NULL;
    for(int i=0;i<hei;++i){
        ch[i]=pix_renderLine(cop->data[i], wid);
        if(!ch[i]){
            for(int j=0;j<i;++j){
                free(ch[j]);
                free(ch);
            }
            return NULL;
        }
    }
    canv_free(cop);
    return ch;
}

void canv_print(FILE* f,Canvas* c,int x,int y){
    char ** da=canv_render(c,c->wid,c->hei);
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
void canv_printR(FILE* f,Canvas* c,int x,int y,int wid,int hei){
    if(wid>c->wid)wid=c->wid;
    if(hei>c->hei)hei=c->hei;
    char ** da=canv_render(c,wid,hei);
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

Canvas* canv_Overlay(Canvas* base, Canvas* over, int o_x, int o_y){
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
Canvas* canv_addOverlay(Canvas* base, Canvas* over, int o_x, int o_y){
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

Canvas* canv_copy (Canvas* bas){
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

/// Returns a copy of a section of the given Canvas
/// @param bas Source canvas
/// @param x1 x starting index (included)
/// @param x2 x ending   index (excluded)
/// @param y1 y starting index (included)
/// @param y2 y ending   index (excluded)
Canvas* canv_subCopy (Canvas* bas,int x1,int x2,int y1,int y2){
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
bool _transparentColumn(Canvas * c, int j){
    if(j>=c->wid) return false;
    bool b=true;
    for(int i=0;i<c->hei;++i)b*=pix_transparent(c->data[i][j]);
    return b;
    
}
Canvas ** canv_VSplit(Canvas* src, int* nelem){
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
                //fprintf(stdout,"%s Abcdeeefesasd",movecur(0, 40*lindex));
                //canv_print(stdout, elements[lindex], 40*(lindex/10),40*(lindex%10));
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
Canvas* canv_AdjustCrop(Canvas* src, int nwidth,int nheight){
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
    return work;
}

