#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Recipe.h"

int main(){
  Recipe *r ;
  int size ;
  int *q, *e, i = 0;

  FILE * f = fopen("Dictionaries/craft_recipies.txt", "r");
  if(!f){
    printf("!r 2\n");
    exit(0);
  }

while(i < 4){
  r = rec_ini();
  if(!r){
    printf("!r\n");
    exit(0);
  }

  r = rec_load(f);
  if(!r){
    printf("!r 3\n");
    exit(0);
  }


  size = rec_getSize(r);
  printf("Result Id: %d, Size: %d, Name: %s\n", rec_getResult_id(r), size, rec_getName(r));
  q = rec_getQuantities(r);
  e = rec_getElements(r);
  for(int i= 0; i<size; i++){
    printf("Object id: %d, Quantiy: %d\n", q[i], e[i]);
  }
  q = NULL;
  e = NULL;

  rec_free(r);
  i++;
}

  fclose(f);


  return 1;
}
