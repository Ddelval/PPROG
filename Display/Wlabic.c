//  ppro
//	Wlabic.c
//  Created by David del Val on 20/10/2019
//
//

#include "Wlabic.h"
struct _Wlabic{
    Wlabel* wl;
    Canvas* pic;
    Alignment l;
    int hgap;
};

Wlabic* wi_ini(char *t, const Font* f,int vgap, int hgap, Alignment l){
    if(!t||!f)return NULL;
    Wlabic* v = calloc(1, sizeof(Wlabic));
    if(!v)return NULL;
    v->wl=wl_ini(t, f, vgap);
    v->l=l;
    if(!v->wl){
        wi_free(v);
        return NULL;
    }
    v->hgap=hgap;
    return v;
}
void wi_free(Wlabic* w){
    if(!w)return;
    canv_free(w->pic);
    wl_free(w->wl);
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
    Canvas *c=wl_renderSmall(wi->wl, width-canv_getWidth(wi->pic)-wi->hgap);
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
    if(wi->l==TEXT_RIGHT){
        if(canv_appendHI(cc, c1)==NULL){
            canv_free(cc);
            canv_free(c1);
        }
    }
    else{
        if(canv_appendHI(c1, cc)==NULL){
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
    return r;
    
}

