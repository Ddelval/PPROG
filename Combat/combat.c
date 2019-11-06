#include "combat.h"

struct _combat{
  char * name1[50];
  char * name2[50];
  skill * moveset1[4];
  skill * moveset2[4];
  atb * stats1;
  atb * stats2;
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
  int move;
  fprintf(stderr, "Choose your next movement:\n",);
  enter: scanf("%d", &move);
  if(move < 1 || move > 6){
  fprintf(stderr, "Please use a valid movement:\n",);
  goto enter;
  }
  return move;
}
