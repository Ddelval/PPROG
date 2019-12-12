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



int main(int argc, const char * argv[]) {
  FILE* fi=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
	Font* f=font_load(fi);
	Window* win = win_ini("Inventory", NULL, 0, 300, 250, 909, 0, f);
  Pixel* pt = pix_ini(200,70,16,255);
  if(!win_setBackColor(win, pt)) return -1;
  pix_free(pt);
	if(!win) {
		fprintf(stderr, "NULL WINDOW");
		return 1;
	}
	errno = 0;

  FILE* ff = fopen("Icons/food.txt", "r");
  Canvas* c=canv_load(ff);
	Welem* we1=we_createLabel("5x data", f, 10);
	win_addWindowElement(win,we1);
  Welem* we2=we_createLabel("5x data", f, 10);
  win_addWindowElement(win,we2);
  Welem* we3=we_createLabel("5x data", f, 10);
  win_addWindowElement(win,we3);
  Welem* we4=we_createLabic("This is an example test", f, 4, 20, c, TEXT_EAST);
  win_addWindowElement(win,we4);

  win_setSelected(win, 0);
	if(!win_setMargins(win, 0, 0, 0, 0)) printf("aaa");
	Canvas* r=win_render(win);
	canv_print(stdout, r, 0,0);
	if(!r) {
		fprintf(stderr, "BAD RENDER");
		win_free(win);
		return 2;
	}

  int i=0;
  while(1){
    char c;
    c=getch1();
    //scanf("%c",&c);
    //printf("%c\n",c);
    if(c=='W'){
        i--;
    }
    if(c=='S'){
        i++;
    }

    win_setSelected(win, abs(i%4));
    canv_free(r);
    r=win_render(win);
    canv_print(stdout, r, 0,0);
    usleep(100000);
  }
	font_free(f);
	fclose(fi);
	we_free(we1);
  we_free(we2);
  we_free(we3);
  we_free(we4);
	win_free(win);
	canv_free(r);
	return 0;
}
