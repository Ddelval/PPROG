#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include <unistd.h>
#include <termios.h>
#include "Room.h"

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

    f = fopen("Worlds/s1.txt", "r");

    Room* r=room_load(f);

    fclose(f);

    f = fopen("Sprites/per.txt", "r");

    Sprite* sp= spr_load(f);
    spr_setOI(sp, 20);
    spr_setOJ(sp,600);
    fclose(f);
    room_addOSprite(r, sp,0);
    //canv_print(stdout, spr_getDispData(sp), 10, 10);
    char* c="Room 1";
    Display* dis= disp_ini(w, h, r, vdiv, c, f8);

    Sprite* ic=sdic_lookup(101);
    Canvas* canvv=spr_getDispData(ic);

    Welem* wel[3];
  //  wel[0]=we_createLabel("6 x Food", f4, 10);
    wel[0]=we_createLabic("  5", f6, 10, 10, canvv, TEXT_EAST);
    ic=sdic_lookup(102);
    canvv=spr_getDispData(ic);
    wel[1]=we_createLabic(" 10", f6, 10, 10, canvv, TEXT_EAST);
    ic=sdic_lookup(102);
    canvv=spr_getDispData(ic);
    wel[2]=we_createLabic(" 15", f6, 10, 10, canvv, TEXT_EAST);

    Welem* wela[3];
    wela[0]=we_createLabel("Title", f6, 10);
    wela[1]=we_createLabel("Build", f6, 10);
    wela[2]=we_createLabel("Attack", f6, 10);



    Window* w1=win_ini("Resources", wel, 3, w-vdiv-1, h/2-20, 0, 0, f8);
    Window* w2=win_ini("Actions", wela, 3, w-vdiv-1, h/2-20, 0, 0, f8);
    disp_AddLWindow(dis, w1);
    disp_AddLWindow(dis, w2);
    canv_print(stdout, disp_Render(dis), 0, 0);
    //disp_DiaglogWindow(dis,"Hello, my name is Pepe. I am going to be your annoying friend in this game. If at some point I get too anoying, you can always press ALT + F4 and I will stop.",f4);
    while(1){
        char c;
        c=getch1();
        //scanf("%c",&c);
        //printf("%c\n",c);
        if(c=='W'){
            if(room_incPos(r, 0, -10, 0)==1){
                //disp_scroll(dis,-0.5,0);
                if(disp_scroll(dis,-0.5,0)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
            }
            else{
                room_printMod(r, 0, 0, 0);
            }
        }
        if(c=='S'){
            if(room_incPos(r, 0, 10, 0)==3){
                if(disp_scroll(dis,0.5,0)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
                
            }
            else{
                room_printMod(r, 0, 0, 0);
            }
        }
        if(c=='A'){
            if(room_incPos(r, 0, 0, -10)==4){
                //disp_scroll(dis,0,0.5);
                if(disp_scroll(dis,0,-0.5)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
            }
            else{
                room_printMod(r, 0, 0, 0);
            }
        }
        if(c=='D'){
            if(room_incPos(r, 0, 0, 10)==2){
                //disp_scroll(dis,0,0.5);
                if(disp_scroll(dis,0,0.5)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
            }
            else{
                room_printMod(r, 0, 0, 0);
            }
            
        }
        if(c=='O'){
            disp_incSelIndex(dis,1,-1);
        }
        if(c=='L'){
            disp_incSelIndex(dis,1,1);
        }
        //usleep(100000);

    }
    return 0;
}
