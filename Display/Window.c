/*	Window.c	*/

#include "Window.h"

struct _Window {
	char* title;
	Welem** Win_elem;
	int num_elems;
	int selected_elem[MAX_SELECTABLE];
	int width, height;
	int scroll_pos;
	int weight;
	int leftm, rightm, topm, botm;
        int ypos, xpos;
};

Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int weight, int ypos, int xpos) {
	Window* win=(Window*)calloc(1, sizeof(Window));
	if(!win) return NULL;

	if(!win_setMargins(win, 0, 0, 0, 0)) {
		free(win);
		return NULL;
	}

	if(!strcpy(win->title, title)) {
		free(win);
		return NULL;
	}
	if(num_elems>0) {
		Welem** we=(Welem**)calloc(num_elems, sizeof(Welem*));
		if(!we) win_free(win);
		for(int i=0; i<num_elems; i++) {
			we[i]=we_copy(Win_elem[i]);
			if(!we[i]) {
				for(int j=0; j<i; ++j) {
					we_free(we[j]);
				}
				free(we);
			}
		}
		win->Win_elem=we;
	}
	win->num_elems=num_elems;
	win->width=wid;
	win->height=hei;
	win->weight=weight;

	return win;
}

void win_free(Window* win) {
	if(!win) return;
	
	free(win->title);
	free(win->selected_elem);
	for(int i=0; i<win->num_elems; i++) {
		we_free(win->Win_elem[i]);
	}
	free(win->Win_elem);
	free(win);
}

Window* win_render(Window* win, int pos) {
	if(!win) return NULL;
        
	Canvas* back = canv_backGrnd(92, 82, 56, 255, win->width, win->height);     // Temporary color for the background
        if(!back) return NULL;
        
        FILE* fi = fopen("/Fonts/Robo_Mono_11.txt","r");
        if(!fi) {
            canv_free(back);
            return NULL;
        }
        Font* f = font_load(fi);
        //check for title size and add it, etc.
	return NULL;
}


Window* win_redraw(Window* win, int width, int height, int weight, int x, int y) {
	if(!win) return NULL;
	win->width=width;
	win->height=height;
	win->weight=weight;
        win->xpos=x;
        win->ypos=y;
	if(!win_render(win, 0)) return NULL;
	return win;
}

Window* win_setSelected(Window* win, int* selected_elem) {
	if(!win || !selected_elem) return NULL;
	for(int i=0; i<=MAX_SELECTABLE; ++i) {
		win->selected_elem[i]=selected_elem[i];
	}
	return win;
}

Welem** win_getSelected(Window* win) {
	if(!win || !win->selected_elem || !win->Win_elem) return NULL;
	Welem** we=(Welem**)calloc(MAX_SELECTABLE, sizeof(Welem*));
	if(!we) return NULL;
	int j=0;
	for(int i=0; i<MAX_SELECTABLE; ++i) {
		we[j]=win->Win_elem[win->selected_elem[i]];
		j++;
	}
	return we;
}

Window* win_scrollDown(Window* win) {
	if(!win) return NULL;
	if(!win_render(win, win->scroll_pos+1)) return NULL;
	win->scroll_pos++;
	return win;
}

Window* win_scrollUp(Window* win) {
	if(!win) return NULL;
	if(!win_render(win, win->scroll_pos-1)) return NULL;
	win->scroll_pos--;
	return win;
}

Window* win_copy(Window* win) {
	if(!win) return NULL;
	Window* win2=(Window*)calloc(1, sizeof(Window));
	if(!win2) return NULL;
	if(!strcpy(win2->title, win->title)) {
		win_free(win2);
		return NULL;
	}
	win2->num_elems=win->num_elems;
	win2->height=win->height;
	win2->weight=win->weight;
	win2->scroll_pos=win->scroll_pos;

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
	m[0]=win->leftm;
	m[1]=win->rightm;
	m[2]=win->topm;
	m[3]=win->botm;

	return m;
}
