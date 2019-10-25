#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include "Room.h"

int main(int argc, const char* argv[]) {
  Canvas* c=canv_backGrnd(0,255,0,255,700,470);
  FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
  Font* fu;
  Font* fo=font_load(f);
  fclose(f);
  f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
  Welem* we=we_createLabel("Resources", f, 10);
 // atom://teletype/portal/17a338be-4b30-4988-a436-2bd408be4641
}
