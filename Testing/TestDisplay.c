#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include "Room.h"

int main(int argc, const char* argv[]) {
  int vdiv=700;
  int w=1200;
  int h=470;
  Canvas* c=canv_backGrnd(0,255,0,255,vdiv,h);
  FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
  Font* f6=font_load(f);
  fclose(f);
  f=fopen("Display/Fonts/Robo_Mono/04.txt", "r");
  Font* f4=font_load(f);
  fclose(f);
  Welem* wel[3];
  wel[0]=we_createLabel("6 x Food", f4, 10);
  wel[1]=we_createLabel("5 x Water", f4, 10);
  wel[2]=we_createLabel("5 x Stone", f4, 10);
  Wlabel* tit=wl_ini("First Room",f6,10);
  Canvas* right=wl_render(tit,w-vdiv);
  int a=(h-canv_getHeight(right))/2;
  Window* wres=win_ini("Resources",wel,3,w-vdiv,a,vdiv,canv_getHeight(right),f6);
  Canvas* tmp=win_render(wres);
  canv_appendVI(right,tmp);

  canv_free(tmp);


  Welem* wela[3];
  wela[0]=we_createLabel("Title", f6, 10);
  wela[1]=we_createLabel("Build", f6, 10);
  wela[2]=we_createLabel("Attack", f6, 10);
  Window* wact=win_ini("Actions", wela,3,w-vdiv,a,vdiv, canv_getHeight(right), f6);
  tmp=win_render(wact);
  canv_appendVI(right,tmp);
  //canv_print(stdout,c,0,0);

  canv_free(tmp);
  Canvas* s=canv_appendH(c,right);

  canv_print(stdout,s,0,0);
  if(!s) printf("BAD RENDER\n");

  font_free(f6);
  font_free(f4);
  for(int i=0; i<3; i++) {
    we_free(wel[i]);
  }
  for(int i=0; i<3; i++) {
    we_free(wela[i]);
  }
  wl_free(tit);
  canv_free(right);
  win_free(wres);
  win_free(wact);
  canv_free(c);
  canv_free(s);
  }
