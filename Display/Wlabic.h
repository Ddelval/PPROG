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
typedef enum {TEXT_EAST,TEXT_WEST,TEXT_NORTH,TEXT_SOUTH} wi_align;
/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new Wlabic element.
 * 
 * This function only sets the text portion of the element and the
 * gap between both sections
 * 
 * @param t     Text to be displayed
 * @param f     Font for this text. Note that it will NOT be copied
 * @param vgap  Vertical gap between lines of text
 * @param hgap  Gap between the picture and the text
 * @param l     Relative position of the text
 * @return      New Wlabic
 */
Wlabic* wi_ini(char *t, const Font* f,int vgap, int hgap, wi_align l);

/*-----------------------------------------------------------------*/
/// Free the memory allocated to the Wlabic element
void wi_free(Wlabic* w);

/*-----------------------------------------------------------------*/
/// Copies the given element
Wlabic* wi_copy(const Wlabic* src);

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the Wlabic
 * 
 * If the Wlabic is not as wide as width, it will be centered
 * 
 * @param wi    Wlabic to be rendered
 * @param width Width that the resulting canvas will have
 * @return      Canvas containing the render
 */
Canvas* wi_render (Wlabic* wi, int width);

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the Wlabic
 * 
 * If the Wlabic is not as wide as width, it will be centered
 * 
 * @param wi    Wlabic to be rendered
 * @param width Width that the resulting canvas will have
 * @return      Canvas containing the render
 */
Canvas* wi_renderSmall (Wlabic* wi, int width);

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the canvas of the Wlabic
 * 
 * @param sr    Wlabic whose canvas will be modified
 * @param can   Canvas that will be set in the Wlabic
 * @return      NULL if there was an error,
 *              sr otherwise 
 */
Wlabic* wi_setCanvas(Wlabic* sr, Canvas* can);

/*-----------------------------------------------------------------*/
/// Change the background color for the Wlabic element
/// @param w        Element to be selected
/// @param r		Red channel of the background
/// @param g		Green channel of the background
/// @param b		Blue channel of the background
/// @param a		Alpha channel of the background
Wlabic* wi_setBackColor(Wlabic* w, int r, int g,int b,int a);
#endif /* Wlabic_h */
