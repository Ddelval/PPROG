//  ppro
//	TestCanvas.c
//  Created by David del Val on 04/10/2019
//
//

#include <stdio.h>
#include "Font.h"
#include "Canvas.h"
#include "Pixel.h"

int main(int argc, const char * argv[]) {
    Canvas* can=canv_backGrnd(0, 255, 255, 255, 1, 1);
    Canvas* c=canv_backGrnd(255, 0, 255, 255, 1, 1);
    canv_addOverlay(c, can, 0, 0);
    canv_free(can);
    canv_free(c);

    // Pixel* p =pix_ini(0,0, 255, 10);
    // Pixel* p2=pix_ini(0,255, 0, 10);
    // Pixel* p3=pix_overlay(p, p2);
    //
    // pix_free(p);
    // pix_free(p2);
    // pix_free(p3);
    return 0;
}
