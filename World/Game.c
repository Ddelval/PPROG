
#include "Game.h"

extern char* next_world;
Entity* player;
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
            game_free(g->loaded[i]);
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
    if(!g)return NULL;
    World* p,*n;
    if(wo_launch(_game_getW(g,"Main"))==NULL)return 1;
    p=_game_getW(g,"Main");
    while(next_world){
        n=_game_getW(g,next_world);
        wo_transferPlayer(n,p);
        if(!wo_launch(n))return 1;
        p=n;
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
    player=edic_lookup(0,NULL);
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
}