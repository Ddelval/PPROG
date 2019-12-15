//  ppro
//	Display.c
//  Created by David del Val on 02/10/2019
//
//

#include "Display.h"
#define ARR_INC 3
#define TITLE_MULTILINE 10
#define DARKEN 0.40
#define BLUR_RAD 10
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
    bool pop_dial;
    bool pop_inv;
    bool pop_craf;
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
    room_setHW(dis->room,dis->height,dis->vdiv);
    //room_setBounds(dis->room,0,0,dis->height,dis->vdiv);
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
    canv_darken(bckg,DARKEN);
    Wlabel* wl= wl_ini(txt,f,10);
    Canvas* ren=wl_render(wl, dis->width-100);
    canv_addOverlay(bckg,ren,10,10);
    canv_print(stdout,bckg,canv_getHeight(c)-h,0);
    dis->pop_dial=true;
    return dis;
}
Display* disp_remDialog(Display* dis){
    if(!dis)return NULL;
    if(!dis->pop_dial)return dis;
    Canvas* c=disp_Render(dis);
    canv_print(stdout,c,0,0);
    canv_free(c);
    dis->pop_dial=false;
    return dis;
}

Display* disp_InventoryWindow(Display* dis, Inventory* inv, Font* ftitle, Font* fsubtitle, Font* ftext, Font* fnumbers){
    if(!dis||!inv)return NULL;
    int dim;
    int *dimens;
    char ** text;
    Canvas *** dat=inv_render(inv,&dim,&dimens,&text,ftext,fnumbers);
    Wlabel* tit=wl_ini("Inventory",ftitle,10);
    Canvas* c=wl_render(tit,dis->width);
    for(int i=0;i<dim;++i){
        //Iterate through each type of element
        if(dimens[i]>0){
            Wlabel* w=wl_ini(text[i],fsubtitle,10);
            Canvas * c2=wl_render(w,dis->width);
            canv_appendVI(c,c2);
        }
        else continue;
        Canvas * b=dat[i][0];
        for(int j=1;j<dimens[i];++j){
            Canvas*bc=canv_backGrnd(0,0,0,0,10,10);
            canv_appendHI(b,bc);
            canv_appendHI(b,dat[i][j]);
        }
        canv_appendVI(c,b);
    }
    Canvas* back=disp_Render(dis);
    Canvas* back2=canv_blur(back,BLUR_RAD);
    canv_free(back);
    canv_darken(back2,DARKEN);
    canv_addOverlay(back2,c,10,0);
    canv_print(stdout,back2,0,0);
    dis->pop_inv=true;
    return dis;
}
Display* disp_CraftingWindow(Display* dis,Inventory* inv){
    if(!dis||!inv)return NULL;

    int gap_w=1;
    int circ_rad=10;
    Pixel* nsel=pix_ini(255,255,255,255);
    Canvas* dot=canv_circle(nsel,circ_rad);
    Pixel* sel=pix_ini(150,150,255,255);
    Canvas* dotsel=canv_circle(sel,circ_rad);

    pix_free(nsel); pix_free(sel);
    
    int size=0;
    Recipe** rec=rdic_getAllDoable(inv,&size);
    if(!size)return canv_backGrnd(0,0,0,0,0,0);
    if(!rec)return NULL;

    int h,w,o_w;
    int hei,wid,ob_wid;
    hei=wid=ob_wid=0;
    for(int i=0;i<size;++i){
        rec_getObjDimensions(rec[i],&o_w,&h);
        hei=max(hei,h);
        ob_wid=max(ob_wid,o_w);
    }
    for(int i=0;i<size;++i){
        w=rec_getMinWidth(rec[i],ob_wid,hei);
        wid=max(wid,w);
    }

    Canvas* fdot =canv_AdjustCrop(dot,canv_getWidth(dot),hei);
    Canvas* fsdot=canv_AdjustCrop(dotsel,canv_getWidth(dotsel),hei);


    Canvas* gap=canv_backGrnd(0,0,0,0,wid,gap_w);
    int margin=10;
    int box_w=20;
    Canvas* cc=rec_render(rec[0],ob_wid,wid,hei,dis->width-2*margin-box_w);
    Canvas* c=canv_appendH(fdot,cc);
    for(int i=1;i<size;++i){
        canv_appendVI(c,gap);
        Canvas* c2=rec_render(rec[i],ob_wid,wid,hei,dis->width-2*margin-box_w);
        Canvas* c3=canv_appendH(fdot,c2);
        canv_appendVI(c,c3);
        canv_free(c2);
        canv_free(c3);
    }
    canv_free(gap);

    Wlabel* wl=wl_ini("Recipies",fcat_lookup(M8),0);
    Canvas* wl_r=wl_render(wl,dis->width);
    int marg=15;
    Canvas* wl_rr=canv_AdjustCrop(wl_r,dis->width,canv_getHeight(wl_r)+marg);
    Canvas* c_r=canv_AdjustCrop(c,dis->width,canv_getHeight(c));
    canv_appendVI(wl_rr,c_r);
    Canvas* back=disp_Render(dis);
    Canvas* back2=canv_blur(back,BLUR_RAD);
    canv_free(back);
    canv_darken(back2,DARKEN);
    canv_addOverlay(back2,wl_rr,0,0);
    canv_print(stdout,back2,0,0);


    dis->pop_craf=true;
}
Display* disp_remInventory(Display* d){
    if(!d)return NULL;
    if(!d->pop_inv)return d;
    d->pop_inv=false;
    Canvas* c=disp_Render(d);
    if(!c)return NULL;
    canv_print(stdout,c,0,0);
    return d;
}


Room* disp_getrefRoom(Display* dis){
    return dis? dis->room:NULL;
}

int disp_incPos(Display* d,int index, int i, int j, int* f_i, int *f_j){

    if(d->pop_dial||d->pop_inv||d->pop_inv)return 0;


    int a=room_incPos(d->room, index, i, j);
    int b=-1;
    int c=-1;
    if(i>0)b=3;
    else if(i<0)b=1;
    if(j>0)c=2;
    else if(j<0)c=4;

    if(a==-1)return -1;
    if(a==b||a==c){
        if(disp_scroll(d,0.5*(i>0)-0.5*(i<0),0.5*(j>0)-0.5*(j<0))==1){
            canv_print(stdout,room_getRender(d->room),0,0);
        }
    }
    else{
        room_printMod(d->room, 0, 0);
    }
    room_getBSpritePos(d->room,index,f_i,f_j);
    return 0;
}
Display* disp_execute(Display* dis, int index, int room_index, void* en){
    if(!dis)return NULL;
    func_trig f =win_getSelectedAction(dis->latWindow[index]);
    trig_type t=win_getSelectedTrigType(dis->latWindow[index]);
    if(tr_needsTrigger(t)){
        int a;
        Trigger** dat =room_getTriggers(dis->room,t,room_index,&a);
        
        if(a)f(dat[0],en,dis);
    }
    else{
        f(NULL,en,dis);
    }
    
    return dis;
}
