/*  Entity.c  */

#include "Entity.h"

#define MAX_NAME_LENGTH 50
#define HORIZONTAL_STEP 30
#define VERTICAL_STEP 10
#define SPRITES 4
#define MAX_QUESTS 30

/*
   name: Entitys's name.
   s: Entity's sprite.
   t: Entity's type, can either be the player, an enemy or an ally (people who the
    player might want to talk to).
   x: Coodinate x of thge entity's position. Will always be a positive integer
    between 0 and n, as the canvas will be a [0, n] x [0, n] square.
   y: Coodinate y of thge entity's position.Will always be a positive integer
    between 0 and n, as the canvas will be a [0, n] x [0, n] square.
   attr: A pointer to the attributes the entity has: money, life...
   inv: A pointer to the entity's inventory.
 */
struct _Entity {
  int id;
  char name[MAX_NAME_LENGTH];
  Sprite* s;
  ent_type t;
  int ipos;
  int jpos;
  Attributes* attr;
  Inventory* inv;
  int room_index;
  Display* dis;
  bool has_dialog;
  int i1,i2,j1,j2;
  DialogMan* dman;
  Quest * adq_quests[MAX_QUESTS];
};


Entity *entity_ini (char *name, ent_type t, int i, int j){
  Entity* e = NULL;
  e = (Entity*)calloc(1,sizeof(Entity));
  if(!e) return NULL;
  if(name && strlen(name)+1 < MAX_NAME_LENGTH) strcpy(e->name, name);

  e->t = t;
  e->ipos = i;
  e->jpos = j;

  e->attr = attb_ini();
  if (!e->attr) {
    entity_free(e);
    return NULL;
  }
  e->inv = inv_ini();
  if(!e->inv) {
    entity_free(e);
    return NULL;
  }
  return e;
}


Entity *entity_load(FILE* f, Display *d){
  Entity *e = NULL;

  e = entity_ini(NULL, 0, 0, 0);
  if(!e) return NULL;
  attb_free(e->attr);
  inv_free(e->inv);

  int spindex;
  fscanf(f,"%d %d %d\n",&(e->id),(ent_type*)&(e->t),&spindex);
  fgets(e->name,MAX_NAME_LENGTH,f);
  if(strlen(e->name)>0)e->name[strlen(e->name)-1]=0;
  if(e->t==ALLY){
      fscanf(f,"%d",(bool*)&e->has_dialog);
  }
  else if(e->t==ENEMY){
    fscanf(f,"%d %d %d %d",&e->i1, &e->i2, &e->j1, &e->j2);
  }
  
  e->ipos=0;
  e->jpos=0;
  e->attr= attb_load(f);
  e->inv=  inv_load(f);

  if(entity_setSprite(e,spindex)==NULL){
    entity_free(e);
    return NULL;
  }
  if(d){
    if(entity_addtoDisplay(e,d)==NULL){
        entity_free(e);
        return NULL;
      }
  }


  return e;
}

Entity* entity_copy(Entity* e) {
  if(!e) return NULL;
  Entity* r = entity_ini(e->name, e->t, e->ipos, e->jpos);
  if(!r) return NULL;

  r->s=spr_copy(e->s);
  if(!r->s) {
    entity_free(r);
    return NULL;
  }
  attb_free(r->attr);
  r->attr=attb_copy(e->attr);
  if(!r->attr) {
    entity_free(r);
    return NULL;
  }
  inv_free(r->inv);
  r->inv=inv_copy(e->inv);
  if(!r->inv) {
    entity_free(r);
    return NULL;
  }

  if(!entity_setDialogs(r,e->dman)) {
    entity_free(r);
    return NULL;
  }
  r->room_index=e->room_index;
  r->i1=e->i1;
  r->i2=e->i2;
  r->j1=e->j1;
  r->j2=e->j2;
  for(int i=0;i<MAX_QUESTS;++i){
    r->adq_quests[i]=quest_copy(e->adq_quests[i]);
  }

  return r;
}

Entity* entity_setName(Entity* p, char* c){
  if(!p || !c) return NULL;
  if (strlen(c) >= MAX_NAME_LENGTH) {
    printf("Name can't have so many characters.");
    return NULL;
  }
  strcpy(p->name, c);
  return p;
}

Entity* entity_setSprite(Entity* p,int d){
  if(!p) return NULL;
  if(p->s) spr_free(p->s);
  p->s = sdic_lookup(d);
  if(!p->s)return NULL;
  spr_setOI(p->s,1);
  spr_setOJ(p->s,1);
  return p;
}

Sprite* entity_getSprite(Entity* p) {
  if(!p||!p->s) return NULL;

  Sprite* rs=spr_copy(p->s);
  if(!rs) return NULL;
  return rs;
}

Entity* entity_setEntType(Entity* p, ent_type t){
  if(!p || t < 1 || t > 3) return NULL;
  p->t = t;
  return p;
}

Entity* entity_setCoordI(Entity* p, int i){
  if(!p || i < 0) return NULL;
  if(p->dis){
    disp_setSpriteI(p->dis,p->room_index,i);
  }
  p->ipos = i;
  return p;
}

Entity* entity_setCoordJ(Entity* p, int j){
  if(!p  || j < 0) return NULL;
  p->jpos = j;
  if(p->dis){
    disp_setSpriteJ(p->dis,p->room_index,j);
  }
  return p;
}



char * entity_getName(Entity* p){
  char * name = NULL;

  if(!p) return NULL;

  name = (char*) calloc (MAX_NAME_LENGTH, sizeof(char));
  if(!name) return NULL;

  strcpy(name, p->name);
  return name;
}

ent_type entity_getEntType(Entity* p){
  return p? p->t:0;
}

int entity_getCoordI(Entity* p){
  if(!p) return -1;
  return p->ipos;
}

int entity_getCoordJ(Entity* p){
  if(!p) return -1;
  return p->jpos;
}

Attributes* entity_getAttributes(Entity* p){
  if(!p || !(p->attr)) return NULL;
  return p->attr;
}

Entity* entity_setAttributes(Entity* p, Attributes* a){
  if(!p) return NULL;
  p->attr=attb_copy(a);
  if(!p->attr) return NULL;
  return p;
}

Inventory *entity_getInventory(Entity* p){
  if(!p || !(p->inv)) return NULL;
  return p->inv;
}
Entity* entity_setInventory(Entity* e, Inventory* inv){
  if(!e||!inv)return NULL;
  if(e->inv)inv_free(e->inv);
  e->inv=inv_copy(inv);
  if(!e->inv)return NULL;
  return e;
}


Entity* entity_moveUp(Entity* p, bool scroll){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,-VERTICAL_STEP,0,&p->ipos,&p->jpos,scroll);
  return p;
}
Entity* entity_moveDown(Entity* p, bool scroll){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,VERTICAL_STEP,0,&p->ipos,&p->jpos,scroll);
  return p;
}
Entity* entity_moveLeft(Entity* p, bool scroll){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,0,-HORIZONTAL_STEP,&p->ipos,&p->jpos,scroll);
  return p;
}
Entity* entity_moveRight(Entity* p, bool scroll){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,0,HORIZONTAL_STEP,&p->ipos,&p->jpos,scroll);
  return p;
}

void entity_free(Entity *p){
  if(!p) return;
  if(p->s) spr_free(p->s);

  if (p->attr) attb_free(p->attr);

  if(p->inv) inv_free(p->inv);

  for(int i=0;i<MAX_QUESTS;++i)quest_free(p->adq_quests[i]);

  if(p->dman)dman_free(p->dman);
  
  free(p);
  return;
}
Entity* entity_addtoDisplay(Entity* e, Display* dis){
  if(!e||!dis)return NULL;
  int aux;
  e->dis=dis;
  Room * r=disp_getrefRoom(dis);
  aux = room_addOSprite(r, e->s,e->t);
  if(aux < 0) {
    return NULL;
  }
  e->room_index = aux;
  return e;
}
Entity* entity_processAlly(Entity* e){
  if(!e||!e->dis)return NULL;
  Room* r=disp_getrefRoom(e->dis);
  room_processAlly(r,e,e->s,e->room_index,ENTITY_TALK_RAD);
  return e;
}

Entity* entity_processEnemy(Entity* e){
  if(!e||!e->dis)return NULL;
  room_processEnemy(disp_getrefRoom(e->dis),e,e->room_index,e->i1,e->i2,e->j1,e->j2);
  return e;
}

int entity_getRoomIndex(const Entity* en){
  return en? en->room_index: -1;
}
Entity* entity_addItem(Entity* en,int itemId, int quantity) {
  Object* ob=odic_lookup(itemId);
  if(!ob)return NULL;
  inv_insertSeveral(en->inv,ob,quantity);
  obj_free(ob);
  return en;
}
Inventory* entity_getInvRef(Entity*en){
  return en? en->inv:NULL;
}

Entity* entity_setDialogs(Entity* e, DialogMan* dman) {
  if(!e) return NULL;
  DialogMan* d=dman_copy(dman);
  e->dman=d;
  return e;
}

char* entity_getLine(Entity* e) {
  if(!e||!e->dman) return NULL;
  return dman_getLine(e->dman);
}

Entity* entity_resetDialog(Entity* e) {
  if(!e||!e->dman) return NULL;
  if(!dman_resetDialog(e->dman)) return NULL;
  return e;
}

Entity* entity_setDialog(Entity* e, int dialogid) {
  if(!e) return NULL;
  dman_setDialog(e->dman,dialogid);
  return e;
}
Entity* entity_advanceDialog(Entity* e){
  if(!e)return NULL;
  dman_advance(e->dman);
  return e;
}

int entity_getId(Entity* e){
  return e? e->id: -1;
}
bool entity_getHasDialog(Entity *e){
  return e? e->has_dialog:false;
}
DialogMan* entity_getDialogs(Entity *e){
  if(!e)return NULL;
  return e->dman;
}

Entity* entity_addQuest(Entity* en, Quest* q){
  if(!en||!q)return NULL;
  int i=0;
  while(i<MAX_QUESTS&&en->adq_quests[i])i++;
  if(i==MAX_QUESTS)return NULL;
  en->adq_quests[i]=quest_copy(q);
  return en;
}
Quest** entity_getQuests(Entity* e, int* siz){
    if(!e||!siz)return NULL;

    int cnt=0;
    while(cnt<MAX_QUESTS&&e->adq_quests[cnt])cnt++;
    Quest** ret=calloc(cnt,sizeof(Quest*));
    for(int i=0;i<cnt;++i){
      ret[i]=quest_copy(e->adq_quests[i]);
    }
    *siz=cnt;
    return ret;
}
Entity* entity_modPlayer(Entity* prev, Entity* new){
  if(!prev||!new)return NULL;
  inv_free(prev->inv); 
  prev->inv=inv_copy(new->inv);
  attb_free(prev->attr); 
  prev->attr=attb_copy(new->attr);
  return prev;
}

Quest* entity_fetchFulfilledQuest(Entity* e,char*n){
  if(!e||!n)return NULL;
  Quest* cr=NULL;

  for(int i=0;i<MAX_QUESTS;++i){
    if(!e->adq_quests[i]||!quest_getCompleted(e->adq_quests[i]))continue;
    char* cc=quest_getAsigner(e->adq_quests[i]);
    if(strcmp(cc,n)==0){
      cr=e->adq_quests[i];
      for(int j=i+1;j<MAX_QUESTS;++j){
        e->adq_quests[j-1]=e->adq_quests[j];
        e->adq_quests[j]=NULL;
      }
    }
    free(cc);
  }
  
  return cr;
}
Quest** entity_questJustCompleted(Entity* e, int* size){
  if(!e)return NULL;
  Quest** res=NULL;
  int siz=0;
  for(int i=0;i<MAX_QUESTS;++i){
    if(e->adq_quests[i]==NULL)break;
    if(quest_getCompleted(e->adq_quests[i]))continue;
    int len=0;
    pairii* req= quest_getRequirements(e->adq_quests[i],&len);
    bool build=true;
    for(int i=0;i<len;++i){
      Object* ob=odic_lookup(req[i].fi);
      if(obj_getType(ob)==BUILDING){
        if(!checkBuilding(req[i]))build=false;
        else{
          for(int j=i+1;j<len;++j){
            req[j-1]=req[j];
          }
          len--;
        }
      }
      obj_free(ob);
      if(!build)break;
    }
    if(build&&inv_checkPresent(entity_getInventory(e),req,len)){
      siz++;
      res=realloc(res,siz*sizeof(Quest*));
      quest_setCompleted(e->adq_quests[i],true);
      res[siz-1]=quest_copy(e->adq_quests[i]);
    }
    free(req);
  }
  *size=siz;
  
  return res;
}