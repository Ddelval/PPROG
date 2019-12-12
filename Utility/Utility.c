//  Test
//	Utility.c
//  Created by David del Val on 27/09/2019
//
//

#include "Utility.h"

void append(char* dest, int* spos, const char* orig){
    if(!dest||!spos||!orig){
        fprintf(stderr,"NULL pointer in append function\n");
        return;
    }
    while(*orig){
        dest[*spos]=*orig;
        orig++;
        *spos=*spos+1;
    };
    dest[*spos]=0;
}
void appendf(char* dest, int* spos, char* orig){
    assert(dest&&spos&&orig);
    if(!dest||!spos||!orig){
        fprintf(stderr,"NULL pointer in append function\n");
        return;
    }
    char* o=orig;
    while(*orig){
        dest[*spos]=*orig;
        orig++;
        *spos=*spos+1;
    };
    dest[*spos]=0;
    free(o);
}

char* movecur(int x,int y){
    char* c=(char*)malloc(sizeof(char)*16);
    sprintf(c,"%c[%d;%dH ",27,x,max(y,0));
    return c;
}

int max(int a,int b){
    return a>b? a:b;
}
int min(int a,int b){
    return a<b? a:b;
}
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
    fflush(stdin);
    //if(read(0, &buf, 1) < 0)
        //perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    tcsetattr(0, TCSADRAIN, &old);
    //if(tcsetattr(0, TCSADRAIN, &old) < 0)
        //perror("tcsetattr ~ICANON");
    return buf;
 }
/*
void handle_winch(Display* dis) {
    signal(SIGWINCH, SIG_IGN);

    endwin();
    initscr();
    refresh();
    clear();
    
    if(!disp_fulldraw(dis, LINES, COLS)) err=WINCH_ERR;
    
    signal(SIGWINCH, handle_winch);
}*/
