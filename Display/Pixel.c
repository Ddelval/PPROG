//  PPROG
//	Pixel.c
//  Created by David del Val on 27/09/2019
//
//

#include "Pixel.h"
#define  MAX_VAL 255

struct _Pixel{
    unsigned char r,g,b,a;
};
/**
 * @brief The pixel ADT represents a color
 * 
 * This ADT also includes all the necessary functions to overlay
 * two colors and deal with transparencies.
 * 
 * In order to do so, aside from the color information, we also
 * store the alpha channel. The higher the alpha value of a pixel,
 * the more opaque it is.
 * 255 -> opaque
 * 0   -> transparent
 */

int _filter(int a){
    if(a>MAX_VAL)a=MAX_VAL;
    if(a<0)a=0;
    return a;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new Pixel
 * 
 * @param r The value of the red channel
 * @param g The value of the green channel
 * @param b The value of the blue channel
 * @param a The value of the alpha channel.
 * @return Pixel* New pixel with these values
 */
Pixel* pix_ini(int r,int g, int b, int a){
    Pixel* p= (Pixel*) calloc(1,sizeof(Pixel));
    if(!p)return NULL;
    p->r=_filter(r);
    p->g=_filter(g);
    p->b=_filter(b);
    p->a=_filter(a);
    return p;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Creates a new transparent pixel
 * 
 * @return Pixel*: a new transparent pixel
 */
Pixel* pix_newTransparent(){
    return pix_ini(0,0,0,0);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Loads the pixel from a file
 * This function was created to set up 
 * a format in which the colors should be stored in files.
 * 
 * @param f File
 * @return Pixel* The pixel containing the data read from the file
 */
Pixel* pix_load(FILE* f){
    if(!f)return NULL;
    int a,r,g,b;
    fscanf(f,"%d, %d, %d, %d]",&a,&r,&g,&b);
    return pix_ini(r, g, b, a);
    
}

/*-----------------------------------------------------------------*/
/// Creates a new pixel with the same values in every channel
/// @param src Pixel whose values will be copied
Pixel * pix_copy(const Pixel* src){
    if(!src)return NULL;
    return pix_ini(src->r, src->g, src->b, src->a);
}

/*-----------------------------------------------------------------*/
/**
 * @brief Averages the value of two pixels
 * 
 * This function may be used to scale down images.
 * For instance it can be aplied to graphics generate
 * in-game whose aspect ratio is important. 
 * In order to maintain the aspect ratio, it is necessary to 
 * shrink them by half in the vertical axis
 * 
 * @param p1 
 * @param p2 
 * @return Pixel* 
 */
Pixel* pix_average(const Pixel* p1,const Pixel*p2){
    if(!p1||!p2) return NULL;
    return pix_ini((p1->r+p2->r)/2,(p1->g+p2->g)/2,(p1->b+p2->b)/2,(p1->a+p2->a)/2);
}

/*-----------------------------------------------------------------*/
/// Blends two pixels. It will take into account the colors and the opacity.
/// @param back The pixel that acts as background
/// @param top The pixel that is placed on top of the previous one
/// @return New pixel with the result of the blend
Pixel* pix_overlay(const Pixel* back, const Pixel* top){
    if(!back||!top)return NULL;
    if(top->a==255)return pix_copy(top);
    if(top->a==0&&back->a==0) return pix_ini(0,0,0,0);
    if(top->a==0) return pix_copy(back);
    if(back->a==0)return pix_copy(top);
    double fr,fg,fb,fa;
    double br,bg,bb,ba;
    double oa,or,og,ob;
    br=back->r/255.0;
    bg=back->g/255.0;
    bb=back->b/255.0;
    ba=back->a/255.0;
    
    fr=top->r/255.0;
    fg=top->g/255.0;
    fb=top->b/255.0;
    fa=top->a/255.0;
    
    
    oa=1-(1-ba)*(1-fa);
    or=(ba*(1-fa)*br+fa*fr)/oa;
    og=(ba*(1-fa)*bg+fa*fg)/oa;
    ob=(ba*(1-fa)*bb+fa*fb)/oa;
    
    oa*=255; or*=255; og*=255; ob*=255;
    return pix_ini((int)round(or), (int)round(og), (int)round(ob), (int)round(oa));
}



/*-----------------------------------------------------------------*/
/// Frees the memory allocated for this pixel
void pix_free(Pixel* p){
    free(p);
}

/*-----------------------------------------------------------------*/
/// Copies the contents of one pixel to another.
/// This function can be useful when we have a grid of pixels
/// and we want to make a part of it equal to another set of pixels
/// @param dest Pixel that will take the new values
/// @param src  Pixel whose values are copied
void pix_copyVals(Pixel*dest, const Pixel*src){
    dest->r=src->r;
    dest->g=src->g;
    dest->b=src->b;
    dest->a=src->a;
}

/*-----------------------------------------------------------------*/
/// Check if two pixels are equal.
/// By definition, for both of them to be equal,
/// the values in all the channels have to be the same
/// @param a First pixel
/// @param b Second pixel
bool pix_equals(const Pixel* a,const Pixel*b){
    return a->r==b->r&&a->g==b->g&&a->b==b->b&&a->a==b->a;
}

/*-----------------------------------------------------------------*/
/// Check if a pixel is transparent
/// @param a Pixel to be checked
bool pix_transparent(const Pixel* a){
    return a->a==0;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Checks if the pixel is half transparent or more
 * 
 * This function will be used to differenciate pixels that 
 * represent shadows from the ones that represent actual objects
 * 
 * @param a Pixel to be tested
 * @return true if the pixel is less than or half opaque
 * @return false if the pixel is more than half opaque
 */
bool pix_halfTransparent(const Pixel* a){
    if(!a)return true;
    return a->a<=255/2+1;
}

/*-----------------------------------------------------------------*/
/// Returns the scape sequence that will change the background
/// color of the terminal to the color of this pixel.
/// @param a Pixel whose color will be taken
char* pix_scapeSeq(const Pixel* a){
    char* c=(char*)malloc(sizeof(char)*25);
    sprintf(c,"%c[%d;%d;%d;%d;%dm",27,48,2,a->r,a->g,a->b);
    return c;
}

/*-----------------------------------------------------------------*/
/// Render a horizontal line of pixels.
/// @param a	Pointer to the array of pixels
/// @param len 	Amount of pixels to be rendered
/// @return An array of chars that, if printed on the screen will 
///         represent this line of pixels.
char * pix_renderLine(const Pixel**a,int len){
    int diff=0;
    for(int i=1;i<len;++i) diff+=!pix_equals(a[i], a[i-1]); //Count differences
    int size=(diff+2)*25+(len+1)+2;
    char * res= (char*) malloc(sizeof(char)*size);
    int ipos=0;
    appendf(res, &ipos, pix_scapeSeq(a[0]));
    char * c =calloc(10, sizeof(char));
    sprintf(c,"%c[1D ", 27);
    appendf(res, &ipos,c);
    //append(res, &ipos, " ");
    for(int i=1;i<len;++i){
        if(!pix_equals(a[i], a[i-1])){
            appendf(res, &ipos, pix_scapeSeq(a[i]));
        }
        append(res, &ipos, " ");
        
    }
    //Pixel* p = pix_ini(0, 0, 0, 0);
    //appendf(res,&ipos,pix_scapeSeq(p));
   // free(p);
    return res;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Reduces the brightness of the pixel
 * 
 * @param p     Pixel to be modified
 * @param light The percentage of light that will remain
 * @return Pixel* A not-null pointer if the process was successful
 */
Pixel* pix_darken(Pixel* p,double light){
    if(!p||light<0)return NULL;
    p->r=min(p->r*light,255);
    p->g=min(p->g*light,255);
    p->b=min(p->b*light,255);
    return p;
}



/* ############################################################### */
/* ###################### SETTERS AND GETTERS #################### */
/* ############################################################### */

/*-----------------------------------------------------------------*/
/// Changes the red value of p to r
Pixel* pix_setR(Pixel* p, int r){
    if(!p)return NULL;
    p->r=r;
    return p;
}

/*-----------------------------------------------------------------*/
/// Changes the green value of p to g
Pixel* pix_setG(Pixel* p, int g){
    if(!p)return NULL;
    p->g=g;
    return p;
}

/*-----------------------------------------------------------------*/
/// Changes the blue value of p to b
Pixel* pix_setB(Pixel* p, int b){
    if(!p)return NULL;
    p->b=b;
    return p;
}

/*-----------------------------------------------------------------*/
/// Changes the alpha value of p to a
Pixel* pix_setA(Pixel* p, int a){
    if(!p)return NULL;
    p->a=a;
    return p;
}

/*-----------------------------------------------------------------*/
/// Returns the value of the red channel of p
int pix_retR(const Pixel* p){
    if(!p)return -1;
    return p->r;
}

/*-----------------------------------------------------------------*/
/// Returns the value of the green channel of p
int pix_retG(const Pixel* p){
    if(!p)return -1;
    return p->g;
}

/*-----------------------------------------------------------------*/
/// Returns the value of the blue channel of p
int pix_retB(const Pixel* p){
    if(!p)return -1;
    return p->b;
}

/*-----------------------------------------------------------------*/
/// Returns the value of the alpha channel of p
int pix_retA(const Pixel* p){
    if(!p)return -1;
    return p->a;
}