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
typedef enum{false,true}bool;

void appendf(char* dest, int* spos,char* orig);
void append (char* dest, int* spos, const char* orig);
char* movecur(int x,int y);
#endif /* Utility_h */
