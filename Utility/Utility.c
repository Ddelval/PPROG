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
    sprintf(c,"%c[%d;%dH ",27,x,y);
    return c;
}

int max(int a,int b){
    return a>b? a:b;
}

void handle_winch(Display* dis) {
    signal(SIGWINCH, SIG_IGN);

    endwin();
    initscr();
    refresh();
    clear();
    
    if(!disp_fulldraw(dis, LINES, COLS)) err=WINCH_ERR;
    
    signal(SIGWINCH, handle_winch);
}
