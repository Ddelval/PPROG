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
	int weight;
	int leftm, rightm, topm, botm;
  int jpos, ipos;
	const Font* titlef;
};

Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int weight, int jpos, int ipos, const Font* titlef) {
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
	win->weight=weight;
	win->ipos=ipos;
	win->jpos=jpos;
  win->scroll_pos=0;
	win->selected_elem=-1;
	return win;
}

void win_free(Window* win) {
	if(!win) return;

	free(win->title);
	for(int i=0; i<win->num_elems; i++) {
		we_free(win->Win_elem[i]);
	}
	free(win->Win_elem);
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

    back=canv_backGrnd(80, 85, 222, 255, win->width, win->height);
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

END:
    r=NULL;
   if(back){
       r=canv_subCopy(back, win->scroll_pos, win->scroll_pos+win->height, 0, win->width);
   }
   wl_free(t_lab);
   canv_free(back);
   canv_free(c_tit);
   canv_free(ele);
   return r;
}

Window* win_redraw(Window* win, int width, int height, int weight, int i, int j) {
    if(!win) return NULL;
    win->width=width;
    win->height=height;
    win->weight=weight;
    win->ipos=i;
    win->jpos=j;
	if(!win_render(win)) return NULL;
	return win;
}

Window* win_setSelected(Window* win, int selected_elem) {
	if(!win) return NULL;
	if(win->selected_elem>=0) {
		if(!we_setBackColor(win->Win_elem[win->selected_elem], 80, 85, 222, 255)) return NULL;
	}
	FILE* f=fopen("file", "w");
	fprintf(f, "Hola");
	fclose(f);
	if(!we_setBackColor(win->Win_elem[selected_elem], 158, 158, 36, 255)) return NULL;
	win->selected_elem=selected_elem;
	return win;
}

Window* win_incrementSelected(Window* win, int incr) {
	if(!win) return NULL;
	if(win->selected_elem<0) {
		if(!win_setSelected(win, 0)) return NULL;
		return win;
	}
	if(!win_setSelected(win, (win->selected_elem+incr)%win->num_elems)) return NULL;

}

Welem* win_getSelected(Window* win) {
	if(!win || win->selected_elem==-1 || !win->Win_elem) return NULL;
	Welem* we=we_copy(win->Win_elem[win->selected_elem]);
	if(!we) return NULL;
	return we;
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
        Window* win2=win_ini(win->title, win->Win_elem, win->num_elems, win->width, win->height, win->weight, win->jpos, win->ipos, win->titlef);
        if(!win2) return NULL;
	win2->scroll_pos=win->scroll_pos;
/*
	for(int i=0; i<=MAX_SELECTABLE; ++i) {
		win2->selected_elem[i]=win->selected_elem[i];
	}

	if(win2->num_elems<0) return win2;
	Welem** we=(Welem**)calloc(win2->num_elems, sizeof(Welem*));
	if(!we) win_free(win2);
	for(int i=0; i<win2->num_elems; i++) {
		we[i]=we_copy(win->Win_elem[i]);
		if(!we[i]) {
			for(int j=0; j<i; ++j) {
				we_free(we[j]);
			}
			free(we);
		}
	}
	win2->Win_elem=we;
*/
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

/// Return the widht of the canvas
int win_getWidth(const Window* w) {
	return win->width;
}


/// Return the height of the canvas
int win_getHeight(const Window* w) {
	return win->height;
}
