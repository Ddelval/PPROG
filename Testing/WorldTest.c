#include "World.h"
int main(){
    term_init();
    
    World* w=wo_get("Cave 1");
   
    wo_launch(w);
    wo_free(w);
    while((w=wo_getNext())){
        wo_launch(w);
        wo_free(w);
    }
    sdic_free();
    edic_free();
    trdic_free();
    fcat_free();
    odic_free();
    term_restore();
    return 0;
}