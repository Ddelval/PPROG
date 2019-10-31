#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include "SpriteDic.h"

int main(){
    FILE* f = fopen("Worlds/s1.txt", "r");
    Room* r=room_load(f);
    canv_print(stdout,room_getRender(r),0,0);
}
