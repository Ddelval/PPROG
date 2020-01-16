//  Test
//	Utility.c
//  Created by David del Val on 27/09/2019
//
//

#include "Utility.h"

int tier=0;
char* curr_world=NULL;

void append(char* dest, int* spos, const char* orig){
    if(!dest||!spos||!orig){
        //fprintf(stderr,"NULL pointer in append function\n");
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
        //fprintf(stderr,"NULL pointer in append function\n");
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
    char c=fgetc(stdin);
    if(c>='a'&&c<='z'){
        c-='A'-'a';
    }
    return c;
 }

struct termios initial;
//sem_t mut;
void term_restore(){
    tcsetattr(fileno(stdin),TCSANOW,&initial);
    fprintf(stdout,"%c[?25h",27);
    //sem_destroy(&mut);
}


void term_init(){
    struct termios new;

    tcgetattr(fileno(stdin),&initial);
    new=initial;
    new.c_lflag&=~ICANON;
    new.c_lflag&=~ECHO;
    new.c_cc[VMIN]=1;
    new.c_cc[VTIME]=0;
    //new.c_lflag&=~ISIG;
    
    tcsetattr(fileno(stdin),TCSANOW,&new);
    fprintf(stdout,"%c[?25l",27);

    //sem_init(&mut,0,1);
}


int sendToScreen(FILE* f, char* ch){
    if(!f||!ch)return -1;
    //sem_wait(&mut);
    return fprintf(f,"%s",ch);
    //sem_post(&mut);
}