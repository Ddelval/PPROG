#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
int main(int argc, const char* argv[]) {
	FILE* fi=fopen("Display/Fonts/Robo_Mono/04.txt", "r");
	Font* f=font_load(fi);
	Window* win = win_ini("Inventory", NULL, 0, 300, 250, 909, 0, f);
	if(!win) {
		fprintf(stderr, "NULL WINDOW");
		return 1;
	}
	errno = 0;

	Wlabel* w=wl_ini("5x data",f,10);
	Welem* wel=we_ini(LABEL,w);
	win_addWindowElement(win,wel);
	if(!win_setMargins(win, 0, 0, 0, 0)) printf("aaa");
	Canvas* r=win_render(win);
	canv_print(stdout, r, 0,0);
	if(!r) {
		fprintf(stderr, "BAD RENDER");
		win_free(win);
		return 2;
	}
	font_free(f);
	fclose(fi);
	wl_free(w);
	we_free(wel);
	win_free(win);
	canv_free(r);
	return 0;
}
