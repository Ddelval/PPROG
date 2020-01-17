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
// Selected and not selected colours
#define NSEL_VALS 255,255,255,0
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
/** PROTOTYPES **/
Canvas* _disp_renderCraftingWindow(Display* dis, Recipe** rec, int size, pairii* coord);
Display* _disp_reprintCraft(pairii* coordinates, int size, int selected,bool doable, Canvas* bckg);

/** FUNCTION DECLARATION **/

/*-----------------------------------------------------------------*/
/**
 * @brief Allocates all the memory required for a new Display ADT
 *
 * @param wid   Width of the new display
 * @param hei   Height of the new display
 * @param room  Room that the display will have on its left half
 * @param vdiv  Position of the vertical dividor between both halves
 * @param tit   Title of the display
 * @param titf  Font for the tile
 * @return Display*
 */
Display* disp_ini(int wid, int hei, Room* room, int vdiv,char* tit, const Font* titf){
    if(!room) return NULL;
    Display* dis=(Display*)calloc(1, sizeof(Display));
    if(!dis) return NULL;

    dis->title=calloc(strlen(tit)+1, sizeof(char));
    if(!dis->title){
        disp_free(dis);
        return NULL;
    }
    if(!strcpy(dis->title, tit)){
        disp_free(dis);
        return NULL;
    }

    dis->width=wid;
    dis->height=hei;
    dis->titf=titf;
    dis->vdiv=vdiv;
    dis->room=room_copy(room);
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

/*-----------------------------------------------------------------*/
/**
 * @brief Frees all the memory allocated for this window
 *
 * @param dat Window to free
 */
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

/*-----------------------------------------------------------------*/
/**
 * @brief Adds a window to the right column
 *
 * @param dis   Display where the window will be added.
 * @param w     Window to add to the display
 * @return      NULL if error
 */
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

/*-----------------------------------------------------------------*/
/**
 * @brief Removes a window from the right column
 *
 * @param dis       Display from which the window
 * @param index     Position of the window that will be removed
 * @return          NULL if error
 */
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



/*-----------------------------------------------------------------*/
/**
 * @brief Renders the display by rendering all its components
 *
 *
 * @param dis    Display to be redered
 * @return       A new Canvas containing the graphical
 *               representation of the display
 */
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
    left=room_getRender(dis->room);
    if(!left)goto CLEAN;
    dis->topm=(canv_getHeight(left)-canv_getHeight(right))/2;
    Canvas* vbar=canv_backGrnd(255,255,255,255,LINE_WIDTH*2,canv_getHeight(right));
    t=canv_appendHNL(left,vbar);
    t2=canv_appendHNL(right,vbar);
    res =canv_appendHNL(t, t2);
    canv_free(vbar);

CLEAN:
    canv_free(left);
    canv_free(right);
    canv_free(bar);
    canv_free(t);
    canv_free(t2);
    return res;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window associated with this display
 *
 * @param dis Display whose window has to be scrolled
 * @param i   Horizontal percentage of the room that will
 *            be scrolled
 * @param j   Vertical percentage of the room that will
 *            be scrolled
 *
 * @return  -1 if there was an error
 *           0 if there was no scrolling to be done
 *           1 if the room scrolled
 */
int disp_scroll(Display* dis,double i,double j){
    if(!dis)return -1;
    return room_scroll(dis->room,i,j);

}

/*-----------------------------------------------------------------*/
/**
 * @brief Prints an updated render of the window in the <index>
 *        position of the right column.
 *
 * This function will calculate the position that this window will
 * have on the screen and print the new render of the window on
 * that position
 *
 * @param dis   Display whose window will be rendered
 * @param index Position of the window in the right column
 * @return Display*
 */
Display* print_Window(Display*dis, int index){
    if(!dis) return NULL;
    if(index<0 || index>dis->nLatWindow) return NULL;
    Canvas* c=win_render(dis->latWindow[index]);
    int ipos=dis->tithei+LINE_WIDTH;

    for(int i=0;i<index;++i){
        ipos+=LINE_WIDTH;
        ipos+=win_getHeight(dis->latWindow[i]);
    }

    canv_print(stdout,c,ipos,dis->vdiv+1+2*LINE_WIDTH);
    canv_free(c);
    return dis;
}


/*-----------------------------------------------------------------*/
/**
 * @brief Enters the dialog mode of the display
 *
 * When this function is called, it will take control
 * of the entire game.
 * Key presses will mean advancing the dialog except 'Q' or 'E',
 * those keys will end the dialog mode and return to
 * the usual gameplay.
 *
 *
 * @param dis   Display
 * @param dman
 * @param ename
 * @return Display*
 */
Display* disp_DialogWindow(Display* dis, DialogMan* dman, char * ename){
    if(!dis||!dman)return NULL;

    char* txt=NULL;
    char* en =NULL;
    bool err=false;
    Canvas *bottom=NULL,*back=NULL,*rend=NULL;
    Canvas *wl_rend=NULL,*result=NULL;
    Canvas *nam_rend=NULL;
    Wlabel* wl=NULL;
    int ipos=0;

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
    free(en);

    int h=150;
    int hmargin=50;
    ipos=dis->height-h;

    rend=disp_Render(dis);
    bottom=canv_subCopy(rend,ipos ,canv_getHeight(rend),0,canv_getWidth(rend));
    back=canv_blur(bottom,10);
    canv_darken(back,DARKEN);
    canv_addOverlay(back,nam_rend,0,0);
    if(!back)goto ERR_END;
    
    txt=dman_getLine(dman);
    if(!txt) txt=strdup("... ... ...");


    wl=wl_ini(txt,fcat_lookup(M4),0);
    free(txt);txt=NULL;
    wl_rend=wl_render(wl, dis->width-100);
    if(!wl_rend)goto ERR_END;

    result=canv_Overlay(back,wl_rend,10+canv_getHeight(nam_rend),hmargin);
    
    canv_print(stdout,result,ipos,0);
    canv_free(result);
    result=NULL;
    char in;
    while(1){
        wl_free(wl); wl=NULL;
        canv_free(wl_rend); wl_rend=NULL;

        //Waits for a new character
        in=getch1();
        if(in=='Q'||in=='E'){
            goto END;
        }

        txt=dman_getLine(dman);
        if(!txt)goto END;
        wl=wl_ini(txt,fcat_lookup(M4),0);
        free(txt);
        txt=NULL;
        wl_rend=wl_render(wl, dis->width-100);
        if(!wl_rend)goto ERR_END;
        result=canv_Overlay(back,wl_rend,10+canv_getHeight(nam_rend),hmargin);
        if(!result)goto ERR_END;
        canv_print(stdout,result,ipos,0);

        canv_free(result); result=NULL;
        canv_free(wl_rend); wl_rend=NULL;
        wl_free(wl); wl=NULL;
    }
    goto END;



ERR_END:
    err=true;
END:
    canv_print(stdout,bottom,ipos,0);
    canv_free(bottom);
    canv_free(nam_rend);
    canv_free(back);
    canv_free(rend);
    free(txt);
    return err? NULL:(Display*)dis;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Enters the inventory mode of this display
 *
 * When this function is called, the inventory window
 * will appear and it will take control of the input.
 *
 * This mode will allow the player to
 *
 * @param dis       Display that will enter the inventory mode
 * @param inv       Inventory that will be displayed
 * @param ftitle    Font for the "Inventory" text
 * @param fsubtitle Font for the title of sections
 * @param ftext     Font for the name of items
 * @param fnumbers  Font for the amount of items
 * @return          NULL if error
 */
Display* disp_InventoryWindow(Display* dis, Inventory* inv, Font* ftitle, Font* fsubtitle, Font* ftext, Font* fnumbers){
    if(!dis||!inv)return NULL;
    int dim;
    int *dimens=NULL;
    char ** text=NULL;
    pairii* sizes=NULL;
    pairii** coordinates=NULL;
    pairii* titlecoord=NULL;

    Canvas *** dat=inv_render(inv,&dim,&dimens,&text,&sizes,ftext,fnumbers);
    Wlabel* tit=wl_ini("Inventory",ftitle,10);
    if(!tit||!dat||!dimens||!text||!sizes)goto END;

    Canvas* c=wl_render(tit,dis->width);
    coordinates = calloc(OBJ_TYPE_SIZE,sizeof(pairii*));
    titlecoord=calloc(OBJ_TYPE_SIZE,sizeof(pairii));
    if(!c||!coordinates||!titlecoord) goto END;

    wl_free(tit);

    for(int i=0;i<dim;++i){
        char* c=calloc(strlen(text[i])+2,sizeof(char));
        c[0]=' ';
        strcpy(c+1,text[i]);
        free(text[i]);
        text[i]=c;
    }
    for(int i=0;i<dim;++i){
        //Iterate through each type of element
        if(dimens[i]<=0)continue;
        Wlabel* w;
        Canvas* c2;
        if(i==WEAPON){ //Add the indicator
            char* txt=calloc(strlen(text[i])+2,sizeof(char));
            txt[0]='-';
            strcpy(txt+1,text[i]+1);
            w=wl_ini(txt,fsubtitle,10);
            free(txt);
            c2=wl_render(w,dis->width);
            wl_free(w);
        }
        else{
            w=wl_ini(text[i],fsubtitle,10);
            c2=wl_render(w,dis->width);
            wl_free(w);
        }
        
        titlecoord[i].fi=canv_getHeight(c);
        titlecoord[i].se=canv_getWidth(c2);
        canv_appendVI(c,c2);
        canv_free(c2);
        
        Canvas * b=dat[i][0];
        coordinates[i]=calloc(dimens[i],sizeof(pairii));
        for(int j=1;j<dimens[i];++j){
            Canvas*bc=canv_backGrnd(0,0,0,0,10,10);
            canv_appendHI(b,bc);
            canv_free(bc);
            coordinates[i][j].se=canv_getWidth(b);
            canv_appendHI(b,dat[i][j]);
        }
        for(int j=0;j<dimens[i];++j){
            coordinates[i][j].fi=canv_getHeight(c);
            coordinates[i][j].se+=(canv_getWidth(c)-canv_getWidth(b))/2;
        }
        canv_appendVI(c,b);
    }
    for(int i=0;i<OBJ_TYPE_SIZE;++i){
        titlecoord[i].se=canv_getWidth(c)/2-titlecoord[i].se/2;
        titlecoord[i].fi+=10;
    }

    Canvas* back=disp_Render(dis);
    Canvas* back2=canv_blur(back,BLUR_RAD);
    canv_free(back);
    canv_darken(back2,DARKEN);

    Canvas* back3=canv_copy(back2);
    canv_addOverlay(back2,c,10,0);
    canv_free(c);
    canv_print(stdout,back2,0,0);
    char cha;
    int selindex=inv_getSelectedIndex(inv,WEAPON);
    int typesel=WEAPON;
    while(1){
        cha=getch1();

        Wlabel* w;
        Canvas * c2;
        Canvas* c3;
        char* txt;

        switch(cha){
            case 'W':

                w=wl_ini(text[typesel],fsubtitle,10);
                c2=wl_render(w,dis->width);
                c3= canv_subCopy(back3,titlecoord[typesel].fi,titlecoord[typesel].fi+canv_getHeight(c2),0,canv_getWidth(c2));
                canv_addOverlay(c3,c2,0,0);
                canv_print(stdout,c3,titlecoord[typesel].fi,0);
                canv_free(c3);
                c3=NULL;


                typesel=(typesel-1+OBJ_TYPE_SIZE)%OBJ_TYPE_SIZE;
                while(dimens[typesel]<=0)typesel=(typesel-1+OBJ_TYPE_SIZE)%OBJ_TYPE_SIZE;


                txt=calloc(strlen(text[typesel])+2,sizeof(char));
                txt[0]='-';
                strcpy(txt+1,text[typesel]+1);
                w=wl_ini(txt,fsubtitle,10);
                free(txt);
                c2=wl_render(w,dis->width);
                c3= canv_subCopy(back3,titlecoord[typesel].fi,titlecoord[typesel].fi+canv_getHeight(c2),0,canv_getWidth(c2));
                canv_addOverlay(c3,c2,0,0);
                canv_print(stdout,c3,titlecoord[typesel].fi,0);
                canv_free(c3);
                c3=NULL;



                break;
            case 'S':

                w=wl_ini(text[typesel],fsubtitle,10);
                c2=wl_render(w,dis->width);
                c3= canv_subCopy(back3,titlecoord[typesel].fi,titlecoord[typesel].fi+canv_getHeight(c2),0,canv_getWidth(c2));
                canv_addOverlay(c3,c2,0,0);
                canv_print(stdout,c3,titlecoord[typesel].fi,0);
                canv_free(c3);
                c3=NULL;

                typesel=(typesel+1+OBJ_TYPE_SIZE)%OBJ_TYPE_SIZE;
                while(dimens[typesel]<=0)typesel=(typesel+1+OBJ_TYPE_SIZE)%OBJ_TYPE_SIZE;

                txt=calloc(strlen(text[typesel])+2,sizeof(char));
                txt[0]='-';
                strcpy(txt+1,text[typesel]+1);
                w=wl_ini(txt,fsubtitle,10);
                free(txt);
                c2=wl_render(w,dis->width);
                c3= canv_subCopy(back3,titlecoord[typesel].fi,titlecoord[typesel].fi+canv_getHeight(c2),0,canv_getWidth(c2));
                canv_addOverlay(c3,c2,0,0);
                canv_print(stdout,c3,titlecoord[typesel].fi,0);
                canv_free(c3);
                c3=NULL;



                break;
            case 'A':
                inv_incrementSelected(inv, typesel,-1);
                break;
            case 'D':
                inv_incrementSelected(inv, typesel,1);
                break;
            case 'E': case 'Q':
                goto END;
                break;
        }
        Canvas* nsel=inv_renderObj(inv,typesel,sizes[typesel].fi,sizes[typesel].se,ftext,fnumbers,selindex,false);
        canv_print(stdout,nsel,coordinates[typesel][selindex].fi+10,coordinates[typesel][selindex].se+1);
        canv_free(nsel);

        selindex=inv_getSelectedIndex(inv,typesel);
        Canvas* sel=inv_renderObj(inv,typesel,sizes[typesel].fi,sizes[typesel].se,ftext,fnumbers,selindex,true);
        canv_print(stdout,sel,coordinates[typesel][selindex].fi+10,coordinates[typesel][selindex].se+1);
        canv_free(sel);

    }
END:
    canv_free(back3);
    canv_free(back2);
    
    for(int i=0;i<OBJ_TYPE_SIZE;++i)free(text[i]);
    free(text);
    free(sizes);
    for(int i=0;i<OBJ_TYPE_SIZE;++i)free(coordinates[i]);
    free(coordinates);
    free(titlecoord);
    if(dat){
        for(int i=0;i<OBJ_TYPE_SIZE;++i){
            if(dat[i]){
                for(int j=0;j<dimens[i];++j){
                    fprintf(stderr,"%d %d",i,j); fflush(stderr);
                    canv_free(dat[i][j]);
                }
            }
            free(dat[i]);
        }
    }
    free(dat);

    free(dimens);
    Canvas* ccc=disp_Render(dis);
    if(!ccc)return NULL;
    canv_print(stdout,ccc,0,0);
    canv_free(ccc);
    return dis;
}
/*-----------------------------------------------------------------*/
/**
 * @brief Starts the crafting mode of the display
 *
 * In this mode, the user will only be able to change
 * the selection until a recipe is crafted or the user
 * exits the mode
 *
 * In the display, all the recipies that can be crafted
 * will be shown.
 *
 * @param dis  Display whose crafting mode will be activated
 * @param inv  Inventory that determines whether or not the
 *             player can craft a recipe
 * @return     NULL if error
 */
Display* disp_CraftingWindow(Display* dis,Inventory* inv){
    if(!dis||!inv)return NULL;
    dis->pop_craf=true;
    int size=0;
    Recipe** rec=rdic_getAllDoable(inv,&size);
    if(!rec)return NULL;
    pairii* coordinates =calloc(size,sizeof(pairii));
    if(!coordinates){
        free(rec);
        return NULL;
    }
    int selindex=0;
    Canvas* base=_disp_renderCraftingWindow(dis,rec,size,coordinates);
    if(!base)return NULL;
    canv_print(stdout,base,0,0);
    _disp_reprintCraft(coordinates,size,selindex,rec_doable(inv,rec[selindex]),base);


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
                selindex++;selindex--;
                if(!rec_doable(inv,rec[selindex]))continue;
                Object* ob=odic_lookup(rec_getResult_id(rec[selindex]));

                if(obj_toplace(ob)){

                    Sprite* place=obj_getSprite(ob);
                    Sprite* player=room_getSpriteO(disp_getrefRoom(dis),0);
                    if(room_buildingInterface(disp_getrefRoom(dis),spr_getId(place),spr_getOI(player),spr_getOJ(player),0,0)){
                        rec_decrease(rec[selindex],inv);
                    }
                    spr_free(place);
                    spr_free(player);

                }
                else{
                    rec_make(rec[selindex],inv);
                }
                obj_free(ob);
                selindex=-10;
                break;
            case 'Q':
                selindex=-10;
                break;

        }
        if(selindex==-10)break;
        else selindex=(selindex+size)%size;
        
        _disp_reprintCraft(coordinates,size,selindex,rec_doable(inv,rec[selindex]),base);
    }
    dis->pop_craf=false;
    Canvas* c=disp_Render(dis);
    canv_print(stdout,c,0,0);
    canv_free(c);
    canv_free(base);
    free(rec);
    free(coordinates);
    return dis;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Opens a window to choose between several triggers
 *
 * @param dis   Display to which the window will be attached
 * @param f     Function that will process the selected trigger
 * @param dat   Array of the triggers that can be selected
 * @param siz   Size of the array
 * @return      The selected index when the window closes
 *              -1 if there was any error
 */
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
        char* tmp=tr_getDesc(dat[i]);
        el[i]=we_createLabel(tmp,fcat_lookup(M6),0);
        free(tmp);
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

/*-----------------------------------------------------------------*/
/**
 * @brief Opens the window that displays the quests.
 *
 * @param dis       Display to which the window will be attached
 * @param amount    Number of quests to display
 * @param quests    Quests to display
 * @return          NULL if error
 */
Display* disp_QuestWindow(Display* dis, int amount, Quest** quests){
    if(!quests||!dis)return NULL;

    int hmargin=20;
    int vgap=10;
    Wlabel* wl=wl_ini("Quests",fcat_lookup(M8),0);
    Canvas* tit=wl_render(wl,dis->width-2*hmargin);
    Canvas* cc=canv_backGrnd(0,0,0,0,dis->width-2*hmargin,vgap);
    canv_appendVI(tit,cc);
    canv_free(cc);

    for(int i=0;i<amount;++i){
        Canvas* tmp=quest_render(quests[i],dis->width-2*hmargin);
        canv_appendVI(tit,tmp);
        canv_free(tmp);
    }


    Canvas* backg=disp_Render(dis);
    Canvas* backg2=canv_blur(backg,BLUR_RAD);
    canv_darken(backg2,DARKEN);

    canv_addOverlay(backg2,tit,10,hmargin);
    canv_print(stdout,backg2,0,0);

    char c=getch1();
    while(c!='Q'&&c!='E'){
        c=getch1();
        //fprintf(stderr,"\n%c",c);
    }
    canv_print(stdout,backg,0,0);

    return dis;

}

Display* disp_QuestFulfilledWindow(Display* dis, Quest* quest){
    if(!quest||!dis)return NULL;

    int hmargin=20;
    int vgap=10;
    Wlabel* wl=wl_ini("Quest Completed",fcat_lookup(M8),0);
    Canvas* tit=wl_render(wl,dis->width-2*hmargin);
    Canvas* dd=canv_backGrnd(0,0,0,0,dis->width-2*hmargin,vgap);
    canv_appendVI(tit,dd);
    canv_free(dd);
    wl_free(wl);

    Canvas* tmp=quest_render(quest,dis->width-2*hmargin);
    canv_appendVI(tit,tmp);
    canv_free(tmp);

    char* o=quest_getAsigner(quest);
    char* cc=calloc(120,sizeof(char));
    sprintf(cc,"You have collected all the object required. You shall now return and talk to %s",o);
    free(o);

    Wlabel* wll=wl_ini(cc,fcat_lookup(M6),0);
    Canvas* ccc=wl_render(wll,dis->width-2*hmargin);
    Canvas* cccc=canv_addMargin(ccc,10,0,0,0);
    canv_appendVI(tit,cccc);
    wl_free(wll);
    canv_free(ccc);
    canv_free(cccc);
    free(cc);


    Canvas* backg=disp_Render(dis);
    Canvas* backg2=canv_blur(backg,BLUR_RAD);
    canv_darken(backg2,DARKEN);

    canv_addOverlay(backg2,tit,10,hmargin);
    canv_print(stdout,backg2,0,0);

    char c=getch1(); //Wait for input

    canv_print(stdout,backg,0,0);
    canv_free(backg);
    canv_free(backg2);
    canv_free(tit);
    return dis;

}


/*-----------------------------------------------------------------*/
/**
 * @brief Creates the render of the crafting window of that list
 *        of recipies
 *
 * Note that the selection markers will not be rendered in this
 * function
 *
 *
 * @param dis   Display to which the crafting window will be
 *              attached to
 * @param rec   Array of recipies that will be included in the
 *              window
 * @param size  Amount of elements in the arrays
 * @param coord Array in which the positions of the selectin
 *              indicators will be rendered
 * @return      New canvas with the graphical representation of
 *              the crafting window.
 *              NULL if error
 */
Canvas* _disp_renderCraftingWindow(Display* dis, Recipe** rec, int size, pairii* coord){
    if(!dis)return NULL;

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
    coord[0].fi=0.5*canv_getHeight(c);
    coord[0].se=canv_getWidth(c);
    for(int i=1;i<size;++i){
        canv_appendVI(c,gap);
        Canvas* c2=rec_render(rec[i],ob_wid,wid,hei,dis->width-2*margin-box_w);
        Canvas* c3;
        c3=canv_appendH(fdot,c2);
        coord[i].fi=canv_getHeight(c)+0.5*canv_getHeight(c3);
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
        //coord[i].fi+=canv_getHeight(wl_rr)+ceil(canv_getHeight(fdot)/2.0);
        coord[i].fi+=canv_getHeight(wl_rr)-ceil(CIRC_RAD/2.0);
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

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the render markers on top of the
 *        background render that should include all the
 *        recipies rendered and prints the result
 *
 * @param coordinates  Array of coordinates for the selection
 *                     indicators
 * @param size         Number of elements in the array
 * @param selected     Index of the selected element
 * @param doable       Whether or not the selected recipe is doable
 * @param bckg         Background canvas, with all the recipies
 *                     rendered already
 * @return             NULL if error
 */
Display* _disp_reprintCraft(pairii* coordinates, int size, int selected,bool doable,Canvas* bckg){
    if(!coordinates)return NULL;

    Pixel* nsel=pix_ini(NSEL_VALS);
    Canvas* dot=canv_circle(nsel,CIRC_RAD);
    Pixel* sel;
    if(doable)sel=pix_ini(SEL_VALS);
    else sel=pix_ini(200,0,0,255);
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
    return (Display*)coordinates;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Finds the selected element in the window that is in the
 *        index position and executes the action associated with it
 *
 * @param dis           Display that contains the window that
 *                      will execute the action
 * @param index         Index of the window in the display that
 *                      will execute the action
 * @param room_index    Index of the sprite in the window
 * @param en            Entity associated with the sprite
 * @return              NULL if error
 */
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
        for(int i=0;i<a;++i){
            tr_free(dat[i]);
        }
        free(dat);
    }
    else{
        f(NULL,en,dis);
    }

    return dis;
}



/*-----------------------------------------------------------------*/
/**
 * @brief Sets the horizontal coordinate of the sprite in the
 *        position ind of the overs array of the room.
 *
 * @param d     Display whose room we will modify
 * @param ind   Position of the sprite in the overs array
 * @param i     New i coordinate
 * @return Display*
 */
Display* disp_setSpriteI(Display* d,int ind, int i){
    if(!d||!d->room)return NULL;
    return room_setSpriteI(d->room,ind,i)? d:NULL;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the vertical coordinate of the sprite in the
 *        position ind of the overs array of the room.
 *
 * @param d     Display whose room we will modify
 * @param ind   Position of the sprite in the overs array
 * @param i     New j coordinate
 * @return Display*
 */
Display* disp_setSpriteJ(Display* d,int ind, int j){
    if(!d||!d->room)return NULL;
    return room_setSpriteJ(d->room,ind,j)? d:NULL;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Increments the position of a sprite
 *
 * @param d      Display in which we want to modify the spite
 * @param index  Index of the sprite in the room
 * @param i      Horizontal increment of the position of the sprite
 * @param j      Vertical increment of the position of the sprite
 * @param f_i    Variable in which the new horizontal coordinate will
 *               be stored
 * @param f_j    Variable in which the new horizontal coordinate will
 *               be stored
 * @param scroll Whether or not the room will screen if necessary
 * @return       -1 if error
 *                0 if there was no error
 */
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


/*-----------------------------------------------------------------*/
/**
 * @brief Gets the lateral window in position windex
 *
 * @param d         Display that contains the window
 * @param windex    Index of the window in the display
 * @return          NULL if error
 */
Window* disp_getLWindow(Display* d, int windex) {
  if(!d||windex<0||windex>=d->nLatWindow) return NULL;

  return d->latWindow[windex];
}

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the amount of windows on the right column
 *
 * @param d     Display given
 * @return      -1 if error
 *              number of windows otherwise
 */
int disp_getNLatWindow(Display* d) {
  if(!d) return -1;

  return d->nLatWindow;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the dimensions of the window
 *
 * @param d     Display whose dimensions will be returned
 * @return      Array with the dimensions in this order:
 *                  width
 *                  height
 *                  vertical divisor
 *
 */
int* disp_getDimensions(Display* d) {
  if(!d) return NULL;

  int* r=(int*)calloc(3, sizeof(int));
  r[0]=d->width;
  r[1]=d->height;
  r[2]=d->vdiv;
  return r;
}
/*-----------------------------------------------------------------*/
/**
 * @brief Returns a reference to the room of the display
 *
 * @param dis   Display whose room we obtain
 * @return      Pointer to the room of the display.
 *              It should never be freed
 */
Room* disp_getrefRoom(Display* dis){
    return dis? dis->room:NULL;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Moves the selection focus of a window in the right column
 *        increment positions down
 *
 * @param dis       Display whose selection will be changed
 * @param winIndex  Index of the window whose selection will be changed
 * @param increment Number of positions that the focus will go down
 *                  A negative number to make the focus go up
 * @return          NULL if error
 */
Display* disp_incSelIndex(Display* dis, int winIndex, int increment){
  if(!dis)return NULL;
  if(winIndex>=dis->nLatWindow) return NULL;
  if(win_incrementSelected(dis->latWindow[winIndex],increment)==NULL){

    return NULL;
  }
  return print_Window(dis,winIndex);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Moves the selection focus of a window in the right column
 *        to selIndex
 *
 * @param dis       Display whose selection will be changed
 * @param winIndex  Index of the window whose selection will be changed
 * @param selIndes  New selection element for the window
 * @return          NULL if error
 */
Display* disp_setSelIndex(Display* dis, int winIndex, int selIndex){
  if(!dis)return NULL;
  if(winIndex>=dis->nLatWindow) return NULL;
  if(win_setSelected(dis->latWindow[winIndex],selIndex)==NULL){
    return NULL;
  }
  return print_Window(dis,winIndex);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the select index of a window in the right
 *        column of the display
 *
 * @param dis       Display in which the window is
 * @param winIndex  Index of the window whose selected index will
 *                  be returned
 * @return          -1 if error
 */
int disp_getSelIndex(Display* dis, int winIndex){
  if(!dis) return -1;
  if(winIndex>=dis->nLatWindow) return -1;

  return win_getSelectedIndex(dis->latWindow[winIndex]);
}


Display* disp_InventoryWindow2(Display* dis, Inventory* inv, Font* ftitle, Font* fsubtitle, Font* ftext, Font* fnumbers){
    if(!dis||!inv)return NULL;
    dis->nLatWindow=3;
    int dim;
    int *dimens;
    char ** text;
    pairii* sizes;
    Canvas *** dat=inv_render(inv,&dim,&dimens,&text,&sizes,ftext,fnumbers);
    Wlabel* tit=wl_ini("Inventory",ftitle,10);
    if(!tit||!dat)goto END;

    Canvas* c=wl_render(tit,dis->width);
    pairii** coordinates = calloc(OBJ_TYPE_SIZE,sizeof(pairii*));
    pairii* titlecoord=calloc(OBJ_TYPE_SIZE,sizeof(pairii));
    if(!c||!coordinates||!titlecoord) goto END;


    for(int i=0;i<dim;++i){
        char* c=calloc(strlen(text[i])+2,sizeof(char));
        c[0]=' ';
        strcpy(c+1,text[i]);
        free(text[i]);
        text[i]=c;
    }

    for(int i=1;i<dim;++i){
        //Iterate through each type of element
        if(dimens[i]>0){
            Wlabel* w=wl_ini(text[i],fsubtitle,10);
            Canvas * c2=wl_render(w,dis->width);
            titlecoord[i].fi=canv_getHeight(c);
            titlecoord[i].se=canv_getWidth(c2);
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
    for(int i=0;i<OBJ_TYPE_SIZE;++i){
        titlecoord[i].se=canv_getWidth(c)/2-titlecoord[i].se/2;
        titlecoord[i].fi+=10;
    }

    Canvas* back=disp_Render(dis);
    Canvas* back2=canv_blur(back,BLUR_RAD);
    canv_free(back);
    canv_darken(back2,DARKEN);
    canv_addOverlay(back2,c,10,0);
    canv_print(stdout,back2,0,0);
    char cha;
    int selindex=inv_getSelectedIndex(inv,CONSUMABLE);
    int typesel=CONSUMABLE;
    while(1){
        cha=getch1();

        Wlabel* w;
        Canvas * c2;
        Canvas* c3;
        char* txt;

        switch(cha){
            case 'A':
                inv_incrementSelected(inv, typesel,-1);
                break;
            case 'D':
                inv_incrementSelected(inv, typesel,1);
                break;
            case 'E': case 'Q':
                goto END;
                break;
        }
        Canvas* nsel=inv_renderObj(inv,typesel,sizes[typesel].fi,sizes[typesel].se,ftext,fnumbers,selindex,false);
        canv_print(stdout,nsel,coordinates[typesel][selindex].fi+10,coordinates[typesel][selindex].se+1);
        canv_free(nsel);

        selindex=inv_getSelectedIndex(inv,typesel);
        Canvas* sel=inv_renderObj(inv,typesel,sizes[typesel].fi,sizes[typesel].se,ftext,fnumbers,selindex,true);
        canv_print(stdout,sel,coordinates[typesel][selindex].fi+10,coordinates[typesel][selindex].se+1);
        canv_free(sel);

    }
END:

    canv_free(back2);
    Canvas* ccc=disp_Render(dis);
    if(!ccc)return NULL;
    canv_print(stdout,ccc,0,0);
    dis->nLatWindow=4;
    return dis;
}
