//  ppro
//	Display.c
//  Created by David del Val on 02/10/2019
//
//

#include "Display.h"
#define ARR_INC 3
#define LINE_WIDTH 2
#define TITLE_MULTILINE 10
#define DARKEN 0.40
#define BLUR_RAD 10
#define NSEL_VALS 255,255,255,255
#define  SEL_VALS 150,150,255,255
#define CIRC_RAD 10
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

    free(dat->title);
    room_free(dat->room);
    for(int i=0;i<dat->nLatWindow;++i){
        win_free(dat->latWindow[i]);
    }
    win_free(dat->popup);
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

Display* disp_incSelIndex(Display* dis, int winIndex, int increment){
  if(!dis)return NULL;
  if(winIndex>=dis->nLatWindow) return NULL;
  if(win_incrementSelected(dis->latWindow[winIndex],increment)==NULL){

    return NULL;
  }
  return print_Window(dis,winIndex);
}

Display* disp_setSelIndex(Display* dis, int winIndex, int selIndex){
  if(!dis)return NULL;
  if(winIndex>=dis->nLatWindow) return NULL;
  if(win_setSelected(dis->latWindow[winIndex],selIndex)==NULL){
    return NULL;
  }
  return print_Window(dis,winIndex);
}

int disp_getSelIndex(Display* dis, int winIndex){
  if(!dis) return -1;
  if(winIndex>=dis->nLatWindow) return -1;

  return win_getSelectedIndex(dis->latWindow[winIndex]);
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
    Canvas* t=NULL,*t2=NULL;
    Wlabel* l = wl_ini(dis->title, dis->titf, TITLE_MULTILINE);
    Canvas* right=wl_render(l, dis->width-dis->vdiv);
    dis->rendered=true;
    dis->tithei=canv_getHeight(right);
    wl_free(l);
    Canvas* bar=canv_backGrnd(255,255,255,255,dis->width-dis->vdiv,LINE_WIDTH);
    for(int i=0;i<dis->nLatWindow;++i){
        canv_appendVI(right,bar);
        Canvas* c=win_render(dis->latWindow[i]);
        if(!c||!canv_appendVI(right, c)){
            canv_free(c);
            goto CLEAN;
        }
        canv_free(c);
    }
    canv_appendVI(right,bar);
    room_setHW(dis->room,dis->height,dis->vdiv);
    //room_setBounds(dis->room,0,0,dis->height,dis->vdiv);
    left=room_getRender(dis->room);
    //left =room_getSubRender(dis->room, 0, 0, dis->vdiv, dis->height);
    if(!left)goto CLEAN;
    dis->topm=(canv_getHeight(left)-canv_getHeight(right))/2;
    Canvas* vbar=canv_backGrnd(255,255,255,255,LINE_WIDTH*2,canv_getHeight(right));
    t=canv_appendHNL(left,vbar);
    t2=canv_appendHNL(right,vbar);
    res =canv_appendHNL(t, t2);

CLEAN:
    canv_free(left);
    canv_free(right);
    canv_free(bar);
    canv_free(t);
    canv_free(t2);
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
    int ipos=dis->tithei+LINE_WIDTH;
    for(int i=0;i<index;++i){
        ipos+=LINE_WIDTH;
        ipos+=win_getHeight(dis->latWindow[i]);
    }
    canv_print(stdout,c,ipos,dis->vdiv+1+2*LINE_WIDTH);
    return dis;
}
Display* disp_DialogWindow(const Display* dis, const DialogMan* dman, char * ename){
    if(!dis||!dman)return NULL;

    char* txt=NULL;
    char* en =NULL;
    bool err=false;
    Canvas *bottom=NULL,*back=NULL,*rend=NULL;
    Canvas *wl_rend=NULL,*result=NULL;
    Canvas *nam_rend=NULL;
    Wlabel* wl=NULL;

    en=calloc(strlen(ename)+2,sizeof(char));
    if(!en) goto ERR_END;
    strcpy(en,ename);
    int l=strlen(en);
    en[l]=':';
    en[l+1]=0;

    wl=wl_ini(en,fcat_lookup(M6),0);
    wl_rend=wl_renderSmall(wl,dis->width);
    nam_rend=canv_addMargin(wl_rend,10,0,0,20);
    if(!nam_rend)goto ERR_END;
    canv_free(wl_rend);
    wl_rend=canv_addMargin(nam_rend,0,dis->width-canv_getWidth(nam_rend),0,0);
    canv_free(nam_rend);
    nam_rend=wl_rend;
    wl_rend=NULL;
    wl_free(wl);

    int h=150;
    int ipos=dis->height-h;

    rend=disp_Render(dis);
    bottom=canv_subCopy(rend,ipos ,canv_getHeight(rend),0,canv_getWidth(rend));
    back=canv_blur(bottom,10);
    canv_darken(back,DARKEN);
    canv_addOverlay(back,nam_rend,0,0);
    if(!back)goto ERR_END;

    txt=dman_getLine(dman);
    if(!txt) txt=strdup("... ... ...");


    wl=wl_ini(txt,fcat_lookup(M4),0);
    wl_rend=wl_render(wl, dis->width-100);
    if(!wl_rend)goto ERR_END;

    result=canv_Overlay(back,wl_rend,10+canv_getHeight(nam_rend),50);
    canv_print(stdout,result,ipos,0);
    char in;
    while(1){
        in=getch1();
        wl_free(wl); wl=NULL;
        canv_free(wl_rend); wl_rend=NULL;

        txt=dman_getLine(dman);
        if(!txt)goto END;
        wl=wl_ini(txt,fcat_lookup(M4),0);
        wl_rend=wl_render(wl, dis->width-100);
        if(!wl_rend)goto ERR_END;
        result=canv_Overlay(back,wl_rend,10+canv_getHeight(nam_rend),10);
        if(!result)goto ERR_END;
        canv_print(stdout,result,ipos,0);

    }
    goto END;

SUCC_END:


ERR_END:
    err=true;
END:
    canv_print(stdout,bottom,ipos,0);
    canv_free(bottom);
    canv_free(back);
    canv_free(rend);
    free(txt);
    return err? NULL:dis;
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
    pairii* sizes;
    Canvas *** dat=inv_render(inv,&dim,&dimens,&text,&sizes,ftext,fnumbers);
    Wlabel* tit=wl_ini("Inventory",ftitle,10);
    Canvas* c=wl_render(tit,dis->width);
    pairii** coordinates = calloc(OBJ_TYPE_SIZE,sizeof(pairii*));
    for(int i=0;i<dim;++i){
        //Iterate through each type of element
        if(dimens[i]>0){
            Wlabel* w=wl_ini(text[i],fsubtitle,10);
            Canvas * c2=wl_render(w,dis->width);
            canv_appendVI(c,c2);
        }
        else continue;
        Canvas * b=dat[i][0];
        coordinates[i]=calloc(dimens[i],sizeof(pairii));
        for(int j=1;j<dimens[i];++j){
            Canvas*bc=canv_backGrnd(0,0,0,0,10,10);
            canv_appendHI(b,bc);
            coordinates[i][j].se=canv_getWidth(b);
            canv_appendHI(b,dat[i][j]);
        }
        for(int j=0;j<dimens[i];++j){
            coordinates[i][j].fi=canv_getHeight(c);
            coordinates[i][j].se+=(canv_getWidth(c)-canv_getWidth(b))/2;
        }
        canv_appendVI(c,b);
    }
    Canvas* back=disp_Render(dis);
    Canvas* back2=canv_blur(back,BLUR_RAD);
    canv_free(back);
    canv_darken(back2,DARKEN);
    canv_addOverlay(back2,c,10,0);
    canv_print(stdout,back2,0,0);
    char cha;
    int selindex=inv_getSelectedIndex(inv,WEAPON);

    while(1){
        cha=getch1();
        switch(cha){
            case 'A':
                inv_incrementSelected(inv, WEAPON,-1);
                break;
            case 'D':
                inv_incrementSelected(inv, WEAPON,1);
                break;
            case 'E': case 'Q':
                goto END;
                break;
        }
        Canvas* nsel=inv_renderObj(inv,WEAPON,sizes[WEAPON].fi,sizes[WEAPON].se,ftext,fnumbers,selindex,false);
        canv_print(stdout,nsel,coordinates[WEAPON][selindex].fi+10,coordinates[WEAPON][selindex].se+1);
        canv_free(nsel);

        selindex=inv_getSelectedIndex(inv,WEAPON);
        Canvas* sel=inv_renderObj(inv,WEAPON,sizes[WEAPON].fi,sizes[WEAPON].se,ftext,fnumbers,selindex,true);
        canv_print(stdout,sel,coordinates[WEAPON][selindex].fi+10,coordinates[WEAPON][selindex].se+1);
        canv_free(sel);
    }
END:
    canv_free(back2);
    Canvas* ccc=disp_Render(dis);
    if(!ccc)return NULL;
    canv_print(stdout,ccc,0,0);
    return dis;
}

Canvas* _disp_renderCraftingWindow(Display* dis, Recipe** rec, Inventory* inv, int size, pairii* coord){
    if(!dis||!inv)return NULL;

    int gap_w=10;
    Pixel* nsel=pix_ini(0,0,0,0);
    Canvas* dot=canv_circle(nsel,CIRC_RAD);

    pix_free(nsel);
    if(!size){
        Wlabel* wl=wl_ini("Recipies",fcat_lookup(M8),0);
        Canvas* wl_r=wl_render(wl,dis->width);
        int marg=15;
        Canvas* wl_rr=canv_AdjustCrop(wl_r,dis->width,canv_getHeight(wl_r)+marg);
        Canvas* back=disp_Render(dis);
        Canvas* back2=canv_blur(back,BLUR_RAD);

        canv_darken(back2,DARKEN);
        canv_addOverlay(back2,wl_rr,0,0);

        wl_free(wl);
        canv_free(wl_r);
        canv_free(wl_rr);
        canv_free(back);
        return back2;
    }
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

    Canvas* lat=canv_backGrnd(0,0,0,0,5,1);
    Canvas* fdot_1 =canv_AdjustCrop(dot,canv_getWidth(dot),hei);
    Canvas* fdot=canv_appendH(fdot_1,lat);

    canv_free(lat);
    canv_free(fdot_1);



    Canvas* gap=canv_backGrnd(0,0,0,0,wid,gap_w);
    int margin=10;
    int box_w=20;
    Canvas* cc=rec_render(rec[0],ob_wid,wid,hei,dis->width-2*margin-box_w);
    Canvas* c;
    c=canv_appendH(fdot,cc);
    coord[0].fi=0;
    coord[0].se=canv_getWidth(c);
    for(int i=1;i<size;++i){
        canv_appendVI(c,gap);
        Canvas* c2=rec_render(rec[i],ob_wid,wid,hei,dis->width-2*margin-box_w);
        Canvas* c3;
        c3=canv_appendH(fdot,c2);
        coord[i].fi=canv_getHeight(c);
        coord[i].se=canv_getWidth(c3);
        canv_appendVIA(c,c3,RIGHT);
        canv_free(c2);
        canv_free(c3);
    }


    Wlabel* wl=wl_ini("Recipies",fcat_lookup(M8),0);
    Canvas* wl_r=wl_render(wl,dis->width);
    int marg=15;
    Canvas* wl_rr=canv_AdjustCrop(wl_r,dis->width,canv_getHeight(wl_r)+marg);

    for(int i=0;i<size;++i){
        coord[i].se=canv_getWidth(c)-coord[i].se +(dis->width-canv_getWidth(c))/2+1;
        coord[i].fi+=canv_getHeight(wl_rr)+ceil(canv_getHeight(fdot)/2.0);
    }

    Canvas* c_r=canv_AdjustCrop(c,dis->width,canv_getHeight(c));
    canv_appendVI(wl_rr,c_r);
    Canvas* back=disp_Render(dis);
    Canvas* back2=canv_blur(back,BLUR_RAD);
    canv_free(back);
    canv_darken(back2,DARKEN);
    canv_addOverlay(back2,wl_rr,0,0);
    wl_free(wl);
    canv_free(c_r);
    canv_free(c);
    canv_free(gap);
    canv_free(cc);
    return back2;
}
Display* _disp_reprintCraft(pairii* coordinates, int size, int selected,Canvas* bckg){
    if(!coordinates)return NULL;

    Pixel* nsel=pix_ini(NSEL_VALS);
    Canvas* dot=canv_circle(nsel,CIRC_RAD);
    Pixel* sel=pix_ini(SEL_VALS);
    Canvas* dotsel=canv_circle(sel,CIRC_RAD);

    for(int i=0;i<size;++i){
        Canvas* back=canv_subCopy(bckg,coordinates[i].fi,coordinates[i].fi+canv_getHeight(dot),coordinates[i].se,coordinates[i].se+canv_getWidth(dot));
        if(i==selected){
            canv_addOverlay(back,dotsel,0,0);
        }
        else{
            canv_addOverlay(back,dot,0,0);
        }
        canv_print(stdout,back,coordinates[i].fi,coordinates[i].se);
    }
}
Display* disp_CraftingWindow(Display* dis,Inventory* inv){
    if(!dis||!inv)return NULL;
    dis->pop_craf=true;
    int size=0;
    Recipe** rec=rdic_getAllDoable(inv,&size);
    if(!rec)return NULL;
    pairii* coordinates =calloc(size,sizeof(pairii));
    if(!coordinates){
        //ERROR CONTROL
    }
    int selindex=0;
    Canvas* base=_disp_renderCraftingWindow(dis,rec,inv,size,coordinates);
    if(!base)return NULL;
    canv_print(stdout,base,0,0);
    _disp_reprintCraft(coordinates,size,selindex,base);


    while(1){
        char c=getch1();
        switch(c){
            case 'W': case 'O':
                selindex++;
                break;
            case 'S': case 'L':
                selindex--;
                break;
            case 'J':
                rec_make(rec[selindex],inv);
                selindex=-10;
                break;
            case 'Q':
                selindex=-10;
                break;

        }
        if(selindex==-10)break;
        else selindex=(selindex+size)%size;
        _disp_reprintCraft(coordinates,size,selindex,base);
    }
    dis->pop_craf=false;
    Canvas* c=disp_Render(dis);
    canv_print(stdout,c,0,0);
    canv_free(c);
    canv_free(base);
    return dis;
}



Room* disp_getrefRoom(Display* dis){
    return dis? dis->room:NULL;
}

Display* disp_setSpriteI(Display* d,int ind, int i){
    if(!d||!d->room)return NULL;
    return room_setSpriteI(d->room,ind,i)? d:NULL;
}
Display* disp_setSpriteJ(Display* d,int ind, int j){
    if(!d||!d->room)return NULL;
    return room_setSpriteJ(d->room,ind,j)? d:NULL;
}

int disp_incPos(Display* d,int index, int i, int j, int* f_i, int *f_j, bool scroll){

    if(d->pop_dial||d->pop_inv||d->pop_inv)return 0;


    int a=room_incPos(d->room, index, i, j,scroll);
    int b=-1;
    int c=-1;
    if(i>0)b=3;
    else if(i<0)b=1;
    if(j>0)c=2;
    else if(j<0)c=4;

    if(a==-1)return -1;
    if(a==b||a==c){
        if(scroll&&disp_scroll(d,0.5*(i>0)-0.5*(i<0),0.5*(j>0)-0.5*(j<0))==1){
            canv_print(stdout,room_getRender(d->room),0,0);
        }
    }
    else{
        room_printMod(d->room,index, 0, 0);
    }
    room_getBSpritePos(d->room,index,f_i,f_j);
    return 0;
}
int disp_chooseWindow(Display* dis, func_trig f, Trigger** dat, int siz){
    if(!dis)return -1;
    bool fail=false;
    bool exit=false;
    bool chosen=false;
    int wid=dis->width/2.5;
    int hei=100;
    int res;
    int ipos=0,jpos=0;
    Window* win;
    Canvas* crender;
    Canvas* rend,*rend2;
    Welem** el;

    //Center the popup
    el=calloc(siz,sizeof(Welem*));

    if(!el){ fail=true; goto END; }
    for(int i=0;i<siz;++i){
        el[i]=we_createLabel(tr_getDesc(dat[i]),fcat_lookup(M6),0);
        if(!el[i]){ fail=true; goto END; }
    }
    win=win_ini("Choose an action:",el,siz,wid,hei,jpos,ipos,fcat_lookup(M8));
    if(!win){ fail=true; goto END; }
    win_setSelected(win,0);


    Pixel* p=pix_ini(255,255,255,255);
    win_addBorder(win,p,1);
    pix_free(p);

    wid=win_getWidth(win);
    hei=win_getHeight(win);
    jpos=dis->width/2-win_getWidth(win)/2;
    ipos=dis->height/2-win_getHeight(win)/2;

    rend=win_render(win);
    if(!rend){ fail=true; goto END; }
    canv_print(stdout,rend,ipos,jpos);
    char c;

    while(1){
        c=getch1();
        switch (c)
        {
        case 'W': case 'O':
            win_incrementSelected(win,-1);
            rend2=win_render(win);
            if(!rend2){ fail=true; goto END; }
            canv_printDiff(stdout,rend2,rend,ipos,jpos-1);
            canv_free(rend);
            rend=rend2;
            rend2=NULL;
            break;

        case 'S': case 'L':
            win_incrementSelected(win,1);
            rend2=win_render(win);
            if(!rend2){ fail=true; goto END; }
            canv_printDiff(stdout,rend2,rend,ipos,jpos-1);
            canv_free(rend);
            rend=rend2;
            rend2=NULL;
            break;

        case 'J':
            chosen=true;
            break;
        case 'Q':
            exit=true;
            break;
        }
       if(exit||chosen)break;
    }
END:

    crender=disp_Render(dis);
    rend2=canv_subCopy(crender,ipos,ipos+hei,jpos,jpos+wid);
    canv_print(stdout,rend2,ipos,jpos);
    for(int i=0;i<siz;++i)we_free(el[i]);
    free(el);
    canv_free(rend);
    canv_free(crender);
    canv_free(rend2);

    if(chosen)res=win_getSelectedIndex(win);
    win_free(win);
    if(chosen) return res;

    return -1;
}

Display* disp_execute(Display* dis, int index, int room_index, void* en){
    if(!dis)return NULL;
    func_trig f =win_getSelectedAction(dis->latWindow[index]);
    trig_type t=win_getSelectedTrigType(dis->latWindow[index]);
    if(tr_needsTrigger(t)){
        int a;
        Trigger** dat =room_getTriggers(dis->room,t,room_index,&a);

        if(a>1){
            int r=disp_chooseWindow(dis,f,dat,a);

            if(r>=0)f(dat[r],en,dis);
        }
        else if (a==1)f(dat[0],en,dis);
        free(dat);
    }
    else{
        f(NULL,en,dis);
    }

    return dis;
}

const Window* disp_getLWindow(Display* d, int windex) {
  if(!d||windex<0||windex>=d->nLatWindow) return NULL;

  return d->latWindow[windex];
}

int disp_getNLatWindow(Display* d) {
  if(!d) return NULL;

  return d->nLatWindow;
}

int* disp_getDimensions(Display* d) {
  if(!d) return NULL;

  int* r=(int*)calloc(3, sizeof(int));
  r[0]=d->width;
  r[1]=d->height;
  r[2]=d->vdiv;
  return r;
}
