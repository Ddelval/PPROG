//  Test
//	main.c
//  Created by David del Val on 27/09/2019
//
//

#include <stdio.h>
#include "Font.h"
#include "Canvas.h"
int main(int argc, const char * argv[]) {
    FILE* f = fopen("Display/Fonts/Robo_Mono/04.txt", "r");
    Font *ff=font_load(f);
    Canvas* c=font_renderText(ff, "Hello, world");
    Canvas* back=canv_backGrnd(0, 0, 255, 255, canv_getWidth(c), canv_getHeight(c));
    canv_addOverlay(back, c, 0, 0);
    canv_print(stdout, back, 10, 10);
    fclose(f);
    font_free(ff);
    canv_free(c);
    canv_free(back);

    return 0;
}
