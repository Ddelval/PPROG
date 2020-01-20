
#include "Game.h"

extern char* next_world;
struct _Game{
    World** loaded;
    int loadedsize;
};

char* first_world;
Game* game_ini(){
    return calloc(1,sizeof(Game));
}
void game_free(Game* g){
    if(!g)return;
    if(g->loaded){
        for(int i=0;i<g->loadedsize;++i){
            wo_free(g->loaded[i]);
        }
        free(g->loaded);
    }
    free(g);
}
/**
 * @brief Loads all the worlds. 
 * 
 * The ones that have been saved in a previous execution
 * will be loaded from the save instead of the original
 * version.
 * 
 * NOT FINISHED
 * 
 * @param g Game in which the worlds will be loaded
 * @return  NULL if error
 */
Game* game_loadSave(Game* g){
    FILE* f=fopen("savefiles/stat.txt","r");
    if(!f)return game_loadWorlds(g);
    extern int tier;
    fscanf(f,"%d\n",&tier);
    first_world=calloc(256,sizeof(char));
    fgets(first_world,255,f);
    fclose(f);


    char buffer[256];
    char buffer2[256];
    FILE* f2;
    FILE* fp=popen("/bin/ls -1 Worlds/","r");
    if(!fp)return NULL;
    while(fgets(buffer,256,fp)){
        
        if(g->loaded)g->loaded=realloc(g->loaded,sizeof(World*)*(g->loadedsize+1));
        else g->loaded=calloc(1,sizeof(World*));
        if(!g->loaded)return NULL;
        
        buffer[strlen(buffer)-1]=0;
        sprintf(buffer2, "Worlds/%s",buffer);
        f2=fopen(buffer2,"r");
        buffer[strlen(buffer)-4]=0;

        if(f2){
            //g->loaded[g->loadedsize]=wo_readSave(f2);
        }
        else{
            g->loaded[g->loadedsize]=wo_get(buffer);
            if(!g->loaded[g->loadedsize])return NULL;
        }
        
        wo_setName(g->loaded[g->loadedsize],buffer);
        g->loadedsize++;
    }
    return g;
}
/**
 * @brief Loads all the worlds
 * 
 * This function will load all the worlds that 
 * are stored in the folder World/
 * 
 * @param g Game in which the worlds will be loaded
 * @return  NULL if error
 */
Game* game_loadWorlds(Game* g){
    char buffer[256];
    FILE* fp=popen("/bin/ls -1 Worlds/","r");
    if(!fp)return NULL;
    while(fgets(buffer,256,fp)){
        buffer[strlen(buffer)-5]=0;

        if(g->loaded)g->loaded=realloc(g->loaded,sizeof(World*)*(g->loadedsize+1));
        else g->loaded=calloc(1,sizeof(World*));
        if(!g->loaded)return NULL;
        g->loaded[g->loadedsize]=wo_get(buffer);
        if(!g->loaded[g->loadedsize])return NULL;
        wo_setName(g->loaded[g->loadedsize],buffer);
        g->loadedsize++;
    }
    first_world=strdup("Main");
    return g;
    
}
void game_opening(){
    term_init();
}

World* _game_getW(Game*g, char* c){
    for(int i=0;i<g->loadedsize;++i){
        if(strcmp(wo_getName(g->loaded[i]),c)==0){
            return g->loaded[i];
        }
    }
    return NULL;
}

int game_execute(Game* g){
    if(!g)return 1;
    World* p,*n;
    if(wo_launch(_game_getW(g,first_world))==NULL)return 1;
    p=_game_getW(g,first_world);
    while(next_world&&strcmp(next_world,"End")!=0){
        n=_game_getW(g,next_world);
        wo_transferPlayer(n,p);
        if(!wo_launch(n))return 1;
        p=n;
        
    }
    if(next_world&&strcmp(next_world,"End")==0){
        Sprite* s=sdic_lookup(5001);
        canv_print(stdout,(Canvas*)spr_getDispData(s),0,0);
        char c=getch1();
        spr_free(s);
        
    }
    return 0;

}
void game_closing(){
    free(first_world);
    sdic_free();
    edic_free();
    trdic_free();
    fcat_free();
    odic_free();
    term_restore();
}


int _game_chooseStart(){
    Sprite* ssp=sdic_lookup(5002);
    Canvas* cnv=canv_copy(spr_getDispData(ssp));
    Wlabel* wl1= wl_ini("1 - New game",fcat_lookup(M8),0);
    Canvas* c1=wl_renderSmall(wl1,1200);

    Canvas* cc1=canv_subCopy(cnv,200,200+canv_getHeight(c1)+10,200,200+canv_getWidth(c1)+20);
    canv_darken(cc1,0.8);
    canv_addOverlay(cc1,c1,5,10);

    canv_addOverlay(cnv,cc1,260,200);


    canv_print(stdout,cnv,0,0);

    canv_free(c1);
    canv_free(cc1);
    canv_free(cnv);
    wl_free(wl1);
    spr_free(ssp);
    while(1){
        char c=getch1();
        if(c=='1')return 0;
        if(c=='2')return 1;
    }
    return -1;
}



int game_launch(){
    int retval=0;
    game_opening();
    Game* g=game_ini();

    //This would contemplate several options at startup
    int ch=_game_chooseStart();


    Sprite* s=sdic_lookup(5003);
    canv_print(stdout,(Canvas*)spr_getDispData(s),0,0);
    spr_free(s);




    if(game_loadWorlds(g)==NULL){
        retval=1;
        goto END;
    }

    
    if(game_execute(g)){
        retval=1;
    }

END:
    game_free(g);
    game_closing();
    return retval;
}