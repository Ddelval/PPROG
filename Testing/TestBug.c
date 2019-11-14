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

int main(int argc, const char * argv[]) {
  FILE* f = fopen("Display/Fonts/Robo_Mono/04.txt", "r");
  Font *ff=font_load(f);
  Wlabic* w=wi_ini("This is an example test", ff, 4, 20, TEXT_RIGHT);
  fclose(f);
  f = fopen("Icons/food.txt", "r");
  Canvas* c=canv_load(f);
  wi_setCanvas(w, c);
  Canvas* a=wi_render(w, 1000);
  canv_print(stdout, a, 0,0);

  wi_setBackColor(w, 255,255,0,255);
  Canvas* b=wi_render(w, 1000);
  canv_print(stdout, b, 50,10);

  canv_free(c);
  canv_free(b);
  canv_free(a);
  wi_free(w);
  font_free(ff);
  fclose(f);
  return 0;
}
