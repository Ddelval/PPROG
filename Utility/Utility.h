//  Test
//	Utility.h
//  Created by David del Val on 27/09/2019
//
//

#ifndef Utility_h
#define Utility_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum{false,true}bool;

void appendf(char* dest, int* spos,char* orig);
void append (char* dest, int* spos, const char* orig);
char* movecur(int x,int y);
#endif /* Utility_h */
