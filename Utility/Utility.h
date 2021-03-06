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
#define TIER_FINISHED 8


typedef enum{false,true}bool;
typedef struct {
    int fi,se;
}pairii;

/*
      Appends to the destination string the origin one, saving the total length
      in spos.
*/
void appendf(char* dest, int* spos,char* orig);
void append (char* dest, int* spos, const char* orig);
/*
      Writes in a char the position of the cursor.
*/
char* movecur(int x,int y);
int max(int a,int b);
int min(int a,int b);
/*
      Reads an input written on  stdin.
*/
char getch1(void);
/*
      Reads an input written on stdin till a char Q, E or J is read.
*/
char waitforchar(void);
void term_restore();
void term_init();
/*
      Writes on the given file a string.
*/
int sendToScreen(FILE* f, char* ch);

/*
      Used to check that the construction of a building has been succesful.
*/
bool checkBuilding(pairii p);
/*
      Used to construct a building
*/
void addBuilding(pairii p);
#endif /* Utility_h */
