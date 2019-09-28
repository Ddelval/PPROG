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
    int padding;
    int whitespace;
    int size;
};
Font* font_ini(int hei, int wid, int size, int spacing, int whitespace){
    Font* f = calloc(1, sizeof(Font));
    if(!f)return NULL;
    f->hei=hei;
    f->wid=wid;
    f->size=size;
    f->id=NULL;
    f->padding=spacing;
    f->whitespace=whitespace;
    f->data= calloc(MAX_CHARS, sizeof(Character*));
    if(!f->data){
        free(f);
        return NULL;
    }
    return f;
}

/// Loads a font from a file
/// <name_length> <name> <size> <width> <height> <spacing> <whitespace>
/// <List of the characters included>, one after the other
/// Canvas with all the characters one after the other
/// Note: The caracters have to be rendered with a white font over a transparent background
/// @param fil file with the font
Font* font_load(FILE* fil){
    int l,si;
    int wid, hei,spa,whitespa;
    fscanf(fil,"%d",&l);
    char* id= calloc(l, sizeof(char));
    if(!id) return NULL;
    fscanf(fil,"%s %d %d %d %d %d",id,&si,&wid,&hei,&spa, &whitespa);
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
    Font* f=font_ini(hei, wid, si,spa,whitespa);
    for(int i=0;i<si;++i){
        tmp=canv_AdjustCrop(split[i], wid, hei);
        f->data[elem[i]]=cha_ini(hei, wid, elem[i],tmp);
    }
    return f;
}
int font_calcWidth(Font* f,char* txt){
    int n=(int)strlen(txt);
    return f->wid*n+(n-1)*f->padding;
}
int font_getHeight(Font* f){
    return f->hei;
}
Canvas* font_getCharacterCanvas(Font* f, char c){
    return cha_getCanvas(f->data[c]);
}
Canvas* font_renderText(Font* f,char* txt){
    int y=0;
    int len=(int)strlen(txt);
    Canvas* tmp;
    Canvas* res=canv_backGrnd(0, 0, 0, 255, font_calcWidth(f, txt), f->hei);
    
    if(!res)return NULL;
    for(int i=0;i<len;++i){
        y+=f->padding;
        if(txt[i]==' '){
            y+=f->whitespace;
        }
        else{
            tmp=font_getCharacterCanvas(f, txt[i]);
            canv_addOverlay(res, tmp, 0,y);
            free(tmp);
            y+=f->wid;
        }
        
    }
    return res;
}
