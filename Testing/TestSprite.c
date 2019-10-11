//  ppro
//	TestSprite.c
//  Created by David del Val on 07/10/2019
//
//

#include "Utility.h"
#include "Sprite.h"
#include "Room.h"
int main(){
    FILE* f = fopen("Sprites/Dat.txt", "r");
    Sprite* s=spr_load(f);
    spr_setCoodrinates(s, 10, 10);
    Pixel* pi=pix_ini(100, 150, 0, 255);
    Room* r=room_ini(1, "Hello", 200, 200, pi);
    room_addBSprite(r, s);
    spr_setCoodrinates(s, 20, 80);
    room_addBSprite(r, s);
    canv_print(stdout, room_getRender(r),0,0);
    room_free(r);
    fclose(f);
    pix_free(pi);
    spr_free(s);
    return 0;
}
