#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "atb.h"
#include "types.h"
#include "skill.h"

int main{
  skill *s = NULL;
  char file[30] = "skills";
  char name[30], desc[1000], special[30];
  atb *a = NULL, *b = NULL;

  skill = skill_readFromFile(file, 6);
  if(skill == NULL){
    printf("skill is NULL\n");
  }

  name  = skill_getName(skill *s);
  desc = skill_getDesc(skill *s);
  special =  skill_getSpecial(skill *s);

  fprintf(stdout, "%s\n%s\n%s\n", name, desc, special);

  return 0;
}
