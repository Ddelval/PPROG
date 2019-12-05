#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include <unistd.h>
#include <termios.h>
#include "Room.h"
#include "Inventory.h"
#include "ObjectDic.h"
int main(){
    int vdiv=800;
    int w=1200;
    int h=470;
    FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
    Font* f6=font_load(f);
    fclose(f);
    f=fopen("Display/Fonts/Robo_Mono/08.txt", "r");
    Font* f8=font_load(f);
    fclose(f);
    f=fopen("Display/Fonts/Robo_Mono/04.txt", "r");
    Font* f4=font_load(f);
    fclose(f);
    Object* o =odic_lookup(1);
    Inventory *inv=inv_ini();
    inv_insert(inv,o);
    inv_insert(inv,o);
    o =odic_lookup(2);
    inv_insert(inv,o);
    int dim;
    int *dimens;
    char ** text;
    Canvas *** dat=inv_render(inv,&dim,&dimens,&text,f4,f6);
    Wlabel* tit=wl_ini("Inventory",f6,10);
    Canvas* c=wl_render(tit,400);
    for(int i=0;i<dim;++i){
        //Iterate through each type of element
        if(dimens[i]>0){
            Wlabel* w=wl_ini(text[i],f6,10);
            Canvas * c2=wl_render(w,400);
            canv_appendVI(c,c2);
        }
        else continue;
        Canvas * b=dat[i][0];
        for(int j=1;j<dimens[i];++j){
            Canvas*bc=canv_backGrnd(0,0,0,0,10,10);
            canv_appendHI(b,bc);
            canv_appendHI(b,dat[i][j]);
        }
        canv_appendVI(c,b);
    }
    Canvas* b=canv_backGrnd(100,100,100,255,canv_getWidth(c),canv_getHeight(c)+10);
    b=canv_addOverlay(b,c,0,0);
    canv_print(stdout,b,10,10);
    return 0;
}
