//  PPROG
//	Welem.h
//  Created by David del Val on 30/09/2019

//  Description:
//  This module acts as a wrapper for all the elements that may be
//  inserted into a window. This allows us to work with different
//  ui elements without needing to know their type.
//  Therefore, in this file we can find the utility functions such as
//  we_ini, we_free and we_copy and the functions that are common to all
//  ui elements.
//

#ifndef Welem_h
#define Welem_h

#include "Utility.h"
#include "Canvas.h"
#include "Wlabel.h"
#include "Wlabic.h"
typedef enum {LABEL, ICONLABEL, ICON} Wtype;
typedef struct _Welem Welem;
/*-----------------------------------------------------------------*/
/// Initalize a window element
/// @param t    Type of the element, must be one of the values included
///             in the Wtype typedef
/// @param data The element that is going to be wrapped by this Welem.
///             It must be a pointer to the type that is described in
///             the other parameter
Welem* we_ini(Wtype t, void* data);

/*-----------------------------------------------------------------*/
/// Free the allocated memory
void we_free(Welem* w);
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
Welem* we_createLabel(char* t, const Font* f,int vgap);

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
Welem* we_createLabic(char*t, const Font*f,int vgap,int hgap, Canvas* c,wi_align l);
/*-----------------------------------------------------------------*/
/// Return a copy of the given window element
/// @param w Element to be copied
Welem* we_copy(Welem* w);

/*-----------------------------------------------------------------*/
/// Render this Welem calling the respective function according to the
/// type of the data contained in this wrapper
/// @param w    Element to be rendered
/// @param wid  The available width. The element will be centred in
///             that width and the space will be distributed as well
///             as possible according to the element that has to be
///             displayed. For instance, a label might be split
///             in several lines and an icon might be centred in
///             the area
/// @return     A new canvas containing the representation of the
///             window element
Canvas* we_render(Welem*w, int wid);

/*-----------------------------------------------------------------*/
/// Change the color of this Welem
/// @param w    Element to be selected
/// @param r		Red channel of the background
/// @param g		Green channel of the background
/// @param b		Blue channel of the background
/// @param a		Alpha channel of the background
Welem* we_setBackColor(Welem* w, int r,int g,int b,int a);

#endif /* Welem_h */
