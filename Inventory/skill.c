#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "skill.h"
extern int errno;



struct _skill {
    int id;
    char name[20];
    char desc[200];
    atb * atbself;
    atb * atbatk;
    char *special_skill[30];
};



/*
 Function name: skill_ini
 Utility: It creates a new skill structure with NULL in each camp
 Inputs:
 Outputs: skill pointer
 */
skill * skill_ini(){
    skill * skil = NULL;

    skil = (skill *) calloc(1,sizeof(skill));
     if (skil == NULL){
        printf("Error: calloc.\n");
        return NULL;
    }
    skil->atbself = atb_ini();
    skil->atbatk = atb_ini();
}



/*
format of the file:
            id(int)
            name(char*)
            description(char*)
            special_skill(char*)
            atb * atbself;
            atb * atbatk;
*/
skill * skill_readFromFile(char *file, int id){
  if(!file) return NULL;
  FILE *f = fopen(file, "r");
  if(!f) return NULL;
  skill *skill = NULL;

  skill = skill_ini();
  if(!skill) return NULL;

  int a, b, c, d, e;
  char *q = NULL;
  char buf[1000];

  do {
    do {
      q  = fgets(buf, 1000, f);
    } while(strlen(buf) < 1 && q);
    if (atoi(buf) == id) {
      skill->id = id;
      fgets(skill->name, 1000, f);
      fgets(skill->desc, 1000, f);
      fgets(skill->special_skill, 1000, f);
      fscanf(f,"%d %d %d %d %d", &a, &b, &c, &d, &e);
      atb_setter(skill->atbself, a, 1);
      atb_setter(skill->atbself, b, 2);
      atb_setter(skill->atbself, c, 3);
      atb_setter(skill->atbself, d, 4);
      atb_setter(skill->atbself, e, 5);
      fscanf(f,"%d %d %d %d %d", &a, &b, &c, &d, &e);
      atb_setter(skill->atbatk, a, 1);
      atb_setter(skill->atbatk, b, 2);
      atb_setter(skill->atbatk, c, 3);
      atb_setter(skill->atbatk, d, 4);
      atb_setter(skill->atbatk, e, 5);
      return skill;
    }
    q = fgets(buf, 1000, f);
    q = fgets(buf, 1000, f);
    q = fgets(buf, 1000, f);
    q = fgets(buf, 1000, f);
    q = fgets(buf, 1000, f);
  }  while(q);
  return NULL;
}





/*
Function name: skill_destroy
 Utility: Destroys a skill struct
 Inputs: skill to destroy (pointer)
 Outputs: skill pointer
 */
void skill_destroy(skill * skill){

    atb_destroy(skill->atbatk);
    atb_destroy(skill->atbself);
    free(skill);
}


/*
Function name: skill_load
 Utility: It takes a file with skill camps and inserts them into an object struct
 Inputs: skill pointer, file pointer, name of the file.
 Outputs: skill pointer
 */
 /*
skill * skill_load(skill* skill, char * name){
    FILE * f;
    int a,b,c,d,e;
    skill = skill_ini();
    f = fopen(name ,"r");


    fscanf(f,"%s %s %d %d %d %d %d",skill->name, skill->desc, &a,&b,&c,&d,&e);
   atb_setter(skill->atb,a,1);
   atb_setter(skill->atb,b,2);
   atb_setter(skill->atb,c,3);
   atb_setter(skill->atb,d,4);
   atb_setter(skill->atb,e,5);
    fclose(f);
}
*/

char * skill_getSpecial(skill *s){
  char name[30];
  if(!s) return NULL;
  strcpy(name, s->special_skill);
  return name;
}

int skill_getId(skill *s){
  return s->id;
}

char * skill_getName(skill *s){
  char name[300];
  strcpy(name, s->name);
  return name;
}

char * skill_getDesc(skill *s){
  char name[300];
  strcpy(name, s->desc);
  return name;
}

atb * skill_getAtbself(skill *s){
  return s->atbself;
}

atb * skill_getAtbatk(skill *s){
  return s->atbatk;
}
