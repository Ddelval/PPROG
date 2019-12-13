/*  Entity.c  */

#include "Entity.h"

#define MAX_NAME_LENGTH 30
#define HORIZONTAL_STEP 30
#define VERTICAL_STEP 10
#define SPRITES 4

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
  char name[MAX_NAME_LENGTH];
  Sprite* s;
  ent_type t;
  int ipos;
  int jpos;
  Attributes* attr;
  Inventory* inv;
  int room_index;
  Display* dis;
  DialogDic* ddic;
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
  ent_type t = 0;
  char name[MAX_NAME_LENGTH];
  int x = 0, y = 0, aux = 0;
  e = entity_ini(NULL, 0, 0, 0);
  if(!e) return NULL;

  fscanf(f, "%s %d %d %d", name, &aux, &x, &y);
  if(entity_setEntType(e, t)) {
    entity_free(e);
    return NULL;
  }
  int sid;
  fscanf(f,"%d",&sid);
  if(entity_setSprite(e,sid)==NULL){
    entity_free(e);
    return NULL;
  }

  if(entity_addtoDisplay(e,d)==NULL){
    entity_free(e);
    return NULL;
  }
  if(entity_setName(e, name) == NULL) {
    entity_free(e);
    return NULL;
  }

  if(entity_setEntType(e,  t) == NULL) {
    entity_free(e);
    return NULL;
  }

  if(entity_setCoordI(e,  x) == NULL) {
    entity_free(e);
    return NULL;
  }

  if(entity_setCoordJ(e,  y) == NULL) {
    entity_free(e);
    return NULL;
  }

  return e;
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

Entity* entity_setEntType(Entity* p, ent_type t){
  if(!p || t < 1 || t > 3) return NULL;
  p->t = t;
  return p;
}

Entity* entity_setCoordI(Entity* p, int i){
  if(!p || i < 0) return NULL;
  p->ipos = i;
  return p;
}

Entity* entity_setCoordJ(Entity* p, int j){
  if(!p  || j < 0) return NULL;
  p->jpos = j;
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

Sprite *entity_getSprite(Entity* p){
  Sprite *s = NULL;
  if(!p || !(p->s)) return NULL;
  s = spr_copy(p->s);
  return s;
}

ent_type entity_getEntType(Entity* p){
  ent_type e = 0;
  if ((!p) || (p->t != 1) && (p->t != 2) && (p->t != 3)) return e;
  return p->t;
}

int entity_getCoordX(Entity* p){
  if(!p) return -1;
  return p->ipos;
}

int entity_getCoordY(Entity* p){
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



Entity* entity_moveUp(Entity* p){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,-VERTICAL_STEP,0,&p->ipos,&p->jpos);
  return p;
}
Entity* entity_moveDown(Entity* p){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,VERTICAL_STEP,0,&p->ipos,&p->jpos);
  return p;
}
Entity* entity_moveLeft(Entity* p){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,0,-HORIZONTAL_STEP,&p->ipos,&p->jpos);
  return p;
}
Entity* entity_moveRight(Entity* p){
  if(!p||!p->dis) return NULL;
  disp_incPos(p->dis,p->room_index,0,HORIZONTAL_STEP,&p->ipos,&p->jpos);
  return p;
}

void entity_free(Entity *p){
  if(!p) return;
  if(p->s) spr_free(p->s);
  p->s = NULL;
  if (p->attr) attb_free(p->attr);
  p->attr = NULL;
  if(p->inv) inv_free(p->inv);
  p->inv = NULL;
  free(p);
  p = NULL;
  return;
}
Entity* entity_addtoDisplay(Entity* e, Display* dis){
  if(!e||!dis)return NULL;
  int aux;
  e->dis=dis;
  Room * r=disp_getrefRoom(dis);
  aux = room_addOSprite(r, e->s);
  if(aux < 0) {
    entity_free(e);
    return NULL;
  }
  e->room_index = aux;
  return e;
}
int entity_getRoomIndex(const Entity* en){
  return en? en->room_index: -1;
}
Entity* entity_addItem(Entity* en,int itemId, int quantity){
  Object* ob=odic_lookup(itemId);
  if(!ob)return NULL;
  inv_insertSeveral(en->inv,ob,quantity);
  return en;
}
const Inventory* entity_getInvRef(Entity*en){
  return en? en->inv:NULL;
}
