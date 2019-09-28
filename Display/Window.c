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
	free(Win_elem);
	free(win);
}

Window* win_redraw(Window* win, int width, int height, int weight) {
	if(!win) return NULL;
	
	Window* win2=win_copy(win);
	if(!win2) return NULL;
	win2->width=width;
	win2->height=height;
	win2->weight=weight;
	win_free(win);
	if(!win_render(win2, 0)) return NULL;
	return win2;
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
	if(!win_render(win, win->scroll_pos+1) return NULL;
	win->scroll_pos++;
	return win;
}

Window* win_scrollUp(Window* win) {
	if(!win) return NULL;
	if(!win_render(win, win->scroll_pos-1) return NULL;
	win->scroll_pos--;
	return win;
}

Window* win_copy(Window* win) {
	if(!win) return NULL;
	Window* win2=(Window*)calloc(1, sizeof(Window);
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
	if(num_elems<0) return win2;
	Welem** we=(Welem**)calloc(num_elems, sizeof(Welem*));
	if(!we) win_free(win2); 
	for(int i=0; i<num_elems; i++) {
		we[i]=Welem_copy(win->Win_elem[i]);
		if(!we[i]) {
			for(int j=0; j<i; ++j) {
				Welem_free(we[j]);
			}
			free(we);
		}	
	}
	win2->Win_elem=we;
}
