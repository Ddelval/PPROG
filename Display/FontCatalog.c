
#include "FontCatalog.h"

Font** dat=NULL;
Font* fcat_ini(){
    dat=calloc(FSIZE,sizeof(Font*));
    FILE* f=NULL;
    f=fopen("Display/Fonts/Robo_Mono/04.txt", "r");
    dat[M4] = font_load(f);
    fclose(f);
    f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
    dat[M6] = font_load(f);
    fclose(f);
    f=fopen("Display/Fonts/Robo_Mono/08.txt", "r");
    dat[M8] = font_load(f);
    fclose(f);
}
void fcat_free(){
    for(int i=0;i<FSIZE;++i){
        font_free(dat[i]);
    }
    free(dat);
}
Font* fcat_lookup(fname f){
    if(!dat)fcat_ini();
    if(!dat)return NULL;
    return dat[f];
}
