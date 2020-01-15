

#include "Skill.h"




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
Skill* skill_ini(){
  Skill* skil;

  skil=(Skill*)calloc(1,sizeof(Skill));
  if(!skil) return NULL;

  return skil;
}

Skill* skill_load(FILE* f) {
  if(!f) return NULL;

  Skill* sk=skill_ini();
  if(!sk) return NULL;
  fscanf(f,"%d",&sk->id);
  sk->name[0]='\n';
  while(sk->name[0]=='\n')fgets(sk->name,20,f);
  sk->name[strlen(sk->name)-1]=0;
  sk->desc[0]='\n';
  while(sk->desc[0]=='\n')fgets(sk->desc,200,f);
  sk->desc[strlen(sk->desc)-1]=0;

  fscanf(f,"%d",&sk->special_skill);
  sk->atbself=attb_load(f);
  sk->atbatk=attb_load(f);
  return sk;
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
Skill* skill_readFromFile(char *file, int id){
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
    if(atoi(buf) == id) {
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

int skill_getId(Skill *s) {
  return s->id;
}

char* skill_getName(Skill *s) {
  if(!s) return NULL;
  return strdup(s->name);
}

char * skill_getDesc(Skill *s){
  if(!s) return NULL;
  return strdup(s->desc);
}

Attributes* skill_getAtbself(Skill *s){
  return attb_copy(s->atbself);
}

Attributes* skill_getAtbatk(Skill *s){
  return attb_copy(s->atbatk);
}

void skill_free(Skill * s){
  if(!s)return;
  attb_free(s->atbatk);
  attb_free(s->atbself);
  free(s);
}

Skill* skill_copy(Skill* sk) {
  if(!sk) return NULL;
  Skill* sc=skill_ini();
  sc->id=sk->id;
  strcpy(sc->name,sk->name);
  strcpy(sc->desc,sk->desc);

  sc->atbself=attb_copy(sk->atbself);
  sc->atbatk=attb_copy(sk->atbatk);
  sc->special_skill=sk->special_skill;
  return sc;
}
