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

int main(int argc, const char * argv[]) {
  FILE* f = fopen("Display/Fonts/Robo_Mono/04.txt", "r");
  Font *ff=font_load(f);
  Wlabic* w=wi_ini("This is an example test", ff, 4, 20, TEXT_NORTH);
  Wlabic* w2=wi_ini("This is an example test", ff, 4, 20, TEXT_SOUTH);
  fclose(f);
  f = fopen("Icons/food.txt", "r");
  Canvas* c=canv_load(f);
  wi_setCanvas(w, c);
  wi_setCanvas(w2, c);
  Canvas* a;
  Canvas* b;
  int i=0;
  while(true) {
    if((i/255)%3==0)wi_setBackColor(w, (255-(i%255)),0,i%255,255);  // 255 0 0
    if((i/255)%3==1)wi_setBackColor(w,  0,(i%255),(255-(i%255)),255); // 0 0 255
    if((i/255)%3==2)wi_setBackColor(w,  i%255 ,255-i%255,0,255);            // 0 255 0

    if((i/255)%3==1)wi_setBackColor(w2, (255-(i%255)),0,i%255,255);  // 255 0 0
    if((i/255)%3==2)wi_setBackColor(w2,  0,(i%255),(255-(i%255)),255); // 0 0 255
    if((i/255)%3==0)wi_setBackColor(w2,  i%255 ,255-i%255,0,255);

    a=wi_render(w, 1000);
    canv_print(stdout, a, 50,10);
    b=wi_render(w2, 1000);
    canv_print(stdout, b, 200,10);
    usleep(200);
    canv_free(a);
    canv_free(b);
    i++;
  }


  canv_free(c);
  canv_free(a);
  wi_free(w);
  font_free(ff);
  fclose(f);
  return 0;
}
