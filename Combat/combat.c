#include "combat.h"

struct _combat{
  char * name1[50];
  char * name2[50];
  skill * moveset1[4];
  skill * moveset2[4];
  atb * stats1;
  atb * stats2;
}

struct _combat2{
  entity *p, *e;
  entity *pc, *ec;
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

  c->pc = entity_copy(player);
  if (!c->pc){
    combat_destroy(c);
    return NULL;
  }
  c->pe = entity_copy(enemy);
  if (!c->pe){
    combat_destroy(c);
    return NULL;
  }

  /*Assuming that the iventory will have a pointer to
  the selected weapon, i->aw, an int.
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
  }
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
  fprintf(stderr, "Choose your next movement:\n",);
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
  int i = 0;

  if(!c) return -1;

  if (c->stats1->speed > c->stats2->speed){

  }


}

int movement_exe(atb *to_attack, skill *s){

}
