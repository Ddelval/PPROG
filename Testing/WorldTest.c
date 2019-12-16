#include "World.h"
int main(){
    World* w=wo_get("s1");
    wo_launch(w);
    sdic_free();
    edic_free();
    trdic_free();
    fcat_free();
    odic_free();
    wo_free(w);
    return 0;
}