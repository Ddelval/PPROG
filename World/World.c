//  PPROG
//	World.c
//  Created by David del Val on 15/12/2019



#include "World.h"
#define MAX_NAME_ENTITY 50
char* w_dir="Worlds/";


char* next_world;

extern char* curr_world;
extern int tier;

struct _World{
    char*    name;
    int      id;
    Display* dis;
    Entity*  player;
    Entity** allies;
    Entity** enemies;
    int      allSiz, enSiz;
};

int  _wo_spacecraftMenu();
Display* _wo_gameDisplay(Room* r);
World* wo_save(World* w);
World* wo_readSave(FILE *f);

World* wo_ini(){
    World* w=calloc(1,sizeof(World));
    return w;
}

World* wo_get(char* c){

    if(curr_world)free(curr_world);
    curr_world=strdup(c);

    char* n=calloc(strlen(w_dir)+strlen(c)+5,sizeof(char));
    int pos=0;
    append(n,&pos,w_dir);
    append(n,&pos,c);
    append(n,&pos,".txt");
    FILE* f=fopen(n,"r");
    free(n);
    if(!f)return NULL;
    World* w=wo_load(f);
    fclose(f);
    next_world=NULL;
    return w;
}

Display* _wo_gameDisplay(Room* r){
    if(!r)return NULL;
    char**  ch;
    char**  cn;

    Welem** wel;
    Welem** wec;

    Window* act;
    Window* cont;

    int vdiv=800;
    int w=DISP_WIDTH;
    int h=DISP_HEIGHT;
    int act_size=6;

    char * tit=room_getName(r);
    if(!tit)goto FAIL;
    Display* dis=disp_ini(w,h,r,vdiv,tit,fcat_lookup(M8));
    if(!dis)goto FAIL;

    /** Actions Window **/

    ch=calloc(act_size,sizeof(char*));
    if(!ch)goto FAIL;
    ch[0]="Collect";
    ch[1]="Inventory";
    ch[2]="Craft";
    ch[3]="Quests";
    ch[4]="Talk";
    ch[5]="Enter";
    wel=calloc(act_size,sizeof(Welem*));
    if(!wel)goto FAIL;

    for(int i=0;i<act_size;++i){
        wel[i]=we_createLabel(ch[i],fcat_lookup(M6),0);
        if(!wel[i])goto FAIL;
    }

    act=win_ini("Actions",wel,act_size,w-vdiv-1,h/2-20,0,0,fcat_lookup(M8));
    if(!act)goto FAIL;
    win_addAction(act,trig_give,0,OBTAIN);
    win_addAction(act,trig_showInv,1,SHOW);
    win_addAction(act,trig_showRec,2,SHOW);
    win_addAction(act,trig_showQuest,3,SHOW);
    win_addAction(act,trig_talk,4,TALK);
    win_addAction(act,trig_enter,5,ENTER);
    win_setSelected(act,0);
    /** Controls **/
    int cont_size=5;
    cn=calloc(cont_size,sizeof(char*));
    if(!cn)goto FAIL;
    cn[0]="Move: W,A,S,D";
    cn[1]="Select action: O,L";
    cn[2]="Execute action: J";
    cn[3]="Exit window: Q";
    cn[4]="Exit game: B";

    wec=calloc(cont_size,sizeof(Welem*));
    if(!wec)goto FAIL;
    for(int i=0;i<cont_size;++i){
        wec[i]=we_createLabel(cn[i],fcat_lookup(M6),0);
        if(!wec[i])goto FAIL;
    }

    cont=win_ini("Controls",wec,cont_size,w-vdiv-1,h/2-20,0,0,fcat_lookup(M8));
    if(!cont)goto FAIL;
    disp_AddLWindow(dis,act);
    disp_AddLWindow(dis,cont);
    goto END;

FAIL:
    free(dis);
    dis=NULL;

END:
    free(ch);
    if(wel)for(int i=0;i<act_size;++i)we_free(wel[i]);
    free(wel);
    win_free(act);

    free(cn);
    if(wec) for(int i=0;i<cont_size;++i)we_free(wec[i]);
    free(wec);
    win_free(cont);

    free(tit);
    return dis;
}

void wo_free(World *w){
    if(!w)return;
    for(int i=0;i<w->allSiz;++i)entity_free(w->allies[i]);
    for(int i=0;i<w->enSiz;++i)entity_free(w->enemies[i]);
    free(w->allies);
    free(w->enemies);
    free(w->name);
    entity_free(w->player);
    disp_free(w->dis);
    free(w);
}

Entity* _wo_eload(FILE* f,World * w){
    if(!f)return NULL;
    int id;
    fscanf(f,"%d",&id);
    Entity * e=edic_lookup(id,w->dis);
    int i,j;
    fscanf(f, "%d %d",&i,&j);
    entity_setCoordI(e,i);
    entity_setCoordJ(e,j);
    char* c=calloc(MAX_NAME_ENTITY,sizeof(char));
    fgets(c,MAX_NAME_ENTITY,f);
    int re=0;
    while(re<MAX_NAME_ENTITY&&c[re]==' ')++re;
    char* c2=calloc(strlen(c)-re+1,sizeof(char));
    strcpy(c2,c+re);
    if(strlen(c2)>0)c2[strlen(c2)-1]=0;
    entity_setName(e,c2);
    free(c2);
    free(c);
    return e;
}
/**
 * @brief Reads the world from a file
 * Format:
 * Room
 *
 * player
 * number_of_allies
 * allies
 * number_of_enemies
 * enemies
 *
 * The format of every entity is:
 * id ipos jpos name
 * @param f
 * @return World*
 */
World* wo_load(FILE* f){
    if(!f)return NULL;

    World* w=wo_ini();
    Room* r=room_load(f);
    if(!w||!r){
        wo_free(w);
        return NULL;
    }

    w->name=room_getName(r);
    w->id=room_getId(r);
    w->dis=_wo_gameDisplay(r);
    w->player=_wo_eload(f,w);

    Room* rr=disp_getrefRoom(w->dis);
    room_setPlayer(rr,w->player);
    Sprite* s=entity_getSprite(w->player);
    int i=entity_getCoordI(w->player);
    int j=entity_getCoordJ(w->player);
    spr_free(s);
    int* dim= disp_getDimensions(w->dis);
    int t2,b2,r2,l2;
    int h, w2;
    h=dim[H_DATA];
    w2=dim[VD_DATA];
    if(i-h/2<0){
        t2=0;
        b2=h;
    }
    else if(i+h/2>=room_getHeight(rr)){
        t2=room_getHeight(rr)-h;
        b2=room_getHeight(rr);
    }
    else{
        t2=i-h/2;
        b2=i+h/2;
    }

    if(j-w2/2<0){
        l2=0;
        r2=w2;
    }
    else if(j+w2/2>=room_getWidth(rr)){
        l2=room_getWidth(rr)-w2;
        r2=room_getWidth(rr);
    }
    else{
        l2=j-w2/2;
        r2=j+w2/2;
    }
    //fprintf(stderr,"%s -> %d %d %d %d\n",w->name,i,j,b2,r2);
    //fflush(stderr);
    room_setBounds(rr,t2,l2,b2,r2);

    fscanf(f,"%d",&(w->allSiz));
    w->allies=calloc(w->allSiz,sizeof(Entity*));
    for(int i=0;i<w->allSiz;++i){
        w->allies[i]=_wo_eload(f,w);
        entity_processAlly(w->allies[i]);

    }
    fscanf(f,"%d",&(w->enSiz));
    w->enemies=calloc(w->enSiz,sizeof(Entity*));
    for(int i=0;i<w->enSiz;++i){
        w->enemies[i]=_wo_eload(f,w);
        entity_processEnemy(w->enemies[i]);
    }
    room_free(r);
    free(dim);
    return w;
}
World* wo_transferPlayer(World* next, World* prev){
    if(!next||!prev)return NULL;
    return entity_modPlayer(next->player,prev->player)? next:NULL;
}

World* wo_launch(World* w){
    if(!w)return NULL;
    int ptier=0;
    if(curr_world)free(curr_world);
    curr_world=strdup(w->name);
    Canvas* d=disp_Render(w->dis);
    Room *r=disp_getrefRoom(w->dis);
    canv_print(stdout,d,0,0);
    canv_free(d);
    while(1){
        ptier=tier;
        char c=getch1();
        next_world=NULL;
        if(c=='W'){
            entity_moveUp(w->player,true);
        }
        if(c=='S'){
            entity_moveDown(w->player,true);
        }
        if(c=='A'){
            entity_moveLeft(w->player,true);
        }
        if(c=='D'){
            entity_moveRight(w->player,true);

        }
        if(c=='O'){
            disp_incSelIndex(w->dis,0,-1);
        }
        if(c=='L'){
            disp_incSelIndex(w->dis,0,1);
        }
        if(c=='J'){
            disp_execute(w->dis,0,entity_getRoomIndex(w->player),w->player);
            
        }
        if(c=='B'){
            wo_save(w);
            break;
        }

        if(tier!=ptier){
            wo_save(w);
        }



        Trigger* t=room_checkCombat(r,0);

        if(t){
            Entity* e=tr_getEntityRef(t);
            if(entity_getHasDialog(e)){
                char* nam=entity_getName(e);
                disp_DialogWindow(w->dis,entity_getDialogs(e),nam);
                free(nam);
            }
            int index=combat_launch(w->player,e);
            if(index==-1){
                return NULL;
            }
            if(index==0){
                room_removeOver(r,tr_getSpr_index(t));
                int index=-1;
                for(int i =0;i<w->enSiz;++i)if(e==w->enemies[i])index=i;
                if(index!=-1){
                    entity_free(w->enemies[index]);
                    w->enemies[index]=NULL;
                }


            }
            Canvas* c=disp_Render(w->dis);
            canv_print(stdout,c,0,0);
            canv_free(c);
            tr_free(t);
        }

        if(next_world!=NULL){

            if(strcmp(next_world,"Spacecraft")==0){
                if(_wo_spacecraftMenu()==0){
                    Canvas* c=disp_Render(w->dis);
                    canv_print(stdout,c,0,0);
                    canv_free(c);
                    continue;
                }
            }

            wo_save(w);
            return w;
        }
    }
    return w;
}


int  _wo_spacecraftMenu(){

    Sprite* sp=sdic_lookup(5000);
    Canvas* c=canv_copy(spr_getDispData(sp));

    char* prefix="Go to ";

    int wid=canv_getWidth(c);
    int hei=canv_getHeight(c);
    int offset=0;
    int retval=-1;

    FILE * f=fopen("Dictionaries/space.txt","r");
    Wlabel* wl=wl_ini("Control Module",fcat_lookup(M8),0);
    Canvas* cc=wl_renderSmall(wl,wid);
    Canvas* bcc=canv_subCopy(c,5,10+canv_getHeight(cc),wid/2-canv_getWidth(cc)/2,wid/2+canv_getWidth(cc)/2);
    Canvas* bcc1=canv_blur(bcc,10);
    canv_darken(bcc1,0.4);
    canv_addOverlay(bcc1,cc,5,0);
    offset=canv_getHeight(bcc1);
    canv_addOverlay(c,bcc1,0,wid/2-canv_getWidth(cc)/2);

    canv_free(cc);
    canv_free(bcc);
    canv_free(bcc1);
    wl_free(wl);


    Canvas* can=canv_backGrnd(0,0,0,0,0,0);
    int wor;
    int margin=10;
    fscanf(f,"%d",&wor);
    char** charr=calloc(wor,sizeof(char*));
    pairii* coord =calloc(wor,sizeof(pairii));
    char* buff=calloc(256,sizeof(char));
    if(!charr||!coord||!buff){
        free(coord); free(charr); free(buff);
        spr_free(sp);
        canv_free(can);
        return -1;
    }

    for(int i=0;i<wor;++i){
        int t;
        fscanf(f,"%d\n",&t);
        if(t>tier)continue;
        char*  w=calloc(256,sizeof(char));
        char* fn=calloc(256,sizeof(char));

        if(!w||!fn){
            free(coord); free(charr); free(buff);
            free(w); free(fn);
            spr_free(sp);
            canv_free(can);
            return -1;
        }
        fgets(w,256,f);
        if(w[strlen(w)-1]=='\n')w[strlen(w)-1]=0;

        fgets(fn,256,f);
        if(fn[strlen(fn)-1]=='\n')fn[strlen(fn)-1]=0;

        if(strcmp(fn,curr_world)==0)continue;

        strcpy(buff,prefix);
        strcpy(buff+strlen(prefix),w);
        Wlabel* wla=wl_ini(buff,fcat_lookup(M6),0);
        Canvas* cc=wl_renderSmall(wla,wid);

        Canvas* bcc=canv_subCopy(c,offset+canv_getHeight(can)+margin,offset+canv_getHeight(can)+margin+canv_getHeight(cc),wid/2-canv_getWidth(cc)/2,wid/2+canv_getWidth(cc)/2);
        Canvas* bcc1=canv_blur(bcc,10);
        canv_darken(bcc1,0.6);
        canv_addOverlay(bcc1,cc,0,0);

        coord[i].fi=canv_getHeight(can)+canv_getHeight(cc)/2+margin;
        coord[i].se=canv_getWidth(cc)/2;
        Canvas* cc2=canv_addMargin(bcc1,margin,0,0,0);
        canv_appendVI(can,cc2);
        canv_free(cc);
        canv_free(cc2);
        canv_free(bcc);
        canv_free(bcc1);
        wl_free(wla);
        free(w);
        charr[i]=fn;
    }
    canv_addOverlay(c,can,offset+margin,wid/2-canv_getWidth(can)/2);
    canv_print(stdout,c,0,0);
    canv_free(can);
    free(buff);
    fclose(f);



    Pixel* p=pix_ini(150,150,255,255);
    Canvas* dot=canv_circle(p,10);
    pix_free(p);

    for(int i=0;i<wor;++i){
        coord[i].fi+=offset+margin-canv_getHeight(dot)/2;
        coord[i].se= wid/2-coord[i].se-canv_getWidth(dot)/2-2*margin;
    }

    int pselindex=0;
    int selindex=0;
    while(charr[selindex]==NULL)selindex=(selindex+1+wor)%wor;

    Canvas* covv=canv_subCopy(c,coord[selindex].fi,coord[selindex].fi+canv_getHeight(dot),coord[selindex].se,coord[selindex].se+canv_getWidth(dot));
    canv_addOverlay(covv,dot,0,0);
    canv_print(stdout,covv,coord[selindex].fi,coord[selindex].se);
    pselindex=selindex;
    canv_free(covv);


    char cinput;
    while(true){
        cinput=getch1();
        switch(cinput){
            case 'Q' : case 'E':
                retval=0;
                break;
            case 'O' : case 'W':
                selindex=(selindex-1+wor)%wor;
                while(charr[selindex]==NULL)selindex=(selindex+1+wor)%wor;
                break;
            case 'L' : case 'S':
                selindex=(selindex+1+wor)%wor;
                while(charr[selindex]==NULL)selindex=(selindex-1+wor)%wor;
                break;
            case 'J':
                retval=1;
                break;
        }

        if(retval!=-1)break;

        Canvas* cov=canv_subCopy(c,coord[pselindex].fi,coord[pselindex].fi+canv_getHeight(dot),coord[pselindex].se,coord[pselindex].se+canv_getWidth(dot));
        canv_print(stdout,cov,coord[pselindex].fi,coord[pselindex].se);
        canv_free(cov);


        Canvas* covv=canv_subCopy(c,coord[selindex].fi,coord[selindex].fi+canv_getHeight(dot),coord[selindex].se,coord[selindex].se+canv_getWidth(dot));
        canv_addOverlay(covv,dot,0,0);
        canv_print(stdout,covv,coord[selindex].fi,coord[selindex].se);
        pselindex=selindex;
        canv_free(covv);
    }

    canv_free(dot);
    spr_free(sp);
    free(next_world);
    free(coord);

    if(retval==1)next_world=strdup(charr[selindex]);
    for(int i=0;i<wor;++i)free(charr[i]);
    free(charr);

    return retval;

}

World* wo_getNext(){
    if(next_world==NULL)return NULL;
    char *c =next_world;
    World* w=wo_get(c);
    free(c);
    return w;
}
World* wo_setName(World* wp,char*c){
    if(!wp||!c)return NULL;
    if(wp->name)free(wp->name);
    wp->name=strdup(c);
    return wp;
}
const char* wo_getName(World* wp){
    return wp? wp->name: NULL;
}

/**
 * @brief stores a world in a save file
 * 
 * NOT FINISHED 
 */
World* wo_save(World* w){
    if(!w)return NULL;
    system("mkdir -p savefiles/");
    
    
    FILE* f=fopen("savefiles/stat.txt","w");
    if(!f)return NULL;
    fprintf(f,"%d\n%s",tier,w->name);
    fclose(f);
    
    
    char buff[128];




    sprintf(buff,"savefiles/%s.txt",w->name);
    f=fopen(buff,"w");
    if(!f)return NULL;
    room_saveToFile(disp_getrefRoom(w->dis),f);
    fprintf(f,"\n");
    entity_saveToFile(w->player,f);
    fprintf(f,"\n%d %d\n",entity_getCoordI(w->player),entity_getCoordJ(w->player));

    int siz=0;
    Quest** q=entity_getQuests(w->player,&siz);
    fprintf(f,"%d\n",siz);
    for(int i=0;i<siz;++i)quest_storeinFile(q[i],f);

    int as=0;
    for(int i=0;i<w->allSiz;++i)as+=(w->allies[i]!=NULL);
    fprintf(f,"%d\n",as);
    for(int i=0;i<w->allSiz;++i){
        if(w->allies[i]==NULL)continue;
        entity_saveToFile(w->allies[i],f);
        fprintf(f,"\n%d %d\n",entity_getCoordI(w->allies[i]),entity_getCoordJ(w->allies[i]));
    }
    as=0;
    for(int i=0;i<w->enSiz;++i)as+=(w->enemies[i]!=NULL);
    fprintf(f,"%d\n",as);
    for(int i=0;i<w->enSiz;++i){
        if(w->enemies[i]==NULL)continue;
        entity_saveToFile(w->enemies[i],f);
        fprintf(f,"\n%d %d\n",entity_getCoordI(w->enemies[i]),entity_getCoordJ(w->enemies[i]));
    }
    fclose(f);
    return w;
}
/**
 * @brief Reads s world from a save file
 * 
 * NOT FINISHED 
 */
World* wo_readSave(FILE *f){
    if(!f)return NULL;
    Room* r=room_load(f);
    World* w=wo_ini();
    if(!w||!r){
        room_free(r);
        wo_free(w);
    }
    w->dis= _wo_gameDisplay(r);
    room_free(r);
    r=disp_getrefRoom(w->dis);
    w->player=entity_load(f,w->dis);
    int i,j;
    fscanf(f,"%d %d",&i, &j);
    entity_setCoordI(w->player,i);
    entity_setCoordJ(w->player,j);

    room_setPlayer(r,w->player);

    Sprite* s=entity_getSprite(w->player);
    i=entity_getCoordI(w->player);
    j=entity_getCoordJ(w->player);
    spr_free(s);
    int* dim= disp_getDimensions(w->dis);
    int t2,b2,r2,l2;
    int h, w2;
    h=dim[H_DATA];
    w2=dim[VD_DATA];
    if(i-h/2<0){
        t2=0;
        b2=h;
    }
    else if(i+h/2>=room_getHeight(r)){
        t2=room_getHeight(r)-h;
        b2=room_getHeight(r);
    }
    else{
        t2=i-h/2;
        b2=i+h/2;
    }

    if(j-w2/2<0){
        l2=0;
        r2=w2;
    }
    else if(j+w2/2>=room_getWidth(r)){
        l2=room_getWidth(r)-w2;
        r2=room_getWidth(r);
    }
    else{
        l2=j-w2/2;
        r2=j+w2/2;
    }
    room_setBounds(r,t2,l2,b2,r2);
    free(dim);

    int sp;
    fscanf(f,"%d",&sp);
    
    Entity*sss =edic_lookup(1,NULL);
    Sprite* ss=entity_getSprite(sss);
    

    for(int i=0;i<sp;++i){
        Quest* q=quest_load(f,spr_getDispData(ss));
        entity_addQuest(w->player,q);
        quest_free(q);
    }
    entity_free(sss);
    spr_free(ss);


    fscanf(f,"%d",&w->allSiz);
    w->allies=calloc(w->allSiz,sizeof(Entity*));
    if(!w->allies){
        wo_free(w); return NULL;
    }
    for(int i=0;i<w->allSiz;++i){
        w->allies[i]=entity_load(f,w->dis);
        int n,m;
        fscanf(f,"%d %d",&n, &m);
        entity_setCoordI(w->allies[i],n);
        entity_setCoordJ(w->allies[i],m);
        entity_processAlly(w->allies[i]);
    }

    fscanf(f,"%d",&w->enSiz);
    w->enemies=calloc(w->enSiz,sizeof(Entity*));
    if(!w->enemies){
        wo_free(w); return NULL;
    }
    for(int i=0;i<w->enSiz;++i){
        w->enemies[i]=entity_load(f,w->dis);
        int n,m;
        fscanf(f,"%d %d",&n, &m);
        entity_setCoordI(w->enemies[i],n);
        entity_setCoordJ(w->enemies[i],m);
        entity_processEnemy(w->enemies[i]);
    }
    
    return w;

}