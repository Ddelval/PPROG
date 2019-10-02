//  ppro
//	TestLabel.c
//  Created by David del Val on 01/10/2019
//
//

#include "Utility.h"
#include "Wlabel.h"
int main(){
	FILE* f = fopen("/Users/daviddelval/Documents/GitHub/Mine/PPStuff/ScapArea/Display/Fonts/CD_Robo_Mono_11.txt", "r");
	Font *ff=font_load(f);
	Wlabel* w=wl_ini("Hello world", ff,0);
	Canvas* r=wl_render(w, 400);
	canv_print(stdout, r, 0, 0);
	return 0;
}
