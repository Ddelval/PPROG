#include "combat.h"

struct _combat{
  entity *p, *e;
  char name[2][50];
  atb * stats[2];
  skill * moveset[2][4];
}

combat * combat_ini2(entity * player, entity * enemy){
  combat * c;
  int aux = 0;

  if(!player || !enemy) return NULL;

  c = (combat*)calloc(1,sizeof(combat));
  if(!c) return NULL;

  c->p = player;
  c->e = enemy;

  c->name[0] = entity_getName(player);
  c->name[1] = entity_getName(enemy);

  c->stats[0] = atb_allCopy(entity_getAttribute(player));
  c->stats[1]= atb_allCopy(entity_getAttribute(enemy));

  /*Assuming that the iventory will have a pointer to the selected weapon, i->aw, an int.
  We should separate objects between weapons and consumables,
  weapons should have an array of possible attacks and
  each attack is just an attribute which can modify either
  attacking entity attributes or the attacked entity
  */


  object *wp = NULL, *we = NULL;

  // inventory_getWeapon: Inputs: inventory, active weapon int.
  wp = inventory_getWeapon(entity_getInventory(player), inventory_getAW(entity_getInventory(player)));
  if(!wp){
    combat_destroy(c);
    return NULL;
  }
  while(aux < 4 and weapon_getAtb(wp, aux)){
    moveset[0][aux] = weapon_getAtb(wp, aux);
    aux++;
  }

  aux = 0;
  we = inventory_getWeapon(entity_getInventory(enemy), inventory_getAW(entity_getInventory(enemy)));
  if(!we){
    combat_destroy(c);
    return NULL;
  }
  while(aux < 4 and weapon_getAtb(we, aux)){
    moveset[1][aux] = weapon_getAtb(we, aux);
    aux++;
  }

  return c;
}


combat * combat_ini(entity * player, entity * enemy){
  if(!player || !enemy) return NULL;

  combat * state;
  int pload, eload;
  state = (combat*)calloc(1,sizeof(combat));
  if(!state) return NULL;

  state->name1 = entity_getName(player);
  state->name2 = entity_getName(player);
  state->stats1 = atb_allCopy(entity_getAttribute(player));
  state->stats1 = atb_allCopy(entity_getAttribute(enemy));
  for(pload = 0, pload < 4, pload++){
    //SELECT SKILLS BASED ON WEAPON , WE NEED TO WORK THROUGH THIS URGENTLY
  }c->stats[0]->health
  for(eload = 0, eload < 4, eload++){
    //SELECT SKILLS BASED ON WEAPON , WE NEED TO WORK THROUGH THIS URGENTLY
  }


  return combat;
}



void combat_process(combat * state){
  int i;

  if(!combat){
    print("Uppss");
    return;
  }

  if(atb_getter(state->stats1, int 4 > atb_getter(state->stats1, int 4)){
    i = 0;
  }
  else i = 1;

  while(atb_getter(state->stats1, int 1) > 0 && atb_getter(state->stats2, int 1) > 0){
    if(i % 2 == 0){
      move_exe(state, player_choice);
    }
    if(i % 2 == 1){
      move_exe(state, IA_choice);
    }
  }
}



int player_choice(){
  int move = 0;
  while(move < 1 || move > 6){
    scanf("%d", &move);
    if(move == 1 || move == 2 || move == 3 || move == 4 || move == 5 || move == 6) break;
    fprintf(stderr, "Please use a valid movement:\n",);
  }
  return move;
}


// For now the IA will just choose the attack with the highest attack field.
int IA_choice(combat * state){
  int max_attack = 0;
  int i = 1;

  if(!state) return -1;

  while(moveset2[i]){
    if(state->moveset2(i)->attack > state->moveset2[max_attack]->attack){
      max_attack = i;
    }
    i++;
  }
  return max_attack;
}



int combat_exe(combat *c){
  int i = 0, aux = 0, move = 0;

  if(!c) return -1;

  if (c->stats[0]->speed < c->stats[1]->speed){
    aux = 1;
  }
  while(c->stats[0]->health > 0 && c->stats[1]->health > 0){
    if(i+aux % 2 == 0){
      fprintf(stdout, "El jugador ataca primero, selecciona una acción:\n");
      fprintf(stdout, "Listado de movimientos:\n %s\t %s\n%s\t%s\n", c->moveset[0][0]->name,c->moveset[0][1]->name,c->moveset[0][2]->name,c->moveset[0][3]->name);
      move = player_choice();
      movement_exe(c, move, 0);
    }
    else{
      move = IA_choice();
      movement_exe(c, move, 1);
    }
    i++;
  }
}

int movement_exe(combat * c, int action, int entity){
  fprintf(stdout, "%s ataca con %s.\n"c->name[entity], c->moveset[entity][action]->name);
  atb_merge(c->stat[entity], skill_getAtb(c->moveset[entity][action]));
  
}
