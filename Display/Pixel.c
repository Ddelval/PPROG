//  Test
//	Pixel.c
//  Created by David del Val on 27/09/2019
//
//

#include "Pixel.h"
#define  MAX_VAL 255
struct _Pixel{
    int r,g,b,a;
};
int _filter(int a){
    if(a>MAX_VAL)a=MAX_VAL;
    if(a<0)a=0;
    return a;
}
Pixel* pix_ini(int r,int g, int b, int a){
    Pixel* p= (Pixel*) calloc(1,sizeof(Pixel));
    if(!p)return NULL;
    p->r=_filter(r);
    p->g=_filter(g);
    p->b=_filter(b);
    p->a=_filter(a);
    return p;
}
void pix_free(Pixel* p){
    free(p);
}

/// Returns the pixel resulting of overlaying the top pixel over the back one
/// @param back     Underlaying pixel. It cannot have an alpha value different from 255
///                 That is to say, hasAlpha=false.
/// @param top      Overlaying pixel. It's alpha value will define the blend.
Pixel* pix_overlay(Pixel* back, Pixel* top){
    if(!back||!top)return NULL;
    if(back->a!=255)return NULL;
    int nr,ng,nb;
    double ra;
    ra=top->a/255.0;
    nr=top->r*ra+(1-ra)*back->r;
    ng=top->g*ra+(1-ra)*back->g;
    nb=top->b*ra+(1-ra)*back->b;
    return pix_ini(nr, ng, nb, 255);
}
Pixel* pix_setR(Pixel* p, int r){
    if(!p)return NULL;
    p->r=r;
    return p;
}
Pixel* pix_setG(Pixel* p, int g){
    if(!p)return NULL;
    p->g=g;
    return p;
}
Pixel* pix_setB(Pixel* p, int b){
    if(!p)return NULL;
    p->b=b;
    return p;
}
Pixel* pix_setA(Pixel* p, int a){
    if(!p)return NULL;
    p->a=a;
    return p;
}
int pix_retR(Pixel* p){
    if(!p)return -1;
    return p->r;
}
int pix_retG(Pixel* p){
    if(!p)return -1;
    return p->g;
}
int pix_retB(Pixel* p){
    if(!p)return -1;
    return p->b;
}
int pix_retA(Pixel* p){
    if(!p)return -1;
    return p->a;
}
bool pix_equals(Pixel* a,Pixel*b){
    return a->r==b->r&&a->g==b->g&&a->b==b->b&&a->a==b->a;
}
char* pix_scapeSeq(Pixel* a){
    char* c=(char*)malloc(sizeof(char)*25);
    sprintf(c,"%c[%d;%d;%d;%d;%dm ",27,48,2,a->r,a->g,a->b);
    return c;
}
char * pix_renderLine(Pixel**a,int len){
    int diff=0;
    for(int i=1;i<len;++i) diff+=!pix_equals(a[i], a[i-1]); //Count differences
    int size=(diff+1)*25+len+2;
    char * res= (char*) malloc(sizeof(char)*size);
    int ipos=0;
    appendf(res, &ipos, pix_scapeSeq(a[0]));
    for(int i=1;i<len;++i){
        if(!pix_equals(a[i], a[i-1])){
            appendf(res, &ipos, pix_scapeSeq(a[i]));
        }
        else{
            append(res, &ipos, " ");
        }
    }
    return res;
}
Pixel * pix_copy(Pixel* src){
    if(!src)return NULL;
    return pix_ini(src->r, src->g, src->b, src->a);
}
