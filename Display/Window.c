/*	Window.c	*/

#include "Window.h"
#include <errno.h>
#define MIN_SIZ 3
#define STEP 5
struct _Window {
	char* title;
	Welem** Win_elem;
	int num_elems;
  	int num_elems_siz;
	int selected_elem;
	int width, height;
	int scroll_pos;
	int leftm, rightm, topm, botm;
  	int jpos, ipos;
	Pixel* backcol;
	Pixel* forecol;
	const Font* titlef;
	func_trig* actions;
	trig_type* act_type;
	int action_size;
	bool hasBorder;
	Pixel* borderColor;
	int borderWidth;
};
/**
 * @brief 	The Window is a graphical structure that contains several
 * 			selectable elements
 *
 * A window may contain just labels or labels with icons (Labic)
 * The foreground color will be used for the selected elements while
 * the background one will be used to render the rest
 */


/*----------------------------------------------------------------*/
/**
 * @brief Initalizes the window
 *
 * This function will alocate all the memory for the window and
 * it will copy the Welems to the window structure
 *
 * @param title 	Text for the title of the screen
 * @param Win_elem 	Array with the elements of the window
 * @param num_elems Number of elements in the window
 * @param wid 		Width of the window
 * @param hei 		Height of the window
 * @param jpos 		J-coordinate of the top left corner
 * @param ipos 		I-coordinate of the top left corner
 * @param titlef 	Font for the title of the window
 * @return 			New window
 */
Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int jpos, int ipos, const Font* titlef) {
	if(!title||!titlef)return NULL;

	Window* win=(Window*)calloc(1, sizeof(Window));
	if(!win) return NULL;

	win->titlef=titlef;
	win->title=(char*)calloc(strlen(title)+1,sizeof(char));
	if(!win->title) {
		win_free(win);
		return NULL;
	}
	if(!strcpy(win->title, title)) {
		win_free(win);
		return NULL;
	}

  	win->num_elems_siz=max(num_elems,MIN_SIZ);
	Welem** we=(Welem**)calloc(win->num_elems_siz, sizeof(Welem*));
	if(!we) return win_free(win);
	for(int i=0; i<num_elems; i++) {
		we[i]=we_copy(Win_elem[i]);
		if(!we[i]) return win_free(win);
	}

	win->Win_elem=we;
	win->num_elems=num_elems;
	win->width=wid;
	win->height=hei;
	win->ipos=ipos;
	win->jpos=jpos;
    win->scroll_pos=0;
	win->selected_elem=-1;
	win_setMargins(win, 0, 0, 0, 0);

	win->backcol = pix_ini(80, 85, 222, 255);
	if(!win->backcol) return win_free(win);
	win->forecol = pix_ini(158, 158, 36, 255);
	if(!win->forecol) return win_free(win);

	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Frees all the memory allocated to this window
 *
 * @param win 	Window whose memory will be freed
 * @return 		Always NULL
 */
Window* win_free(Window* win) {
	if(!win) return NULL;

	free(win->title);
	for(int i=0; i<win->num_elems; i++) {
		we_free(win->Win_elem[i]);
	}
	free(win->Win_elem);
	pix_free(win->forecol);
	pix_free(win->backcol);
	free(win->actions);
	free(win->act_type);
	free(win);
	return NULL;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Copies the window
 *
 * @param win 	Window to be copied
 * @return 		A copy of the window
 */
Window* win_copy(Window* win) {
	if(!win) return NULL;
    Window* win2=win_ini(win->title, win->Win_elem, win->num_elems, win->width, win->height, win->jpos, win->ipos, win->titlef);
    if(!win2) return NULL;
	

	win2->num_elems=win->num_elems;
	win2->num_elems_siz=win->num_elems_siz;
	win2->selected_elem=win->selected_elem;
	win2->width=win->width;
	win2->height=win->height;
	win2->scroll_pos=win->scroll_pos;
	win2->leftm=win->leftm;
	win2->rightm=win->rightm;
	win2->topm=win->topm;
	win2->botm=win->botm;
	win2->jpos=win->jpos;
	win2->ipos=win->ipos;
	win2->action_size=win->action_size;
	win2->borderColor=pix_copy(win->borderColor);
	win2->hasBorder=win->hasBorder;
	win2->borderWidth=win->borderWidth;

	

	win2->act_type=calloc(win->action_size,sizeof(trig_type));
	memcpy(win2->act_type,win->act_type,win->action_size*sizeof(trig_type));

	win2->actions=calloc(win->action_size,sizeof(func_trig));
	memcpy(win2->actions,win->actions,win->action_size*sizeof(func_trig));


    return win2;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Appends a Welem to the Window
 *
 * @param win 	Window to which the element will be added
 * @param we 	Element that will be added
 * @return 		NULL in case of error
 */
Window* win_addWindowElement(Window* win, Welem* we){
    if(!win||!we)return NULL;
    if(win->num_elems>win->num_elems_siz) return NULL;
    if(win->num_elems==win->num_elems_siz){
        int nsiz=win->num_elems_siz+STEP;
        Welem** w=calloc(nsiz,sizeof(Welem*));
        if(!w)return NULL;
        for(int i=0;i<win->num_elems;++i){
            w[i]=win->Win_elem[i];
        }
        free(win->Win_elem);
        win->Win_elem=w;
        win->num_elems_siz=nsiz;
    }
    win->Win_elem[win->num_elems]=we_copy(we);
    if(!win->Win_elem[win->num_elems])return NULL;
    win->num_elems++;
    return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Adds several elements to the window
 *
 * @param w		Window to which the elements will be added
 * @param we 	Array of Welems
 * @param n 	Number of elements to add
 * @return 		NULL if error
 */
Window* win_addWelems(Window* w, Welem** we, int n) {
	if(!w||!we) return NULL;

	for(int i=0;i<n;i++) {
		if(!win_addWindowElement(w,we[i])) return NULL;
	}
	return w;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Removes an element from the window
 *
 * @param win  	Window in which the element is
 * @param index Position of the element in the window
 * @return 		NULL in case of error
 */
Window* win_remWindowElement(Window* win, int index) {
	if(!win||index>win->num_elems||index<win->num_elems) return NULL;
	we_free(win->Win_elem[index]);

	if(win->selected_elem==index)win->selected_elem=-1;
	else if (win->selected_elem>index)win->selected_elem--;

	for(int i=index;i<win->num_elems-1;++i){
		win->Win_elem[i]=win->Win_elem[i+1];
	}
	win->Win_elem[win->num_elems-1]=NULL;
	win->num_elems--;

	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Eliminates all the elements in the window
 *
 * @param w Window whose elements will be deleted
 * @return 	NULL if error
 */
Window* win_clear(Window* w) {
	if(!w) return NULL;

	while(w->num_elems)if(!win_remWindowElement(w, w->num_elems-1)) return NULL;
	return w;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Attaches an action to the element in position index
 *
 * Actions are defined by a function and the type of trigger that
 * the function takes.
 * With this definition, we will be able to check for the triggers
 * that are stored in the map (Room) in a given position and
 * apply the function to them
 *
 * @param win 	Window in wich the window element is
 * @param f 	Function that will be attached to the Welem
 * @param index Index of the element in the window
 * @param t 	Type of the trigger accepted by f
 * @return 		NULL in case of error
 */
Window* win_addAction(Window* win,func_trig f, int index,trig_type t){
	if(!win)return NULL;
	if(!win->actions){
		win->actions=calloc(win->num_elems,sizeof(func_trig));
		win->act_type=calloc(win->num_elems,sizeof(trig_type));
	}
	else{
		win->actions=realloc(win->actions,sizeof(func_trig)*win->num_elems);
		win->act_type=realloc(win->act_type,sizeof(trig_type)*win->num_elems);
	}
	win->actions[index]=f;
	win->act_type[index]=t;
	win->action_size=win->num_elems;
	return win;
}
/*-----------------------------------------------------------------*/
/**
 * @brief 	Gets the function that has to be executed when the
 * 			selected element is "pressed"
 *
 * @param win 	Window that has been selected
 * @return 		func_trig associated to the selected Welem
 */
func_trig win_getSelectedAction(Window* win){
	if(!win||win->selected_elem==-1||win->selected_elem>=win->action_size)return NULL;
	return win->actions[win->selected_elem];
}

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the function that has to be executed when a given
 * 	      element of the window is "pressed"
 *
 * @param win 	Window that contains the Welem
 * @param index Index of the element in the window
 * @return 		func_trig associated to the Welem
 */

func_trig win_getAction(Window* win, int index){
	if(!win||index>=win->action_size)return NULL;
	return win->actions[index];
}

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the type of the trigger that is attached to the
 * 		  selected Welem
 *
 * @param win 	Window that contains the Welem
 * @return 		trig_type associated to the Welem
 */
trig_type win_getSelectedTrigType(Window * win){
	if(!win||win->selected_elem==-1||win->selected_elem>=win->action_size)return -1;
	return win->act_type[win->selected_elem];
}

/*-----------------------------------------------------------------*/
/**
 * @brief Renders the window
 *
 * @param win 	Window to be rendered
 * @return 		Canvas* containing the graphical representation of
 * 				the window
 */
Canvas* win_render(Window* win) {
    if(!win||!win->title) return NULL;
    errno=0;
    Wlabel* t_lab=NULL;
    Canvas* back=NULL;
    Canvas* c_tit=NULL;
    Canvas* ele=NULL;
    const Font* f=NULL;
    Canvas* r;
    f=win->titlef;
    if(!f) goto END;

    t_lab=wl_ini(win->title, f, 4);
    if(!t_lab) goto END;

    back=canv_backGrnd(pix_retR(win->backcol), pix_retG(win->backcol), pix_retB(win->backcol), pix_retA(win->backcol), win->width, win->height);
    if(!back)  goto END;


    c_tit=wl_render(t_lab, win->width-win->leftm-win->rightm-10); //padding for the window with its title element
    if(!c_tit) {
        back=NULL;
        goto END;
    }

    for(int i=0;i<win->num_elems;++i) {
        ele=we_render(win->Win_elem[i], win->width-win->leftm-win->rightm);
        if(!ele) {
            back=NULL;
            goto END;
        }
        canv_appendVI(c_tit, ele);
        canv_free(ele);
        ele=NULL;
        if(!c_tit){
            back=NULL;
            goto END;
        }
    }
    if(!canv_addOverlay(back, c_tit, 0, 0)) {
        back=NULL;
        goto END;
    }
	if(win->hasBorder){
		Canvas* bck=canv_backGrnd(pix_retR(win->borderColor),pix_retG(win->borderColor),
								  pix_retB(win->borderColor),pix_retA(win->borderColor),
								  win->width+4* win->borderWidth,win->height+2*win->borderWidth);
		Canvas* tmp= canv_AdjustCrop(back,canv_getWidth(bck),canv_getHeight(bck));
		canv_free(back);
		back=tmp;
		canv_addOverlay(bck,back,0,0);
		canv_free(back);
		back=bck;
	}

END:
    r=NULL;
   if(back){
       r=canv_subCopy(back, win->scroll_pos, win->scroll_pos+win->height+(win->hasBorder? 2*win->borderWidth:0), 0, win->width+(win->hasBorder? 4*win->borderWidth:0));
   }
   wl_free(t_lab);
   canv_free(back);
   canv_free(c_tit);
   canv_free(ele);
   return r;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the index of the selected element in the window
 *
 * @param win  Window whose selected element will be modified
 * @param selected_elem Index of the new selected element
 * @return 	NULL in case of error
 */
Window* win_setSelected(Window* win, int selected_elem) {
	if(!win) return NULL;
	if(selected_elem>=win->num_elems) return NULL;
	if(win->selected_elem>=0) {
		if(!we_setBackColor(win->Win_elem[win->selected_elem], pix_retR(win->backcol), pix_retG(win->backcol), pix_retB(win->backcol), pix_retA(win->backcol))) return NULL;
	}
	if(selected_elem==-1) {	/*	If we want to deselect and not select anything	*/
		win->selected_elem=selected_elem;
		return win;
	}
	if(!we_setBackColor(win->Win_elem[selected_elem], pix_retR(win->forecol), pix_retG(win->forecol), pix_retB(win->forecol), pix_retA(win->forecol))) return NULL;
	win->selected_elem=selected_elem;
	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Increments the index of the selected element
 *
 * @param win  	Window whose selected element will be modified
 * @param incr 	Increment of the selected element index
 * @return 		NULL in case of error
 */
Window* win_incrementSelected(Window* win, int incr) {
	if(!win) return NULL;
	if(win->selected_elem<0) {
		if(!win_setSelected(win, 0)) return NULL;
		return win;
	}
	if(!win_setSelected(win, (win->selected_elem+incr+win->num_elems*(incr/win->num_elems+1))%win->num_elems)) return NULL;
	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Returns a copy of the selected element
 *
 * @param win 	Window from which the element will be copied
 * @return 		New Welem identical to the selected one in win
 */
Welem* win_getSelected(Window* win) {
	if(!win || win->selected_elem==-1 || !win->Win_elem) return NULL;
	Welem* we=we_copy(win->Win_elem[win->selected_elem]);
	if(!we) return NULL;
	return we;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Returns the index of the selected element in the window
 *
 * @param win 	Window whose selected element is being requested
 * @return 		Index of the selected element
 */
int win_getSelectedIndex(Window* win) {
	if(!win || !win->Win_elem) return -1;
	return win->selected_elem;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window down
 *
 * Note that the canges will only be noticeable when the window
 * is rendered again
 *
 * @param win 	Window that will be scrolled down
 * @return 		NULL if there is an error
 */
Window* win_scrollDown(Window* win) {
	if(!win) return NULL;
    win->scroll_pos+=win->height;
	if(!win_render(win)) return NULL;
	win->scroll_pos++;
	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Scrolls the window up
 *
 * Note that the canges will only be noticeable when the window
 * is rendered again
 *
 * @param win 	Window that will be scrolled up
 * @return 		NULL if there is an error
 */
Window* win_scrollUp(Window* win) {
	if(!win) return NULL;
    win->scroll_pos=max(0,win->scroll_pos-win->height);
	if(!win_render(win)) return NULL;
	win->scroll_pos--;
	return win;
}



/*-----------------------------------------------------------------*/
/**
 * @brief Sets the margins in the window
 *
 * When the window is rendered, the margins will be transparent
 *
 * @param win Window whose margins will be set
 * @param lm Left 	margin
 * @param rm Right 	margin
 * @param tm Top 	margin
 * @param bm Bottom margin
 * @return	 NULL if there was an error
 */
Window* win_setMargins(Window *win, int lm, int rm, int tm, int bm) {
	if (!win) return NULL;
	win->leftm=lm;
	win->rightm=rm;
	win->topm=tm;
	win->botm=bm;

	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Gets the margins of the window
 *
 * The order will be: top, right, bottom, left
 *
 * @param win 	Window whose margins we are requesting
 * @return 		Pointer to an array of four integers containing the
 * 				size of the margins
 */
int* win_getMargins(Window *win) {
	if(!win) return NULL;

	int* m=(int*)calloc(4, sizeof(int));
	if(!m) return NULL;
	m[0]=win->topm;
	m[1]=win->rightm;
	m[2]=win->botm;
	m[3]=win->leftm;

	return m;
}
/*-----------------------------------------------------------------*/
/**
 * @brief Adds a border to the window
 *
 *
 * @param win 	Window that will get the border
 * @param color Color of the border
 * @param width Vertical width of the border
 * 				(the horizontal will be double this ammount)
 * @return 		NULL if error
 */
Window* win_addBorder(Window* win,Pixel* color, int width){
	if(!win||!color)return NULL;
	win->borderWidth=width;
	if(win->borderColor)pix_free(win->borderColor);
	win->borderColor=pix_copy(color);
	win->hasBorder=true;
	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the background color of the window
 *
 * @param win 		Window whose background color will be set
 * @param backcol 	Color for the background
 * @return 			NULL if error
 */
Window* win_setBackColor(Window *win, Pixel* backcol) {
	if (!win || !backcol) return NULL;
	Pixel* t = pix_copy(backcol);
	if(!t) return NULL;
	win->backcol=t;
	return win;
}

/*-----------------------------------------------------------------*/
/**
 * @brief Sets the foreground color of the window
 * 		  (color for the selected elements)
 *
 * @param win 		Window whose foreground color will be set
 * @param forecol 	Color for the foreground
 * @return 			NULL if error
 */
Window* win_setForeColor(Window *win, Pixel* forecol) {
	if (!win || !forecol) return NULL;
	Pixel* t = pix_copy(forecol);
	if(!t) return NULL;
	win->forecol=t;
	return win;
}

/*-----------------------------------------------------------------*/
/// Return the background color
Pixel* win_getBackColor(Window *win) {
	if (!win) return NULL;
	return win->backcol;
}

/*-----------------------------------------------------------------*/
/// Return the foreground color
Pixel* win_getForeColor(Window *win) {
	if (!win) return NULL;
	return win->forecol;
}

/*-----------------------------------------------------------------*/
/// Return the widht of the canvas
int win_getWidth(const Window* w) {
	if(!w) return -1;
	return w->width+(w->hasBorder? 4*w->borderWidth:0);
}

/*-----------------------------------------------------------------*/
/// Return the height of the canvas
int win_getHeight(const Window* w) {
	if(!w) return -1;
	return w->height+(w->hasBorder? 2*w->borderWidth:0);
}
