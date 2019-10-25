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


/// Initalizes a window element
/// @param t    Type of the element, must be one of the values included in the Wtype typedef
/// @param data The element that is going to be wrapped by this Welem. It must be a pointer to
///             the type that is described in the other parameter
Welem* we_ini(Wtype t, void* data){
	Welem* w= calloc(1,sizeof(Welem));
	if(!w)return NULL;
	if(w->t==LABEL){
		w->dat=wl_copy(data);
	}
	if(!w->dat){
		free(w);
		return NULL;
	}
  w->t=t;
	return w;
}


/// Frees the allocated memory
void we_free(Welem* w){
    if(w->t==LABEL){
        wl_free(w->dat);
    }
		free(w);
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
    if(w->t==LABEL){
        return wl_render((Wlabel*)w->dat, wid);
    }
	return NULL;
}

/// Returns a copy of the given window element
/// @param w Element to be copied
Welem* we_copy(Welem* w){
    void * data=NULL;
    if(w->t==LABEL){
        data= wl_copy((Wlabel*)w->dat);
        return we_ini(w->t, data);
    }

	return NULL;
}
