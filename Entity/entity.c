
#include <stdlib.h>
#include <string.h>
#include "entity.h"

#define MAX_NAME_LENGTH 30

typedef enum entType {PLAYER = 1, ENEMY = 2, ALLY = 3};

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
        attribute *attr;
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
  e = entity_ini();
  if(!e) return NULL;

  FILE *fopen(const char *filename, const char *mode)
}

Status entity_setName(Entity* p, char* c){
        if(!p || !c) return ERROR;
        if strlen(c) >= MAX_NAME_LENGTH{
          printf("Name can't have so many characters.");
          return ERROR;
        }
        strcpy(p->name, c);
        return OK;
}

Status entity_setSprite(Entity* p, Sprite *s){
        if(!p || !s) return ERROR;
        if (sprite_set(p->s, c) == ERROR) return ERROR;
        return OK;
}

Status entity_setEntType(Entity* p, entType t){
        if(!p) return ERROR;
        p->t = t;
        return OK;
}

Status entity_setCoordX(Entity* p, int x){
        if(!p) return ERROR;
        p->x = x;
        return OK;
}

Status entity_setCoordY(Entity* p, int y){
        if(!p) return ERROR;
        p->y = y;
        return OK;
}



char *entity_getName(Entity* p){
  char name[MAX_NAME_LENGTH];
  if(!p) return NULL;
  if(!(p->name)) return NULL;
  strcpy(name, p->name);
  return name;
}

Sprite *entity_getSprite(Entity* p){
  Sprite *s = NULL;
  if(!p || !(p->s)) return NULL;
  s = sprite_get(p);
  return s;
}

entType entity_getEntType(Entity* p){
  entType e = -1;
  if ((!p) || ((p->t != 0) && (p->t != 1) && (p->t != 2)) return e;
  return p->t;
}

int entity_getCoordX(Entity* p){
  if(!p) return 100000;
  return p->x;
}

int entity_getCoordY(Entity* p){
  if(!p) return 100000;
  return p->y;
}

attribute *entity_getAttribute(Entity* p){
  if(!p || !(p->atrr)) return NULL;
  return p->attr;
}

inventory *entity_getInventory(Entity* p){
  if(!p || !(p->inv)) return NULL;
  return p->inv;
}


Status entity_moveUp(Entity* p, int canv_size){
  if(!p) return ERROR;
  if(p->y == canv_size){
    printf("You cannot move in this direction!");
    return OK;
  }
  (p->y)--;
  return OK;
}
Status entity_moveDown(Entity* p){
  if(!p) return ERROR;
  if(p->y == 0){
    printf("You cannot move in this direction!");
    return OK;
  }
  (p->y)++;
  return OK;
}
Status entity_moveLeft(Entity* p){
  if(!p) return ERROR;
  if(p->x == canv_size){
    printf("You cannot move in this direction!");
    return OK;
  }
  (p->x)--;
  return OK;
}
Status entity_moveRight(Entity* pint canv_size){
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
  if(p->s) sprite_destroy(p->s);
  p->s = NULL;
  if (p->attr) attribue_destroy(p->atrr);
  p->atrr = NULL;
  if(p->inv) inventory_destroy(p->inv);
  p->inv = NULL;
  free(p);
  return;
}
