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
  FILE* fi=fopen("Display/Fonts/Robo_Mono/04.txt", "r");
  Font* f=font_load(fi);
  fclose(fi);
  Welem* we = we_createLabel("a", f, 4);
  Welem* ws = we_copy(we);

  Canvas* c = we_render(we, 40);

  canv_free(c);
  we_free(we);
  we_free(ws);
  font_free(f);
  return 0;
}
