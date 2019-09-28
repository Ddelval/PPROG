/*	Window.c	*/

#include "Window.h"

struct _Window {
	char* title;
	Welem** Win_elem;
	int num_elems;
	int selected_elem[MAX_SELECTABLE];
	int width, height;
};

Window* win_ini(char* title, Welem** Win_elem, int num_elems, int wid, int hei) {
	Window* win=(Window*)calloc(1, sizeof(Window));
	if(!win) return NULL;

	if(!strcpy(win->title, title)) {
		free(win);
		return NULL;
	}
	if(num_elems>0) {
		for(int i=0; i<num_elems; i++) {
			win->Win_elem[i]=(Welem*)calloc(1, sizeof(Welem*));
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
