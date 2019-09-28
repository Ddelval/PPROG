//  Test
//	main.c
//  Created by David del Val on 27/09/2019
//
//

#include <stdio.h>
#include "Canvas.h"
int main(int argc, const char * argv[]) {
    FILE* f = fopen("/Users/delvaldavid/Desktop/out.sh", "r");
    Canvas* c=canv_load(f);
    canv_print(stdout, c, 0, 0);
    canv_free(c);
    return 0;
}
