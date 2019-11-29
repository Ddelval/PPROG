#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "atb.h"
#include "types.h"
#include "skill.h"
extern int errno;



struct _skill {
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
    skil->special = 0;
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

skill * skill_getSpecial(skill *s){
  char name[30];
  if(!s) return NULL;
  strcpy(name, s->special_skill);
  return name;
}
