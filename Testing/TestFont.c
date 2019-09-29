//  Test
//	main.c
//  Created by David del Val on 27/09/2019
//
//

#include <stdio.h>
#include "Font.h"
#include "Canvas.h"
int main(int argc, const char * argv[]) {
    FILE* f = fopen("Display/Fonts/CD_Robo_Mono_11.txt", "r");
    Font *ff=font_load(f);


    Canvas* c=font_renderText(ff, "Hello 123");
    canv_print(stdout, c, 10, 10);
    canv_free(c);
    fclose(f);
    font_free(ff);
    return 0;
}
