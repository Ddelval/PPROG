//  PPROG
//	World.c
//  Created by David del Val on 15/12/2019

//valgrind --leak-check=full ./WorldTest 2>out.txt


#include "World.h"
#define MAX_NAME_ENTITY 50
char* w_dir="Worlds/";


char* next_world;


struct _World{
    char*    name;
    int      id;
    Display* dis;
    Entity*  player;
    Entity** allies;
    Entity** enemies;
    int      allSiz, enSiz;
};

World* wo_ini(){
    World* w=calloc(1,sizeof(World));
    return w;
}

World* wo_get(char* c){
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
    int w=1200;
    int h=400;
    char * tit=room_getName(r);
    if(!tit)goto FAIL;
    Display* dis=disp_ini(w,h,r,vdiv,tit,fcat_lookup(M8));
    if(!dis)goto FAIL;

    /** Actions Window **/
    int act_size=6;
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

    /** Controls **/
    int cont_size=3;
    cn=calloc(cont_size,sizeof(char*));
    if(!cn)goto FAIL;
    cn[0]="Move: W,A,S,D";
    cn[1]="Select action: O,L";
    cn[2]="Execute action: J";

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
    for(int i=0;i<act_size;++i)we_free(wel[i]);
    free(wel);
    win_free(act);

    free(cn);
    for(int i=0;i<cont_size;++i)we_free(wec[i]);
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
    return w;
}

World* wo_launch(World* w){
    if(!w)return NULL;
    Canvas* d=disp_Render(w->dis);
    Room *r=disp_getrefRoom(w->dis);
    canv_print(stdout,d,0,0);
    canv_free(d);
    while(1){
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
        if(c=='E'){
            break;
        }
        Trigger* t=room_checkCombat(r,0);

        if(t){
            Entity* e=tr_getEntityRef(t);
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
            
        }
        if(next_world!=NULL){
            return w;
        }
    }
    return w;
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