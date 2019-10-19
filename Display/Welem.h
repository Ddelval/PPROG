//  ppro
//	Welem.h
//  Created by David del Val on 30/09/2019
//
//

#ifndef Welem_h
#define Welem_h

#include "Utility.h"
#include "Canvas.h"
#include "Wlabel.h"
typedef enum {LABEL, ICONLABEL, ICON} Wtype;
typedef struct _Welem Welem;

/// Initalizes a window element
/// @param t Type of the element, must be one of the values included in the Wtype typedef
/// @param data The element that is going to be wrapped by this Welem. It must be a pointer to
/// the type that is described in the other parameter
Welem* we_ini(Wtype t, void* data);


/// Frees the allocated memory
void we_free(Welem* w);


/// Returns a copy of the given window element
/// @param w Element to be copied
Welem* we_copy(Welem* w);


/// Renders this Welem calling the respective function accoring to the type
/// of the data contained in this wrapper
/// @param w    Element to be rendered
/// @param wid  The available width. The element will be centered in that width and
///             the space will be distributed as well as possible according to the element
///             that has to be displayed. For instance, a label might be split in several
///             lines and an icon might be centered in the area
/// @return     A new canvas containing all the
Canvas* we_render(Welem*w, int wid);
	
#endif /* Welem_h */
