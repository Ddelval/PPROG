#include <stdio.h>
#include <errno.h>
#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include "SpriteDic.h"

int main(){
    
    canv_print(stdout, spr_getDispData(sdic_lookup(1)), 0, 0);
    
    
    
    return 0;
}
