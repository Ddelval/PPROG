//  ppro
//	Display.c
//  Created by David del Val on 02/10/2019
//
//

#include "Display.h"
#define ARR_INC 3
#define TITLE_MULTILINE 10
struct _Display{
    int width, height;
    int vdiv;
    bool rendered;
    int tithei;
    int topm;
    char* title;
    const Font* titf;
    Room* room;
    int nLatWindow;
    int latWinalloc;
    Window** latWindow;
    Window* popup;
    bool dialog;
};

Display* disp_ini(int wid, int hei, Room* room, int vdiv,char* tit, const Font* titf){
    if(!room) return NULL;
    Display* dis=(Display*)calloc(1, sizeof(Display));
    if(!dis) return NULL;
    dis->width=wid;
    dis->height=hei;
    dis->titf=titf;
    dis->vdiv=vdiv;
    dis->title=calloc(strlen(tit)+1, sizeof(char));
    if(!dis->title){
        disp_free(dis);
        return NULL;
    }
    if(!strcpy(dis->title, tit)){
        disp_free(dis);
        return NULL;
    }
    dis->room=room;
    dis->nLatWindow=0;
    dis->latWinalloc=3;
    dis->rendered=false;
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
    free(dat->title);
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
Display* disp_incSelIndex(Display* dis, int winIndex, int increment){
  if(!dis)return NULL;
  if(winIndex>=dis->nLatWindow) return NULL;
  if(win_incrementSelected(dis->latWindow[winIndex],increment)==NULL){
    
    return NULL;
  }
  return print_Window(dis,winIndex);
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
Canvas* disp_Render(Display* dis){
    Canvas* left=NULL;
    Canvas* res=NULL;
    Wlabel* l = wl_ini(dis->title, dis->titf, TITLE_MULTILINE);
    Canvas* right=wl_render(l, dis->width-dis->vdiv);
    dis->rendered=true;
    dis->tithei=canv_getHeight(right);
    wl_free(l);
    for(int i=0;i<dis->nLatWindow;++i){
        Canvas* c=win_render(dis->latWindow[i]);
        if(!c||!canv_appendVI(right, c)){
            canv_free(c);
            goto CLEAN;
        }
    }
    room_setBounds(dis->room,0,0,dis->height,dis->vdiv);
    left=room_getRender(dis->room);
    //left =room_getSubRender(dis->room, 0, 0, dis->vdiv, dis->height);
    if(!left)goto CLEAN;
    dis->topm=(canv_getHeight(left)-canv_getHeight(right))/2;
    res =canv_appendHNL(left, right);

CLEAN:
    canv_free(left);
    canv_free(right);

    return res;
}
int disp_scroll(Display* dis,double i,double j){
    if(!dis)return -1;
    return room_scroll(dis->room,i,j);
    
}
Display* print_Window(Display*dis, int index){
    if(!dis) return NULL;
    if(index<0 || index>dis->nLatWindow) return NULL;
    Canvas* c=win_render(dis->latWindow[index]);
    //int ipos=dis->tithei+dis->topm;
    int ipos=dis->tithei;
    for(int i=0;i<index;++i){
      ipos+=win_getHeight(dis->latWindow[i]);
    }
    canv_print(stdout,c,ipos,dis->vdiv+1);
    return dis;
}

Display* disp_DiaglogWindow(Display* dis, char * txt,const Font* f){
    if(!dis||!txt)return NULL;
    Canvas* c=disp_Render(dis);
    int h=150;
    Canvas* bottom=canv_subCopy(c,canv_getHeight(c)-h,canv_getHeight(c),0,canv_getWidth(c));
    Canvas* bckg=canv_blur(bottom,10);
    canv_darken(bckg,0.40);
    Wlabel* wl= wl_ini(txt,f,10);
    Canvas* ren=wl_render(wl, dis->width-100);
    canv_addOverlay(bckg,ren,10,10);
    canv_print(stdout,bckg,canv_getHeight(c)-h,0);
    dis->dialog=true;
    return dis;
}
Display* disp_remDialog(Display* dis){
    if(!dis)return NULL;
    Canvas* c=disp_Render(dis);
    canv_print(stdout,c,0,0);
    canv_free(c);
    dis->dialog=false;
    return dis;
}

Room* disp_getrefRoom(Display* dis){
    return dis? dis->room:NULL;
}
