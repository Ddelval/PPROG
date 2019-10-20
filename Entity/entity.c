
#include <stdlib.h>
#include <string.h>
#include "entity.h"

#define MAX_NAME_LENGTH 30

typedef enum {PLAYER = 1, ENEMY = 2, ALLY = 3} entType;

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
        Sprite *s;
        entType t;
        int x;
        int y;
        atb *attr;
        inventory *inv;
};


Entity *entity_ini (char *name, Sprite *s, entType t, int x, int y){
        Entity* e = NULL;
        e = (Entity*) malloc(sizeof(Entity));
        if(!e) return NULL;
        if(!name || strlen(name)>=MAX_NAME_LENGTH) e->name = NULL;
        else{
          strcpy(e->name, name);
        }
        if (sprite_set(e->s, s) == ERROR){
          entity_destroy(e);
          return NULL;
        }
        e->t = t;
        e->x = x;
        e->y = y;
        e->attr = atb_ini();
        if (e->attr == NULL){
          entity_destroy(e);
          return NULL;
        }
        e->inv = inv_ini();
        if (e->inv == NULL){
          entity_destroy(e);
          return NULL;
        }

        return e;
}

Entity *entity_load(char *file){
  Entity *e = NULL;
  FILE *f = NULL;
  entType t = 0;
  char name[MAX_NAME_LENGTH];
  int x = 0, y = 0;
  e = entity_ini(NULL, NULL, 0, 0, 0);
  if(!e) return NULL;

  f = fopen(file, "r");
   if (f == NULL) {
       printf("Error when opening the file\n");
       entity_destroy(e);
       return NULL;
   }

   fscanf(f, "%s %d %d %d", name, t, x, y);
   if (spr_load(file) == NULL){
     entity_destroy(e);
     return NULL;
   }

   if(entity_setName(Entity* p, char* c) == ERROR){
     entity_destroy(e);
     return NULL;
   }

   if(entity_setEntType(e,  t) == ERROR) {
     entity_destroy(e);
     return NULL;
   }

   if(entity_setCoordX(e,  x) == ERROR) {
     entity_destroy(e);
     return NULL;
   }

   if(entity_setCoordY(e,  y) == ERROR) {
     entity_destroy(e);
     return NULL;
   }

   return e;
}

Status entity_setName(Entity* p, char* c){
        if(!p || !c) return ERROR;
        if (strlen(c) >= MAX_NAME_LENGTH){
          printf("Name can't have so many characters.");
          return ERROR;
        }
        strcpy(p->name, c);
        return OK;
}

Status entity_setSprite(Entity* p, Sprite *s){
        if(!p || !s) return ERROR;
        p->s = s;
        return OK;
}

Status entity_setEntType(Entity* p, entType t){
        if(!p || t < 1 || t > 3) return ERROR;
        p->t = t;
        return OK;
}

Status entity_setCoordX(Entity* p, int x){
        if(!p || x < 0) return ERROR;
        p->x = x;
        return OK;
}

Status entity_setCoordY(Entity* p, int y){
        if(!p  || y < 0) return ERROR;
        p->y = y;
        return OK;
}



char *entity_getName(Entity* p){
  char *name = NULL;

  if(!p) return NULL;
  if(!(p->name)) return NULL;

  name = (char*) calloc (MAX_NAME_LENGTH, sizeof(char));
  if(!name) return NULL;

  strcpy(name, p->name);
  return name;
}

Sprite *entity_getSprite(Entity* p){
  Sprite *s = NULL;
  if(!p || !(p->s)) return NULL;
  s = sprite_copy(p->s);
  return s;
}

entType entity_getEntType(Entity* p){
  entType e = 0;
  if ((!p) || (p->t != 1) && (p->t != 2) && (p->t != 3)) return e;
  return p->t;
}

int entity_getCoordX(Entity* p){
  if(!p) return -1;
  return p->x;
}

int entity_getCoordY(Entity* p){
  if(!p) return -1;
  return p->y;
}

atb *entity_getAttribute(Entity* p){
  atb *a = NULL;
  if(!p || !(p->attr)) return NULL;
  a = attribute_copy(p->attr);
  return a;
}

inventory *entity_getInventory(Entity* p){
  inventory *i = NULL;
  if(!p || !(p->inv)) return NULL;
  i = inventory_copy(p->inv);
  return i;
}



Status entity_moveUp(Entity* p){
  if(!p) return ERROR;
  if(p->y == 0){
    printf("You cannot move in this direction!");
    return OK;
  }
  (p->y)--;
  return OK;
}
Status entity_moveDown(Entity* p, int canv_size){
  if(!p) return ERROR;
  if(p->y == canv_size){
    printf("You cannot move in this direction!");
    return OK;
  }
  (p->y)++;
  return OK;
}
Status entity_moveLeft(Entity* p){
  if(!p) return ERROR;
  if(p->x == 0){
    printf("You cannot move in this direction!");
    return OK;
  }
  (p->x)--;
  return OK;
}
Status entity_moveRight(Entity* p, int canv_size){
  if(!p) return ERROR;
  if(p->x == canv_size){
    printf("You cannot move in this direction!");
    return OK;
  }
  (p->x)++;
  return OK;
}

void entity_destroy(Entity *p){
  if(!p) return;
  if(p->name) free(p->name);
  p->name = NULL;
  if(p->s) spr_free(p->s);
  p->s = NULL;
  if (p->attr) attribue_destroy(p->atrr);
  p->atrr = NULL;
  if(p->inv) inventory_destroy(p->inv);
  p->inv = NULL;
  free(p);
  p = NULL;
  return;
}
