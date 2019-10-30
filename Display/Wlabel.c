//  ppro
//	Wlabel.c
//  Created by David del Val on 30/09/2019
//
//

#include "Wlabel.h"
struct _Wlabel{
	char* txt;
	int tlen;
	int vgap;
	const Font *f;
};

/// Creates a new label
/// @param t    Text that will be displayed. This data is copied
/// @param f    Pointer to the font that will be used to represent the text.
///             Note that this is not copied, so if the font is changed (which it should not),
///             the display font will also be changed.
/// @param vgap Vertical separation between two lines of text, besides the height of the text itself

Wlabel* wl_ini(char* t, const Font* f,int vgap){
	if(!t||!f)return NULL;
	Wlabel* w= calloc(1, sizeof(Wlabel));
	if(!w) return NULL;
	w->f=f;
	w->txt=calloc(strlen(t)+1,sizeof(char));
	strcpy(w->txt, t);
	if(!w->txt){
		wl_free(w);
		return NULL;
	}
	w->vgap=vgap;
	return w;
}
/// Frees the allocated memory
void wl_free(Wlabel* l){
	if(!l)return;
	if(l->txt){
		free(l->txt);
	}
	free(l);
}

/// Copies the label
/// @param src Label to be copied
Wlabel* wl_copy(Wlabel* src){
    return wl_ini(src->txt,src->f,src->vgap);
}

/// Internal function used to split the string to display it.
/// It will always try to split the text where there is a space
/// The endpos argument should be passed to the function
/// @param txt Text to be splitted
/// @param width Available width
/// @param f Font to be used. It is used to calculate the width of a piece of text
/// @param endpos This is used to tell the caller where the copy process ended.
char* _charSplit(char* txt, int width, const Font* f,char** endpos){
	char c=txt[0];
	char* res;
	int n=(int)strlen(txt);
	int ps=-1;
	for(int i=0;i<n;++i){
		if(txt[i]==' ')ps=i;
		c=txt[i];
		txt[i]=0;
		if(font_calcWidth(f, txt)>width){
			txt[i]=c;
			if(ps!=-1){
				txt[ps]=0;
				res=calloc(strlen(txt)+1, sizeof(char));
				strcpy(res, txt);
				txt[ps]=' ';
				*endpos=txt+ps+1;
				return res;
			}
			else{
				txt[i]=0;
				res=calloc(strlen(txt)+1, sizeof(char));
				strcpy(res, txt);
				txt[i]=c;
				*endpos=txt+i;
				return res;
			}
		}
		txt[i]=c;
	}
	*endpos=txt+n;
	res=calloc(strlen(txt)+1, sizeof(char));
	strcpy(res, txt);
	return res;

}


/// Returns a canvas with the text displayed
/// @param l Label to be processed
/// @param width Maximum width that the canvas can take;
Canvas* wl_render(Wlabel* l,int width){
	if(!l)return NULL;
	if(font_calcWidth(l->f, l->txt)<width){
		Canvas* c= font_renderText(l->f, l->txt);
        Canvas*cc=canv_AdjustCrop(c, width, canv_getHeight(c));
        canv_free(c);
        return cc;
	}
	char* endpos=l->txt;
	char*res;
	Canvas* c=NULL;
	while( ((res=_charSplit(endpos, width, l->f, &endpos)))&&(*res)){
		if(!c){
			c=font_renderText(l->f, res);
		}
		else{
			Canvas* aux=font_renderText(l->f, res);
			Canvas* aux2=canv_addMargin(aux, l->vgap, 0, 0, 0);
			Canvas* aux3=canv_appendV(c, aux2);
			canv_free(aux);
			canv_free(aux2);
			canv_free(c);
			c=aux3;
		}
		free(res);
		if(!strlen(endpos))break;
	}
	Canvas* cc=canv_AdjustCrop(c, width, canv_getHeight(c));
	//canv_print(stdout,cc,0,0);
	canv_free(c);
	return cc;
}
Canvas* wl_renderSmall(Wlabel* l,int width){
    if(!l)return NULL;
    if(font_calcWidth(l->f, l->txt)<width){
        Canvas* c= font_renderText(l->f, l->txt);
        //Canvas*cc=canv_AdjustCrop(c, width, canv_getHeight(c));
        //canv_free(c);
        return c;
    }
    char* endpos=l->txt;
    char*res;
    Canvas* c=NULL;
    while( ((res=_charSplit(endpos, width, l->f, &endpos)))&&(*res)){
        if(!c){
            c=font_renderText(l->f, res);
        }
        else{
            Canvas* aux=font_renderText(l->f, res);
            Canvas* aux2=canv_addMargin(aux, l->vgap, 0, 0, 0);
            Canvas* aux3=canv_appendV(c, aux2);
            canv_free(aux);
            canv_free(aux2);
            canv_free(c);
            c=aux3;
        }
        free(res);
        if(!strlen(endpos))break;
    }
    return c;
}
