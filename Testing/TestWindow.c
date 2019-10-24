#include <stdio.h>
#include <errno.h>
#include "Window.h"

int main(int argc, const char* argv[]) {
	Window* win = win_ini("Inventory", NULL, 0, 300, 250, 1, 909, 0);
	if(!win) {
		fprintf(stderr, "NULL WINDOW");
		return 1;
	}
	errno = 0;
	if(!win_setMargins(win, 0, 0, 0, 0)) printf("aaa");
	if(!win_render(win)) {
		fprintf(stderr, "BAD RENDER");
		win_free(win);
		return 2;
	}
	win_free(win);
	return 0;
}
