//  ppro
//	Wlabel.h
//  Created by David del Val on 30/09/2019
//
//

#ifndef Wlabel_h
#define Wlabel_h

#include "Utility.h"
#include "Font.h"
#include "Canvas.h"
typedef struct _Wlabel Wlabel;


/// Creates a new label
/// @param t    Text that will be displayed. This data is copied
/// @param f    Pointer to the font that will be used to represent the text.
///             Note that this is not copied, so if the font is changed (which it should not),
///             the display font will also be changed.
/// @param vgap Vertical separation between two lines of text, besides the height of the text itself
Wlabel* wl_ini(char* t, const Font* f, int vgap);

/// Frees the allocated memory
void wl_free(Wlabel* l);

/// Copies the label
/// @param src Label to be copied
Wlabel* wl_copy(Wlabel* src);


/// Returns a canvas with the text displayed
/// @param l Label to be processed
/// @param width Maximum width that the canvas can take;
Canvas* wl_render(Wlabel* l,int width);


#endif /* Wlabel_h */
