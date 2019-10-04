//  ppro
//	TestCanvas.c
//  Created by David del Val on 04/10/2019
//
//

#include <stdio.h>
#include "Font.h"
#include "Canvas.h"
int main(int argc, const char * argv[]) {
    FILE* f = fopen("/Users/daviddelval/Documents/GitHub/Mine/PPROG/dat.txt", "r");
	Canvas* can=canv_load(f);
	Canvas * c=canv_backGrnd(255, 255, 255, 255, canv_getWidth(can), canv_getHeight(can));
	canv_addOverlay(c, can, 0, 0);
	canv_print(stdout, c, 0, 0);
    return 0;
}
