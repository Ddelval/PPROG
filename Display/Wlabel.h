//  PPROG
//	Wlabel.h
//  Created by David del Val on 30/09/2019


//  Description:
//  Wlabel is a window element that displays text.
//  To initalize the element, it is required to specify the font,
//  text and vertical spacing.
//  However, the width of the element will be given when it is rendered.
//  This approach allows us to work with different widths for each label.
//  When the label is rendered, the text may be splitted into
//  several lines if it cannot fit in one line without exceeding
//  the given width.

#ifndef Wlabel_h
#define Wlabel_h

#include "Utility.h"
#include "Font.h"
#include "Canvas.h"
typedef struct _Wlabel Wlabel;

/*-----------------------------------------------------------------*/
/// Creates a new label
/// @param t    Text that will be displayed. This data is copied
/// @param f    Pointer to the font that will be used to represent
///             the text. Note that this is not copied, so if the
///             font is changed (which it should not),
///             the display font will also be changed.
/// @param vgap Vertical separation between two lines of text,
///             besides the height of the text itself
Wlabel* wl_ini(char* t, const Font* f, int vgap);

/// Frees the allocated memory
void wl_free(Wlabel* l);

/// Copies the label
/// @param src  Label to be copied
/// @return     A new label with the same data
Wlabel* wl_copy(Wlabel* src);

/*-----------------------------------------------------------------*/
/// Returns a canvas with the text displayed.
/// If the text is too wide to fit on a single lines. It will be
/// splitted into several lines.
/// As long as it is possible, this separation will be done on whitespaces.
/// However, if this is not enough to make the text fit in less width
/// than the maximum specified, it will be cut at any given character.
/// @param l        Label to be processed
/// @param width    Maximum width that the canvas can take;
Canvas* wl_render(Wlabel* l,int width);


#endif /* Wlabel_h */
