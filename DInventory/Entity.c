
#include "Entity.h"

#define MAX_NAME_LENGTH 30
#define HORIZONTAL_STEP 30
#define VERTICAL_STEP 10

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
        Attributes* *attr;
        Inventory *inv;
        int room_index;
        Display *dis;

};


Entity *entity_ini (char *name, entType t, int x, int y){
        Entity* e = NULL;
        e = (Entity*) malloc(sizeof(Entity));
        if(!e) return NULL;
        if(name && strlen(name)+1 < MAX_NAME_LENGTH) strcpy(e->name, name);

        e->t = t;
        e->x = x;
        e->y = y;

        e->attr = atb_ini();
        if (e->attr == NULL) {
                entity_free(e);
                return NULL;
        }
        e->inv = inv_ini();
        if (e->inv == NULL) {
                entity_free(e);
                return NULL;
        }
        return e;
}
/** 
 * 
 * 
 * 
 * 
 */
Entity *entity_load(FILE* f, Display *d){
        Entity *e = NULL;
        entType t = 0;
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
        e->s=sdic_lookup(sid);

        e->dis=d;
        Room * r=disp_getrefRoom(d);
        aux = room_addOSprite(r, e->s);
        if(aux < 0) {
                entity_free(e);
                return NULL;
        }
        e->room_index = aux;

        if(entity_setName(e, name) == NULL) {
                entity_free(e);
                return NULL;
        }

        if(entity_setEntType(e,  t) == NULL) {
                entity_free(e);
                return NULL;
        }

        if(entity_setCoordX(e,  x) == NULL) {
                entity_free(e);
                return NULL;
        }

        if(entity_setCoordY(e,  y) == NULL) {
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

Entity* entity_setSprite(Entity* p, Sprite *s){
        if(!p || !s) return NULL;
        if(p->s) spr_free(p->s);
        p->s = s;
        return p;
}

Entity* entity_setEntType(Entity* p, entType t){
        if(!p || t < 1 || t > 3) return NULL;
        p->t = t;
        return p;
}

Entity* entity_setCoordX(Entity* p, int x){
        if(!p || x < 0) return NULL;
        p->x = x;
        return p;
}

Entity* entity_setCoordY(Entity* p, int y){
        if(!p  || y < 0) return NULL;
        p->y = y;
        return p;
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
        s = spr_copy(p->s);
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

Attributes *entity_getAttribute(Entity* p){
        if(!p || !(p->attr)) return NULL;
        return p->attr;
}

Inventory *entity_getInventory(Entity* p){
        if(!p || !(p->inv)) return NULL;
        return p->inv;
}



Entity* entity_moveUp(Entity* p){
        if(!p) return NULL;
        Room* r=disp_getrefRoom(p->dis); 
        if(room_incPos(r, 0, -VERTICAL_STEP, 0)==1){
                //disp_scroll(dis,-0.5,0);
                if(disp_scroll(p->dis,-0.5,0)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
            }
            else{
                room_printMod(r, 0, 0);
            }
        return p;
}
Entity* entity_moveDown(Entity* p){
        if(!p) return NULL;
        Room* r=disp_getrefRoom(p->dis); 
        if(room_incPos(r, 0, 0, VERTICAL_STEP)==2){
                if(disp_scroll(p->dis,0,0.5)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
            }
            else{
                room_printMod(r, 0, 0);
            }

        return p;
}
Entity* entity_moveLeft(Entity* p){
        if(!p) return NULL;
        Room* r=disp_getrefRoom(p->dis); 
        if(room_incPos(r, 0, 0, -HORIZONTAL_STEP)==4){
                if(disp_scroll(p->dis,0,-0.5)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
            }
            else{
                room_printMod(r, 0, 0);
            }
        return p;
}
Entity* entity_moveRight(Entity* p){
        if(!p) return NULL;
        Room* r=disp_getrefRoom(p->dis); 
        if(room_incPos(r, 0, 0, HORIZONTAL_STEP)==2){
                if(disp_scroll(p->dis,0,0.5)==1){
                    canv_print(stdout,room_getRender(r),0,0);
                }
            }
            else{
                room_printMod(r, 0, 0);
            }
        return p;
}

void entity_free(Entity *p){
        if(!p) return;
        if(p->s) spr_free(p->s);
        p->s = NULL;
        if (p->attr) atb_destroy(p->attr);
        p->attr = NULL;
        if(p->inv) inventory_destroy(p->inv);
        p->inv = NULL;
        free(p);
        p = NULL;
        return;
}