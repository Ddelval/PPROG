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

/* Prototypes*/
Canvas* _font_getCharacterCanvas(const Font* f, char c);


/// Initalizes the font with these values
/// @param hei          Height of each character
/// @param wid          Widht of the characters
/// @param size         Amount of elements that will be stored
/// @param spacing      The gap left between each letter
/// @param whitespace   The gap left when a ' ' is encountered
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
void font_free(Font* f){
    if(!f)return;
    if(f->data)for(int i=0;i<MAX_CHARS;++i)cha_free(f->data[i]);
    free(f->data);
    free(f->id);
    free(f);
}
/// Loads a font from a file. The format should be:
///
/// <name_length> <name> <size> <width> <height> <spacing> <whitespace>
/// <List of the characters included>, one after the other
/// Canvas with all the characters one after the other
///
/// @param fil file with the font
///
/// @remark The caracters have to be rendered with a white font over a transparent background
Font* font_load(FILE* fil){
	if(!fil)return NULL;
    int l,si;
    int wid, hei,spa,whitespa;
    fscanf(fil,"%d",&l);
    char* id= calloc(l+1, sizeof(char));
    if(!id) return NULL;
    fscanf(fil,"%s %d %d %d %d %d\n",id,&si,&wid,&hei,&spa, &whitespa);
    char* elem= calloc(si+1,sizeof(char));
    if(!elem){
        free(id);
        return NULL;
    }
    //fscanf(fil,"%s",elem);
    fgets(elem,si+1,fil);
    fflush(stdout);
    Canvas* whole=canv_load(fil);
    if(!whole){
        free(elem);
        free(id);
        return NULL;
    }
    int nelem;

    Canvas** split=canv_VSplit(whole, &nelem);
    canv_free(whole);
    if(nelem<strlen(elem)){
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
    f->id=id;
    for(int i=0;i<si;++i){
        tmp=canv_AdjustCrop(split[i], wid, hei);
        f->data[elem[i]]=cha_ini(hei, wid, elem[i],tmp);
        canv_free(split[i]);
    }
    free(elem);
    free(split);
    return f;
}
Font* font_copy(const Font*f){
	if(!f)return NULL;
	Font * nf=font_ini(f->hei, f->wid, f->size, f->padding, f->whitespace);
	if(!nf)return NULL;
	strcpy(nf->id, f->id);
	for(int i=0;i<MAX_CHARS;++i){
		if(f->data[i])nf->data[i]=cha_copy(f->data[i]);
		if(!nf->data[i]){
			font_free(nf);
			return NULL;
		}
	}
	return nf;
	
}
/// Calculate the width that a
/// given string needs to be displayed with a font
int font_calcWidth(const Font* f,char* txt){
    int ch=0,sp=0;
    for(int i=0;i<strlen(txt);++i){
        if(txt[i]==' ')sp++;
        else ch++;
    }
    
    return f->wid*ch+(ch+sp)*f->padding+sp*f->whitespace;
}

/// Get the height of the font
int font_getHeight(const Font* f){
    return f->hei;
}
char* _remQuotes(char* src){
    int cnt=0;
    int n=(int)strlen(src);
    for(int i=0;i<n;++i)cnt+=src[i]=='"';
    char * res= calloc(n+cnt+1, sizeof(char));
    if(!res)return NULL;
    int j=0;
    for(int i=0;i<n;++i){
        if(src[i]=='\"'){
            res[j]='\'';
            j++;
            res[j]='\'';
        }
        else{
            res[j]=src[i];
        }
        ++j;
    }
    return res;
}
/// Renders the given string in a canvas with the given font
/// @param f    Font to be used in the render
/// @param txt  String to be rendered
Canvas* font_renderText(const Font* f,char* txt){
    if(!f)return NULL;
    int y=0;
    int len=(int)strlen(txt);
    char *mod_txt=_remQuotes(txt);
    Canvas* tmp;
    Canvas* res=canv_backGrnd(0, 0, 0, 0, font_calcWidth(f, mod_txt), f->hei);
    if(!res){
        return NULL;
    }
    if(!res)return NULL;
    for(int i=0;i<len;++i){
        y+=f->padding;
        if(mod_txt[i]==' '){
            y+=f->whitespace;
        }
        else{
            //printf("%c",mod_txt[i]);
            fflush(stdout);
            tmp=_font_getCharacterCanvas(f, mod_txt[i]);
            if(!tmp){
                canv_free(res);
                return NULL;
            }
            if(canv_addOverlay(res, tmp, 0,y)==NULL){
                free(tmp);
                canv_free(res);
                return NULL;
            }
            canv_free(tmp);
            y+=f->wid;
        }
        
    }
    free(mod_txt);
    return res;
}
Canvas* _font_getCharacterCanvas(const Font* f, char c){
    return cha_getCanvas(f->data[c]);
}
