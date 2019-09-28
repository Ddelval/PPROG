/*	Window.c	*/

#include "Window.h"

struct _Window {
	char* title;
	Welem** Win_elem;
	int num_elems;
	int selected_elem[MAX_SELECTABLE];
	int width, height;
	int weight;
};

Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei, int weight) {
	Window* win=(Window*)calloc(1, sizeof(Window));
	if(!win) return NULL;

	if(!strcpy(win->title, title)) {
		free(win);
		return NULL;
	}
	if(num_elems>0) {
		for(int i=0; i<num_elems; i++) {
			win->Win_elem[i]=(Welem*)calloc(1, sizeof(Welem));
			if(!win->Win_elem[i]) {
				for(int j=0; j<i; j++) {
					Welem_free(win->Win_elem[j]);
				}
				free(win->title);
				free(win);
				return NULL;
			}
			win->Win_elem[i]=Welem_copy(Win_elem[i]);
			if(!win->Win_elem[i]) {	
				for(int j=0; j<i; j++) {
					Welem_free(win->Win_elem[j]);
				}
				free(win->title);
				free(win);
				return NULL;
			}
		}
		win->num_elems=num_elems;
	}
	win->width=wid;
	win->height=hei;
	win->weight=weight;
	
	return win;
}

void win_free(Window* win) {
	if(!win) return;
	
	free(win->title);
	free(win->selected_elem);
	for(int i=0; i<num_elems; i++) {
		Welem_free(win->Win_elem[i]);
	}
	free(win);
}

Window* win_redraw(Window* win, int width, int height, int weight) {
	if(!win) return NULL;
	
	Window* win2=win_copy(win);
	if(!win2) return NULL;

	win_free(win);
	if(!win_render(win2)) return NULL;
	return win2;
}

Window* win_setSelected(Window* win, int* selected_elem) {
	if(!win || !selected_elem) return NULL;
	for(int i=0; i<MAX_SELECTABLE; ++i) {
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
