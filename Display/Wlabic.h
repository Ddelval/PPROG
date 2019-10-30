//  PPROG
//	Wlabic.h
//  Created by David del Val on 20/10/2019
//

/// Description:
/// This ADT is a window element and stores both text and a picture.
/// So far, the only allowed orientation has been horizontal.
/// With the picture on the left and the text on the right.
/// However, in the future, we might expand that

#ifndef Wlabic_h
#define Wlabic_h

#include "Utility.h"
#include "Wlabel.h"

typedef struct _Wlabic Wlabic;
typedef enum {TEXT_RIGHT,TEXT_LEFT} Alignment;
/*-----------------------------------------------------------------*/
/// Create a new Wlabic element.
/// This function only sets the text portion of the element and the
/// gap between both sections
/// @param t    Text to be displayed
/// @param f    Font for this text. Note that it will NOT be copied
/// @param vgap Vertical gap between lines of text
/// @param hgap Gap between the picture and the text
Wlabic* wi_ini(char *t, const Font* f,int vgap, int hgap, Alignment l);

/*-----------------------------------------------------------------*/
/// Free the memory allocated to the Wlabic element
void wi_free(Wlabic* w);

/*-----------------------------------------------------------------*/
/// Copies the given element
Wlabic* wi_copy(const Wlabic* src);

Canvas* wi_render (Wlabic* wi, int width);
Wlabic* wi_setCanvas(Wlabic* sr, Canvas* can);
#endif /* Wlabic_h */
