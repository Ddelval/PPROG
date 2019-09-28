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
    char** ch=(char**) malloc(sizeof(char*)*c->hei);
    if(!ch)return NULL;
    for(int i=0;i<hei;++i){
        ch[i]=pix_renderLine(c->data[i], wid);
        if(!ch[i]){
            for(int j=0;j<i;++j){
                free(ch[j]);
                free(ch);
            }
            return NULL;
        }
    }
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
Canvas* _canv_setOverlay(Canvas* res,Canvas* base,Canvas* over,int o_x, int o_y){
    int owid,ohei;
    owid=over->wid;
    ohei=over->hei;
    if(owid+o_x>base->wid)owid=base->wid-o_x;
    if(ohei+o_y>base->hei)ohei=base->hei-o_y;
    for(int i=0;i<base->hei;++i){
        for(int j=0;j<base->wid;++j){
            if(i>=o_x&&j>=o_y&&i<(o_x+ohei)&&j<(o_y+owid)){
                res->data[i][j]=pix_overlay(base->data[i][j], over->data[i][j]);
            }
            else{
                res->data[i][j]=pix_copy(base->data[i][j]);
            }
            if(!res->data[i][j]){
                for(int w=0;w<j;++w){
                    pix_free(res->data[i][w]);
                }
                for(int z=0;z<i;++z){
                    for(int w=0;w<base->wid;++w){
                        pix_free(res->data[z][w]);
                    }
                    free(res->data[z]);
                }
                free(res->data);
                free(res);
                
            }
        }
    }
    return res;
}

Canvas* canv_Overlay(Canvas* base, Canvas* over, int o_x, int o_y){
    if(!base||!over)return NULL;
    Canvas* res=canv_ini(base->wid, base->hei);
    if(!res)return NULL;
    if(!_canv_setOverlay(res, base, over, o_x, o_y)){
        canv_free(res);
        return NULL;
    }
    return res;
}
Canvas* canv_AddOverlay(Canvas* base, Canvas* over, int o_x, int o_y){
    return _canv_setOverlay(base, base, over, o_x, o_y);
}

Canvas* canv_copy (Canvas* bas){
    if(!bas)return NULL;
    Canvas* res=canv_ini(bas->wid, bas->hei);
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
}/*
Canvas ** canv_split(Canvas* src){
    
}
*/
