//  ppro
//	Wlabic.c
//  Created by David del Val on 20/10/2019
//
//

#include "Wlabic.h"
struct _Wlabic{
    Wlabel* wl;
    Canvas* pic;
    wi_align l;
    int br, bg, bb, ba;
    int gap;
};

Wlabic* wi_ini(char *t, const Font* f,int lgap, int igap, wi_align l){
    if(!t||!f)return NULL;
    Wlabic* v = calloc(1, sizeof(Wlabic));
    if(!v)return NULL;
    v->wl=wl_ini(t, f, lgap);
    v->l=l;
    if(!v->wl){
        wi_free(v);
        return NULL;
    }
    v->gap=igap;
    return v;
}
void wi_free(Wlabic* w){
    if(!w)return;
    canv_free(w->pic);
    wl_free(w->wl);
    free(w);
}
Wlabic* wi_setCanvas(Wlabic* sr, Canvas* can){
    if(!sr||!can)return NULL;
    sr->pic=canv_copy(can);
    if(!sr->pic)return NULL;
    return sr;
}
Wlabic* wi_copy(const Wlabic* src){
    if(!src)return NULL;
    Wlabic* v=calloc(1, sizeof(Wlabic));
    if(!v)return NULL;
    v->l=src->l;
    v->wl=wl_copy(src->wl);
    v->pic=canv_copy(src->pic);
    if((!v->wl&&src->wl)||(!v->pic&&src->pic)){
        wi_free(v);
        return NULL;
    }
    return v;
}
Canvas* wi_render (Wlabic* wi, int width){
    if(!wi)return NULL;
    if(!wi->wl||!wi->pic)return NULL;
    Canvas *c=NULL;
    if(wi->l==TEXT_EAST||wi->l==TEXT_WEST)c=wl_renderSmall(wi->wl, width-canv_getWidth(wi->pic)-wi->gap);
    else if(wi->l==TEXT_NORTH){
        c=wl_renderSmall(wi->wl, width-canv_getWidth(wi->pic));
        canv_addMargin(c,0,0,0,wi->gap);
    }
    else{
        c=wl_renderSmall(wi->wl, width-canv_getWidth(wi->pic));
        canv_addMargin(c,wi->gap,0,0,0);
    }
    int nhei=max(canv_getHeight(c),canv_getHeight(wi->pic));
    Canvas* cc=canv_AdjustCrop(wi->pic, canv_getWidth(wi->pic), nhei);
    if(!cc){
        canv_free(c);
        return NULL;
    }
    Canvas *c1=canv_AdjustCrop(c, canv_getWidth(c), nhei);
    if(!c1){
        canv_free(cc);
        canv_free(c);
        return NULL;
    }
    canv_free(c);
    if(wi->l==TEXT_EAST){
        if(canv_appendHI(cc, c1)==NULL){
            canv_free(cc);
            canv_free(c1);
        }
    }
    else if(wi->l==TEXT_WEST){
        if(canv_appendHI(c1, cc)==NULL){
            canv_free(cc);
            canv_free(c1);
        }
        Canvas * c=c1;
        c1=cc;
        cc=c;
    }
    else if(wi->l==TEXT_NORTH){
        if(canv_appendVI(cc, c1)==NULL){
            canv_free(cc);
            canv_free(c1);
        }
        
    }
    else{
        if(canv_appendVI(c1, cc)==NULL){
            canv_free(cc);
            canv_free(c1);
        }
        Canvas * c=c1;
        c1=cc;
        cc=c;
    }

    canv_free(c1);
    Canvas* r=canv_AdjustCrop(cc, width, canv_getHeight(cc));
    canv_free(cc);

    Canvas* cb=canv_backGrnd(wi->br,wi->bg, wi->bb, wi->ba, canv_getWidth(r), canv_getHeight(r));
  	if(!canv_addOverlay(cb, r, 0, 0)) {
  		canv_free(cb);
  		canv_free(r);
  		return NULL;
  	}
  	canv_free(r);
  	return cb;
}

/*-----------------------------------------------------------------*/
/// Change the background color for the Wlabic element
/// @param w    Element to be selected
/// @param r		Red channel of the background
/// @param g		Green channel of the background
/// @param b		Blue channel of the background
/// @param a		Alpha channel of the background
Wlabic* wi_setBackColor(Wlabic* w, int r, int g,int b,int a) {
  if(!w) return NULL;
  w->br=r;
  w->bg=g;
  w->bb=b;
  w->ba=a;
  return w;
}
