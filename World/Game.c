
#include "Game.h"

struct _Game{
    World** loaded;
    int loadedsize;
};


Game* game_ini(){
    return calloc(1,sizeof(Game));
}

Game* game_loadWorlds(){
    char buffer[248];
    FILE* fp=popen("/bin/ls -1 Worlds/","r");
    
}
void game_opening(){
    
}

void game_closing(){
    sdic_free();
    edic_free();
    trdic_free();
    fcat_free();
    odic_free();
    term_restore();
}