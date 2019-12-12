#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Crafting.h"

int main(){
  Recipe *r = rec_ini();
  if(!r){
    printf("!r\n");
    exit(0);
  }

  FILE * f = fopen("craft_recipes.txt", "r");
  r = rec_load(f);
  if(!r){
    printf("!r 2\n");
    exit(0);
  }

  rec_free(r);
  return 1;
}
