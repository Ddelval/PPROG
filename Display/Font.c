//  Test
//	Font.c
//  Created by David del Val on 28/09/2019
//
//

#include "Font.h"
#define MAX_CHARS 256
struct _Font{
    Character** data;
    char *id;
    int hei;
    int wid;
    int size;
};
Font* font_ini(int hei, int wid, int size){
    Font* f = calloc(1, sizeof(Font));
    if(!f)return NULL;
    f->hei=hei;
    f->wid=wid;
    f->size=size;
    f->id=NULL;
    f->data= calloc(MAX_CHARS, sizeof(Character*));
    if(!f->data){
        free(f);
        return NULL;
    }
    return f;
}

/// Loads a font from a file
/// <name_length> <name> <size> <width> <height>
/// <List of the characters included>, one after the other
/// Canvas with all the characters one after the other
/// Note: The caracters have to be rendered with a white font over a transparent background
/// @param fil file with the font
Font* font_load(FILE* fil){
    int l,si;
    int wid, hei;
    fscanf(fil,"%d",&l);
    char* id= calloc(l, sizeof(char));
    if(!id) return NULL;
    fscanf(fil,"%s %d %d %d",id,&si,&wid,&hei);
    char* elem= calloc(si,sizeof(char));
    if(!elem){
        free(id);
        return NULL;
    }
    fscanf(fil,"%s",elem);
    Canvas* whole=canv_load(fil);
    if(!whole){
        free(elem);
        free(id);
        return NULL;
    }
    int nelem;
    Canvas** split=canv_VSplit(whole, &nelem);
    canv_free(whole);
    if(nelem<si){
        fprintf(stderr,"Not all the characters were found");
        free(id);
        free(elem);
        for(int i=0;i<siz(split);++i){
            canv_free(split[i]);
        }
        free(split);
        return NULL;
    }
    Canvas* tmp;
    Font* f=font_ini(hei, wid, si);
    for(int i=0;i<si;++i){
        tmp=canv_AdjustCrop(split[i], wid, hei);
        f->data[elem[i]]=cha_ini(hei, wid, elem[i],tmp);
    }
    return f;
}
Character* font_getChar(Font* f,char c){
    return f->data[c];
}
