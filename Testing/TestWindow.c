#include <stdio.h>
#include <errno.h>
#include "Window.h"

int main(int argc, const char* argv[]) {
	Window* win = win_ini("Hello", NULL, 0, 200, 150, 1, 400, 0);
	if(!win) {
		fprintf(stderr, "NULL WINDOW");
		return 1;
	}
	errno = 0;
	if(!win_setMargins(win, 20, 10, 20, 20)) printf("aaa");
	if(!win_render(win, 0)) {
		fprintf(stderr, "BAD RENDER");
		return 2;
	}
	win_free(win);
	return 0;
}
