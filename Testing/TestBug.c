//  ppro
//	TestCanvas.c
//  Created by David del Val on 04/10/2019
//
//

#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Font.h"
#include "Canvas.h"
#include "Pixel.h"
#include <unistd.h>
#include <termios.h>

char getch1(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    tcgetattr(0, &old);
    //if(tcgetattr(0, &old) < 0)
        //perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &old);
   // if(tcsetattr(0, TCSANOW, &old) < 0)
        //perror("tcsetattr ICANON");
    read(0, &buf, 1);
    //if(read(0, &buf, 1) < 0)
        //perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    tcsetattr(0, TCSADRAIN, &old);
    //if(tcsetattr(0, TCSADRAIN, &old) < 0)
        //perror("tcsetattr ~ICANON");
    return buf;
 }

int main(int argc, const char * argv[]) {
  FILE* fi=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
	Font* f=font_load(fi);
	Window* win = win_ini("Inventory", NULL, 0, 300, 250, 1, 909, 0, f);
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
