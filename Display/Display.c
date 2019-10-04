//  ppro
//	Display.c
//  Created by David del Val on 02/10/2019
//
//

#include "Display.h"
#define ARR_INC 3
struct _Display{
    int width, height;
    Room* room;
    int nLatWindow;
    int latWinalloc;
    Window** latWindow;
    Window* popup;
};

Display* disp_ini(int wid, int hei, Room* room){
    if(!room) return NULL;
    Display* dis=(Display*)calloc(1, sizeof(Display));
    if(!dis) return NULL;
    dis->width=wid;
    dis->height=hei;
    dis->room=room;
    dis->nLatWindow=0;
    dis->latWinalloc=3;
    dis->latWindow=(Window**)calloc(ARR_INC, sizeof(Window*));
    if(!dis->latWindow){
        disp_free(dis);
        return NULL;
    }
    return dis;
}
void disp_free(Display* dat){
    if(!dat)return;
    free(dat->latWindow);
    free(dat);
}
Display* disp_AddLWindow(Display*dis, Window* w){
    if(!dis||!w)return NULL;
    if(dis->nLatWindow+1>dis->latWinalloc){
        Window** aux=realloc(dis->latWindow,sizeof(Window*)*(dis->latWinalloc+ARR_INC));
        if(!aux)return NULL;
        else dis->latWindow=aux;
        dis->latWinalloc+=ARR_INC;
    }
    dis->latWindow[dis->nLatWindow]=win_copy(w);
    if(!dis->latWindow[dis->nLatWindow])return NULL;
    dis->nLatWindow++;
    return dis;
}
Display* disp_RemLwindow(Display* dis, int index){
    if(!dis) return NULL;
    if(index<0||index>=dis->nLatWindow)return NULL;
    win_free(dis->latWindow[index]);
    dis->latWindow[index]=NULL;
    for(int j=index+1;j<dis->nLatWindow;++j){
        dis->latWindow[j-1]=dis->latWindow[j];
    }
    if(index!=dis->nLatWindow-1){
        dis->latWindow[dis->nLatWindow-1]=NULL;
    }
    dis->nLatWindow--;
    return dis;
}
Display* disp_SetPopup(Display* dis, Window* p){
    if(!dis||!p) {
        return NULL;
    }
    dis->popup=win_copy(p);
    if(!dis->popup) return NULL;

    return dis;
}
void disp_RemPopup(Display* dis){
    if(!dis->popup) return;
    win_free(dis->popup);
}
