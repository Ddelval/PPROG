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
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>

#define DISP_HEIGHT 400
#define DISP_WIDTH 1200

#define ENTITY_TALK_RAD 20



typedef enum{false,true}bool;
typedef struct {
    int fi,se;
}pairii;

void appendf(char* dest, int* spos,char* orig);
void append (char* dest, int* spos, const char* orig);
char* movecur(int x,int y);
int max(int a,int b);
int min(int a,int b);

char getch1(void);
void term_restore();
void term_init();
int sendToScreen(FILE* f, char* ch);
#endif /* Utility_h */
