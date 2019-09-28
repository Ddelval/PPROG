//  Test
//	main.c
//  Created by David del Val on 27/09/2019
//
//

#include <stdio.h>
#include "Font.h"
#include "Canvas.h"
int main(int argc, const char * argv[]) {
    FILE* f = fopen("/Users/delvaldavid/Desktop/data", "r");
    Canvas* c=canv_load(f);
    Canvas* c2=canv_copy(c);
    Canvas* ad=canv_appendV(c, c2);
    canv_print(stdout, ad, 10, 10);
    return 0;
}
