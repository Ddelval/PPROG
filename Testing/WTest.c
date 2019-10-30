#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include "Room.h"

int main(){
    int vdiv=800;
    int w=1200;
    int h=470;
    FILE* f=fopen("/Users/delvaldavid/Documents/GitHub/Mine/PPStuff/ScapArea/Display/Fonts/Robo_Mono/06.txt", "r");
    Font* f6=font_load(f);
    fclose(f);
    f=fopen("/Users/delvaldavid/Documents/GitHub/Mine/PPStuff/ScapArea/Display/Fonts/Robo_Mono/04.txt", "r");
    Font* f4=font_load(f);
    fclose(f);
    
    f = fopen("/Users/delvaldavid/Documents/GitHub/Mine/PPStuff/ScapArea/Worlds/s1.txt", "r");
    
    Room* r=room_load(f);
    
    fclose(f);
    
    f = fopen("/Users/delvaldavid/Documents/GitHub/Mine/PPStuff/ScapArea/Sprites/per.txt", "r");
       
    Sprite* sp= spr_load(f);
    spr_setOI(sp, 20);
    spr_setOJ(sp,30);
    fclose(f);
    room_addOSprite(r, sp);
    //canv_print(stdout, spr_getDispData(sp), 10, 10);
    char* c="Room 1";
    Display* dis= disp_ini(w, h, r, vdiv, c, f6);
    
    
    
    Welem* wel[3];
    wel[0]=we_createLabel("6 x Food", f4, 10);
    wel[1]=we_createLabel("5 x Water", f4, 10);
    wel[2]=we_createLabel("5 x Stone", f4, 10);
    
    Welem* wela[3];
    wela[0]=we_createLabel("Title", f6, 10);
    wela[1]=we_createLabel("Build", f6, 10);
    wela[2]=we_createLabel("Attack", f6, 10);
    
    
    
    Window* w1=win_ini("Resources", wel, 3, w-vdiv, h/2-20, 1, 0, 0, f6);
    Window* w2=win_ini("Actions", wela, 3, w-vdiv, h/2-20, 1, 0, 0, f6);
    disp_AddLWindow(dis, w1);
    disp_AddLWindow(dis, w2);
    Canvas * buff=disp_Render(dis);
    Canvas * buff2=NULL;
    canv_print(stdout, buff, 0, 0);
    buff2=canv_subCopy(buff, 0, 0, vdiv, h);
    canv_free(buff);
    buff=buff2;
    buff2=NULL;
    while(1){
        char c;
        scanf("%c",&c);
        printf("%c\n",c);
        if(c=='W'){
            room_incPos(r, 0, -10, 0);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
//            buff2=disp_Render(dis);
//            canvas_printDiff(stdout, buff2, buff, 0, 0);
//            canv_free(buff);
//            buff=buff2;
//            buff2=buff;
        }
        if(c=='S'){
            room_incPos(r, 0, 10, 0);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
            //buff2=room_getSubRender(r, 0, 0, vdiv, h);
            //canvas_printDiff(stdout, buff2, buff, 0, 0);
            //canv_print(stdout, buff2, 0, 0);
            //canv_free(buff);
        }
        if(c=='A'){
            room_incPos(r, 0, 0, -10);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
//            buff2=disp_Render(dis);
//            //canvas_printDiff(stdout, buff2, buff, 0, 0);
//
//            canv_free(buff);
//            buff=buff2;
//            buff2=buff;
        }
        if(c=='D'){
            room_incPos(r, 0, 0, 10);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
//            buff2=disp_Render(dis);
//            canvas_printDiff(stdout, buff2, buff, 0, 0);
//            canv_free(buff);
//            buff=buff2;
//            buff2=buff;
        }
        
    }
    return 0;
}
