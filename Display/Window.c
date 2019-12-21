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

Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int jpos, int ipos, const Font* titlef) {
	Window* win=(Window*)calloc(1, sizeof(Window));
	if(!win) return NULL;
	if(!titlef) {
		free(win);
		return NULL;
	}
	win->titlef=titlef;
	if(!win_setMargins(win, 0, 0, 0, 0)) {
		win_free(win);
		return NULL;
	}
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
	if(!we) {
		win_free(win);
		return NULL;
	}
	for(int i=0; i<num_elems; i++) {
		we[i]=we_copy(Win_elem[i]);
		if(!we[i]) {
			for(int j=0; j<i; ++j) {
				we_free(we[j]);
			}
			free(we);
      return NULL;
		}
	}
	win->Win_elem=we;

	win->num_elems=num_elems;
	win->width=wid;
	win->height=hei;
	win->ipos=ipos;
	win->jpos=jpos;
    win->scroll_pos=0;
	win->selected_elem=-1;
	win->backcol = pix_ini(80, 85, 222, 255);
	if(!win->backcol) {
		win_free(win);
		return NULL;
	}
	win->forecol = pix_ini(158, 158, 36, 255);
	if(!win->forecol) {
		win_free(win);
		return NULL;
	}
	return win;
}

void win_free(Window* win) {
	if(!win) return;

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
}


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

Window* win_remWindowElement(Window* win, int index) {
	if(!win||index>win->num_elems||index<win->num_elems) return NULL;
	we_free(win->Win_elem[index]);
	win->selected_elem=-1;
	win->num_elems--;
	/*Actions array handling*/
	return win;
}
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
func_trig win_getSelectedAction(Window* win){
	if(!win||win->selected_elem==-1||win->selected_elem>=win->action_size)return NULL;
	return win->actions[win->selected_elem];
}
func_trig win_getAction(Window* win, int index){
	if(!win||index>=win->action_size)return NULL;
	return win->actions[index];
}
trig_type win_getSelectedTrigType(Window * win){
	if(!win||win->selected_elem==-1||win->selected_elem>=win->action_size)return -1;
	return win->act_type[win->selected_elem];
}
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

Window* win_redraw(Window* win, int width, int height, int i, int j) {
    if(!win) return NULL;
    win->width=width;
    win->height=height;
    win->ipos=i;
    win->jpos=j;
	if(!win_render(win)) return NULL;
	return win;
}

Window* win_setSelected(Window* win, int selected_elem) {
	if(!win) return NULL;
	if(selected_elem>=win->num_elems) return NULL;
	if(win->selected_elem>=0) {
		if(!we_setBackColor(win->Win_elem[win->selected_elem], pix_retR(win->backcol), pix_retG(win->backcol), pix_retB(win->backcol), pix_retA(win->backcol))) return NULL;
	}
	// FILE* f=fopen("file", "w");
	// fprintf(f, "Hola");
	// fclose(f);
	if(!we_setBackColor(win->Win_elem[selected_elem], pix_retR(win->forecol), pix_retG(win->forecol), pix_retB(win->forecol), pix_retA(win->forecol))) return NULL;
	win->selected_elem=selected_elem;
	return win;
}

Window* win_incrementSelected(Window* win, int incr) {
	if(!win) return NULL;
	if(win->selected_elem<0) {
		if(!win_setSelected(win, 0)) return NULL;
		return win;
	}
	if(!win_setSelected(win, (win->selected_elem+incr+win->num_elems*(incr/win->num_elems+1))%win->num_elems)) return NULL;
	return win;
}

Welem* win_getSelected(Window* win) {
	if(!win || win->selected_elem==-1 || !win->Win_elem) return NULL;
	Welem* we=we_copy(win->Win_elem[win->selected_elem]);
	if(!we) return NULL;
	return we;
}

int win_getSelectedIndex(Window* win) {
	if(!win || !win->Win_elem) return -1;
	return win->selected_elem;
}

Window* win_scrollDown(Window* win) {
	if(!win) return NULL;
    win->scroll_pos+=win->height;
	if(!win_render(win)) return NULL;
	win->scroll_pos++;
	return win;
}

Window* win_scrollUp(Window* win) {
	if(!win) return NULL;
    win->scroll_pos=max(0,win->scroll_pos-win->height);
	if(!win_render(win)) return NULL;
	win->scroll_pos--;
	return win;
}

Window* win_copy(Window* win) {
	if(!win) return NULL;
    Window* win2=win_ini(win->title, win->Win_elem, win->num_elems, win->width, win->height, win->jpos, win->ipos, win->titlef);
    if(!win2) return NULL;
	win2->scroll_pos=win->scroll_pos;

	win2->act_type=calloc(win->action_size,sizeof(trig_type));
	memcpy(win2->act_type,win->act_type,win->action_size*sizeof(trig_type));

	win2->actions=calloc(win->action_size,sizeof(func_trig));
	memcpy(win2->actions,win->actions,win->action_size*sizeof(func_trig));

	win2->action_size=win->action_size;

    return win2;
}

Window* win_setMargins(Window *win, int lm, int rm, int tm, int bm) {
	if (!win) return NULL;
	win->leftm=lm;
	win->rightm=rm;
	win->topm=tm;
	win->botm=bm;

	return win;
}

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

Window* win_addBorder(Window* win,Pixel* color, int width){
	if(!win||!color)return NULL;
	win->borderWidth=width;
	if(win->borderColor)pix_free(win->borderColor);
	win->borderColor=pix_copy(color);
	win->hasBorder=true;
}
/// Return the widht of the canvas
int win_getWidth(const Window* w) {
	if(!w) return -1;
	return w->width+(w->hasBorder? 4*w->borderWidth:0);
}


/// Return the height of the canvas
int win_getHeight(const Window* w) {
	if(!w) return -1;
	return w->height+(w->hasBorder? 2*w->borderWidth:0);
}

Window* win_setBackColor(Window *win, Pixel* backcol) {
	if (!win || !backcol) return NULL;
	Pixel* t = pix_copy(backcol);
	if(!t) return NULL;
	win->backcol=t;
	return win;
}

Window* win_setForeColor(Window *win, Pixel* forecol) {
	if (!win || !forecol) return NULL;
	Pixel* t = pix_copy(forecol);
	if(!t) return NULL;
	win->forecol=t;
	return win;
}

Pixel* win_getBackColor(Window *win) {
	if (!win) return NULL;
	return win->backcol;
}

Pixel* win_getForeColor(Window *win) {
	if (!win) return NULL;
	return win->forecol;
}
