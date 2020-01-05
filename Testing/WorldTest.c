#include "World.h"
int main(){
    term_init();
    World* w=wo_get("Cave 1");
    wo_launch(w);
    sdic_free();
    edic_free();
    trdic_free();
    fcat_free();
    odic_free();
    wo_free(w);
    term_restore();
    return 0;
}