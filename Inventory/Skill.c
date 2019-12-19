#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Skill.h"
extern int errno;



struct _skill {
        int id;
        char name[20];
        char desc[200];
        Attributes * atbself;
        Attributes * atbatk;
        int special_skill;
};



/*
   Function name: skill_ini
   Utility: It creates a new skill structure with NULL in each camp
   Inputs:
   Outputs: skill pointer
 */
Skill * skill_ini(){
        Skill * skil = NULL;

        skil = (Skill *) calloc(1,sizeof(Skill));
        if (skil == NULL) {
                //printf("Error: calloc.\n");
                return NULL;
        }
        skil->atbself = attb_ini();
        skil->atbatk = attb_ini();
        return skil;
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
Skill * skill_readFromFile(char *file, int id){
        if(!file) return NULL;
        FILE *f = fopen(file, "r");
        if(!f) return NULL;
        Skill *s = NULL;

        s = skill_ini();
        if(!s) return NULL;

        char *q = NULL;
        char buf[1000];

        do {
                do {
                        q  = fgets(buf, 1000, f);
                } while(strlen(buf) < 1 && q);
                if (atoi(buf) == id) {
                        s->id = id;
                        fgets(s->name, 1000, f);
                        fgets(s->desc, 1000, f);
                        fscanf(f, "%d", &s->special_skill);
                        s->atbself = attb_load(f);
                        s->atbatk = attb_load(f);
                        return s;
                }
                q = fgets(buf, 1000, f);
                q = fgets(buf, 1000, f);
                q = fgets(buf, 1000, f);
                q = fgets(buf, 1000, f);
                q = fgets(buf, 1000, f);
        }  while(q);
        return NULL;
}




int skill_getSpecial(Skill *s){
        if(!s) return -1;
        return s->special_skill;
}

int skill_getId(Skill *s){
        return s->id;
}

char * skill_getName(Skill *s){
        if(!s) return NULL;
        return s->name;
}

char * skill_getDesc(Skill *s){
        if(!s) return NULL;
        return s->desc;
}

Attributes * skill_getAtbself(Skill *s){
        return s->atbself;
}

Attributes * skill_getAtbatk(Skill *s){
        return s->atbatk;
}

void skill_free(Skill * s){
    attb_free(s->atbatk);
    attb_free(s->atbself);
    free(s);
}
