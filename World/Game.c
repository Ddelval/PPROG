
#include "Game.h"

extern char* next_world;
struct _Game{
    World** loaded;
    int loadedsize;
};


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
    if(wo_launch(_game_getW(g,"Icarus"))==NULL)return 1;
    p=_game_getW(g,"Icarus");
    while(next_world&&strcmp(next_world,"End")!=0){
        n=_game_getW(g,next_world);
        wo_transferPlayer(n,p);
        if(!wo_launch(n))return 1;
        p=n;
        
    }
    if(next_world&&strcmp(next_world,"End")==0){
        Sprite* s=sdic_lookup(5001);
        canv_print(stdout,spr_getDispData(s),0,0);
        char c=getch1();
        spr_free(s);
        
    }
    return 0;

}
void game_closing(){
    sdic_free();
    edic_free();
    trdic_free();
    fcat_free();
    odic_free();
    term_restore();
}

int game_launch(){
    int retval=0;
    game_opening();
    Game* g=game_ini();
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