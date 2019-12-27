//  ppro
//	Welem.c
//  Created by David del Val on 30/09/2019
//
//

#include "Welem.h"
struct _Welem {
	Wtype t;
	void* dat;
};


/*-----------------------------------------------------------------*/
/// Initalize a window element
/// @param t    Type of the element, must be one of the values included
///             in the Wtype typedef
/// @param data The element that is going to be wrapped by this Welem.
///             It must be a pointer to the type that is described in
///             the other parameter
Welem* we_ini(Wtype t, void* data){
	Welem* w= calloc(1,sizeof(Welem));
	if(!w)return NULL;
    w->t=t;
	if(w->t==LABEL){
		w->dat=wl_copy(data);
	}
    if(w->t==ICONLABEL){
        w->dat=wi_copy(data);
    }
	if(!w->dat){
		free(w);
		return NULL;
	}
  w->t=t;
	return w;
}

/*-----------------------------------------------------------------*/
/// Free the allocated memory
void we_free(Welem* w){
    if(!w)return;
    if(w->t==LABEL){
        wl_free(w->dat);
    }
    if(w->t==ICONLABEL){
        wi_free(w->dat);
    }
    free(w);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a Welem that contains a label.
 * Note that this function takes exactly the same arguments as the 
 * constructor of the label 
 * 
 * @param t     String of text that will be rendered in the label  
 * @param f     Font that will be used to render the label 
 * @param vgap  Vertical gap between several lines of the label
 * @return      Welem* New welem with the label in it
 */
Welem* we_createLabel(char* t, const Font* f,int vgap){
	if(!t||!f)return NULL;
	Wlabel* wl=wl_ini(t,f,vgap);
	if(!wl)return NULL;
	Welem* w=we_ini(LABEL,wl);
	wl_free(wl);
	return w;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a Welem that contains a labic (text+ icon). 
 * 
 * @param t     Text for the labic
 * @param f     Font that will be used to render the text
 * @param vgap  Vertical gap between serveral lines of text
 * @param hgap  Gap between the icon and the text
 * @param c     Canvas that will be used as the icon
 * @param l     Alignment of the labic. That is to say relative positon
 *              of the icon and text
 * @return Welem* 
 */
Welem* we_createLabic(char*t, const Font*f,int vgap,int hgap, Canvas* c,wi_align l){
    if(!t||!f||!c)return NULL;
    Wlabic* w=wi_ini(t, f , vgap, hgap,l);
    if(!w)return NULL;
    if(!wi_setCanvas(w,c)){
        wi_free(w);
        return NULL;
    }
    Welem* ww=we_ini(ICONLABEL, (void*)w);
    wi_free(w);
    return ww;
}

/*-----------------------------------------------------------------*/
/// Return a copy of the given window element
/// @param w Element to be copied
Welem* we_copy(Welem* w){
	if(!w) return NULL;
    void *data=NULL;
    if(w->t==LABEL){
        data= wl_copy((Wlabel*)w->dat);
		Welem* e=we_ini(w->t, data);
		wl_free(data);
        return e;
    }
    if(w->t==ICONLABEL){
        data= wi_copy((Wlabic*)w->dat);
		Welem* e=we_ini(w->t, data);
		wl_free(data);
        return e;
    }

	return NULL;
}

/// Renders this Welem calling the respective function accoring to the type
/// of the data contained in this wrapper
/// @param w    Element to be rendered
/// @param wid  The available width. The element will be centered in that width and
///             the space will be distributed as well as possible according to the element
///             that has to be displayed. For instance, a label might be split in several
///             lines and an icon might be centered in the area
/// @return      A new canvas containing all the
Canvas* we_render(Welem*w, int wid){
	if(!w) return NULL;
    if(w->t==LABEL){
        return wl_render((Wlabel*)w->dat, wid);
    }
    if(w->t==ICONLABEL){
        return wi_render((Wlabic*)w->dat, wid);
    }
	return NULL;
}



/*-----------------------------------------------------------------*/
/// Change the color of this Welem
/// @param w    Element to be selected
/// @param r		Red channel of the background
/// @param g		Green channel of the background
/// @param b		Blue channel of the background
/// @param a		Alpha channel of the background
Welem* we_setBackColor(Welem* w, int r, int g,int b,int a) {
	if(!w) return NULL;
	if(w->t==LABEL) {
		if(!wl_setBackColor((Wlabel*)w->dat, r,g,b,a)) return NULL;
		return w;
	}
	if(w->t==ICONLABEL) {
		if(!wi_setBackColor((Wlabic*)w->dat,r,g,b,a)) return NULL;
		return w;
	}
	return w;
}
