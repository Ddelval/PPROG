//  Test
//	Utility.h
//  Created by David del Val on 27/09/2019
//
//

#ifndef Utility_h
#define Utility_h
#define siz(x) sizeof(x)/sizeof((x[0]))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>

#define DISP_HEIGHT 471
#define DISP_WIDTH 1209
/*
#include <signal.h>
#include <ncurses.h>
#define WINCH_ERR -10
*/
typedef enum{false,true}bool;
//extern int err=0;

void appendf(char* dest, int* spos,char* orig);
void append (char* dest, int* spos, const char* orig);
char* movecur(int x,int y);
int max(int a,int b);
int min(int a,int b);

char getch1(void);

//void handle_winch(Display* dis);
#endif /* Utility_h */
