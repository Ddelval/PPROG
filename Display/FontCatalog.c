
#include "FontCatalog.h"

Font** dat=NULL;
/**
 * @brief Initalises the font catalog
 * That is to say it will load all the font files into memory
 * 
 */
void fcat_ini(){
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
/**
 * @brief Frees all the memory allocated for the catalog
 * 
 */
void fcat_free(){
    if(dat){
        for(int i=0;i<FSIZE;++i){
            font_free(dat[i]);
        }
    }
    free(dat);
}
/**
 * @brief Returns a constant reference to a font
 * 
 * @param f name of the font
 * @return  Refence to the font in the catalog
 */
Font* fcat_lookup(fname f){
    if(!dat)fcat_ini();
    if(!dat)return NULL;
    return dat[f];
}
