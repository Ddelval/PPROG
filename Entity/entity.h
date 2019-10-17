

#ifndef entity.h
#define entity.h

enum entType;
typedef struct _Entity Entity;

/*Creates an entity and assigns a name, a sprite, an entType, a x and y coordinate
and initializes the inventory and attribute field. If either of the pounter arguments is NULL, it sets
that field to NULL*/
Entity *entity_ini (char *name, Sprite *s, entType t, int x, int y);

/* */
Entity *entity_load(char *file);

/* Renames the character, if either of the aruments is NULL it will return ERROR*/
Status entity_setName(Entity* p, char* c);
Status entity_setSprite(Entity* p, Sprite *s);
Status entity_setEntType(Entity* p, entType t);
Status entity_setCoordX(Entity* p, int x);
Status entity_setCoordY(Entity* p, int y);

/* Allocates memory for a char pointer that will be returned with the name of p
unless p or p's name is NULL*/
char *entity_getName(Entity* p);
Sprite *entity_getSprite(Entity* p);
/* Returns -1 if p is NULL or the entType field has not been defined yet; or
the entTypt of p otherwise. */
entType entity_getEntType(Entity* p);
/* Returns 10000(which would be out of the screen) if p is NULL or the x
coordinate of p otherwise*/
int entity_getCoordX(Entity* p);
/* Returns 10000(which would be out of the screen) if p is NULL or the y
coordinate of p otherwise*/
int entity_getCoordY(Entity* p);
/* Returns the pointer to the attribute field of the entity passed as argument,
returns NULL if there is an error*/
attribute *entity_getAttribute(Entity* p);
/* Returns the pointer to the inventory field of the entity passed as argument,
returns NULL if there is an error*/
inventory *entity_getInventory(Entity* p);

/* Arguments: p, the entity will be moved and canv:size, the size of the canvas
where the player is located.
If p is NULL return ERROR, if the player tries to go out of the canvas he
will not move and will recieve a warning error and the function will return
OK. It will increase y's coordinate once otherwise.*/
Status entity_moveUp(Entity* p, int canv_size);
Status entity_moveDown(Entity* p);
Status entity_moveLeft(Entity* p);
Status entity_moveRight(Entity* p, int canv_size);

void entity_destroy(Entity *p);


#endif
