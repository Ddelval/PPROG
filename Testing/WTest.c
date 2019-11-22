#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include <unistd.h>
#include <termios.h>
#include "Room.h"
char getch1(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    tcgetattr(0, &old);
    //if(tcgetattr(0, &old) < 0)
        //perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &old);
   // if(tcsetattr(0, TCSANOW, &old) < 0)
        //perror("tcsetattr ICANON");
    read(0, &buf, 1);
    //if(read(0, &buf, 1) < 0)
        //perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    tcsetattr(0, TCSADRAIN, &old);
    //if(tcsetattr(0, TCSADRAIN, &old) < 0)
        //perror("tcsetattr ~ICANON");
    return buf;
 }

int main(){
    int vdiv=800;
    int w=1200;
    int h=470;
    FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
    Font* f6=font_load(f);
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
    spr_setOJ(sp,30);
    fclose(f);
    room_addOSprite(r, sp);
    //canv_print(stdout, spr_getDispData(sp), 10, 10);
    char* c="Room 1";
    Display* dis= disp_ini(w, h, r, vdiv, c, f6);

    f=fopen("Icons/food.txt", "r");
    Canvas* canvv=canv_load(f);
    fclose(f);

    Welem* wel[3];
  //  wel[0]=we_createLabel("6 x Food", f4, 10);
    wel[0]=we_createLabic("5", f6, 10, 10, canvv, TEXT_LEFT);
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

    canv_print(stdout, disp_Render(dis), 0, 0);

    while(1){
        char c;
        c=getch1();
        //scanf("%c",&c);
        //printf("%c\n",c);
        if(c=='W'){
            room_incPos(r, 0, -10, 0);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
        }
        if(c=='S'){
            room_incPos(r, 0, 10, 0);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
        }
        if(c=='A'){
            room_incPos(r, 0, 0, -10);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
        }
        if(c=='D'){
            room_incPos(r, 0, 0, 10);
            room_printMod(r, 0, 0, 0, 0, vdiv, h);
        }
        if(c=='O'){
            disp_incSelIndex(dis,1,-1);
        }
        if(c=='L'){
            disp_incSelIndex(dis,1,1);
        }
        usleep(100000);

    }
    return 0;
}
