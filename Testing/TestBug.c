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
  Canvas* c= font_renderText(ff, "Hello");
  canv_print(stdout, c, 0, 0);
  canv_free(c);
  font_free(ff);
  fclose(f);
  return 0;
}
