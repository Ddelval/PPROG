#include "combat.h"
#include "Window.h"
#include "Room.h"
#include "Font.h"
#include "Pixel.h"
#include <stdilib.h>
#include <time.h>

struct _combat{
  entity *p, *e;
  char name[2][50];
  atb * stats[2];
  skill * moveset[2][4];
  window * window[3];
  room * room;
  Bool stunp, stune;
}

combat * combat_ini(entity * player, entity * enemy){
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

  c->stunp = FALSE;
  c->stune = FALSE;

//SKILLS LOADING ??
  for(pload = 0, pload < 4, pload++){
    //SELECT SKILLS BASED ON WEAPON , WE NEED TO WORK THROUGH THIS URGENTLY
  }c->stats[0]->health
  for(eload = 0, eload < 4, eload++){
    //SELECT SKILLS BASED ON WEAPON , WE NEED TO WORK THROUGH THIS URGENTLY
  }
  Pixel* backroom=pix_ini(134, 151, 179, 255)
  FILE* f=fopen("Display/Fonts/Robo_Mono/08.dat");
  Font* titlef = font_load(f);
  combat->window[0] = win_ini(entity_getName(player), NULL, 0, 0, 0, 0, 0, titlef);
  combat->window[1] = win_ini(entity_getName(enemy), NULL, 0, 0, 0, 0, 0, titlef);
  combat->window[2] = win_ini("ACTIONS", NULL, 0, 0, 0, 0, 0, titlef);
  combat->room = room_ini(902, "COMBAT!",0, 0, backroom);


  return c;
}



/*
Status combat_process(combat * state){
  int i;

  if(!combat){
    print("Uppss");
    return ERROR;
  }

  if(atb_getter(state->stats[0],4) > atb_getter(state->stats[1],4)){
    i = 0;
  }
  else i = 1;

  while(atb_getter(state->stats[0],1) > 0 && atb_getter(state->stats[1],1) > 0){
    if(i % 2 == 0){
      move_exe(state, player_choice);
    }
    if(i % 2 == 1){
      move_exe(state, IA_choice);
    }
  }
  return OK;
}
*/


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
    if(atb_getter(state->moveset[i], 2) > atb_getter(state->moveset[max_attack], 2)) max_attack = i;
    i++;
  }

  return max_attack;
}



int combat_exe(combat *c){
  int i = 0, aux = 1, move = 0;

  if(!c) return -1;

  if(atb_getter(state->stats[0],4) > atb_getter(state->stats[1],4)){
    aux = 0;
  }



  while(atb_getter(state->stats[0],1) > 0 && atb_getter(state->stats[1],1)>0){
    if(i+aux % 2 == 0){
      if(c->stunp == FALSE){
        fprintf(stdout, "El jugador ataca primero, selecciona una acción:\n");
        fprintf(stdout, "Listado de movimientos:\n %s\t %s\n%s\t%s\n", c->moveset[0][0]->name,c->moveset[0][1]->name,c->moveset[0][2]->name,c->moveset[0][3]->name);
        move = player_choice();
        movement_exe(c, move, 0);
      }
      if(c->stunp == TRUE){
        fprintf(stdout, "You have been stunned, meanwhile, you can have some tea.\n");
        stunp = FALSE;
      }

    }
    else{
      if(c->stune == FALSE){
        move = IA_choice();
        movement_exe(c, move, 1);
      }
      if(c->stunp == TRUE){
        fprintf(stdout, "THe enemy has been stunned, his damage increased by 100, just joking.\n");
        stunp = FALSE;
      }
    }
    i++;
  }
  //decir qn ha muerto y liberar la memoria de la strutura combat
  return 0;
}

int movement_exe(combat * c, int action, int entity){
  int other =0;
  fprintf(stdout, "%s ataca con %s.\n"c->name[entity], c->moveset[entity][action]->name);
  if(entity == 0){
    other = 1;
  }
  if (attack_goes(c,c->moveset[entity][action],entity) == TRUE){
    skill_stun(c,c->moveset[entity][action], entity);

    atb_merge(c->stats[entity], skill_getAtbSelf(c->moveset[entity][action]));

    if(atb_getter(c->moveset[entity][action],2) == 0){
      atb_merge(c->stats[entity], skill_getAtbSelf(c->moveset[entity][action]);
    }
    else{

    }
  }
  else fprintf(stdout, "Attack dogded");
}


void skill_stun(combat * c, skill * skil, int who){
  //CONTROL ERRORES
  if(strcmp(skill_getSpecial(skil), "STUNNER") != 0) return;
  if(who == 0){
    c->stune = TRUE;
  }
  else{
    c->stunp = TRUE;
  }
}



Bool attack_goes(combat * c, skill * skil, int who){
  //CONTROL DE ERRRORES
  double res;
  double random;
  int p1, p2;
  p1 = atb_getter(c->stats[who], 5);
  if(who == 0){
  p2 = atb_getter(c->stats[1], 5);
  }
  else p2 = atb_getter(c->stats[0], 5);

  random = (double)rand()
  random /= MAX_RAND;

  res = (p1/100) * 1.8 - (p2/100) - random;

  if(strcmp(skill_getSpecial(skil), "UNDOGDE") == 0){
    res += 3;
  }
  if(res < 0) return FALSE;
  if(res >= 0) return TRUE;
}
