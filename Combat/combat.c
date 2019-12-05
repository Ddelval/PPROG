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
  Attributes * stats[2];
  skill * moveset[2][4];
  window * window[3];
  room * room;
  Bool stunp, stune;
}



/*Funcion de inicialización de la estructura combate */
//REVISADA NO COMPILA
combat * combat_ini(entity * player, entity * enemy){
  combat * c;
  int aux = 0;
  int load;
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
  for(load = 0, load < 3, load++){
    c->moveset[0][i]= skill_readFromFile("",entity_getSelected(c->p));//OJO
    c->moveset[1][i]= skill_readFromFile("",entity_getSelected(c->e));
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

//REVISADA, NO COMPILA
int player_choice(){
  int move = 0;
  while(move < 1 || move > 6){
    scanf("%d", &move);
    if(move < 6 && move > 1) break;
    fprintf(stderr, "Please use a valid movement:\n",);
  }
  return move;
}


// For now the IA will just choose the attack with the highest attack field.
//REVISADA, NO COMPILA
int IA_choice(combat * state){
  int max_attack = 0;
  int i = 0;

  if(!state) return -1;
//ESTAMOS ASUMIENDO QUE EL ENEMIGO TIENE 4 ATAQUES SINO QUIERES QUE ESTO OCURRA AÑADE UN CAMPO A LA ESTRUCTURA SKILL
  while(i <= 3){
    if(attb_get(state->moveset[i], 1) > attb_get(state->moveset[max_attack], 1)) max_attack = i;
    i++;
  }

  return max_attack;
}



int combat_exe(combat *c){
  int i = 0, aux = 1, move = 0;

  if(!c) return -1;

  if(attb_get(state->stats[0],3) > attb_get(state->stats[1],3)){
    aux = 0;
  }



  while(attb_get(state->stats[0],0) > 0 && attb_get(state->stats[1],0)>0){
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

  combat_destroy(c);
  //decir qn ha muerto y liberar la memoria de la strutura combat
  return 0;
}
//ESTA FUNCION REQUERE DE UNA ACTUALIZACION DEL ATB.H
int movement_exe(combat * c, int action, int entity){
  int other =0;
  int dmgout;
  Attributes * aux1;
  Attributes * aux2;
  fprintf(stdout, "%s ataca con %s.\n"c->name[entity], c->moveset[entity][action]->name);
  if(entity == 0){
    other = 1;
  }
  if (attack_goes(c,c->moveset[entity][action],entity) == TRUE){
      skill_stun(c,c->moveset[entity][action], entity);

      aux = attb_merge(c->stats[entity], skill_getAtbAtk(c->moveset[entity][action]));

      if(attb_get(skill_getAtbAtk(c->moveset[entity][action]),1) == 0){
      aux2 = attb_merge(c->stats[entity], skill_getAtbself(c->moveset[entity][action]);
      attb_setAll(c->stats[entity],attb_getAll(aux2));
      attb_free(aux2);
    }
    else{
      dmgout = attb_get(aux1,1) - attb_get(c->stats[other], 2);
      attb_set(c->stats[other],attb_get(c->stats[other],0) - dmgout,0);
    }
    attb_free(aux1);
  }
  else fprintf(stdout, "Attack dogded");
  return 0;
}

//REVISADA, NO COMPILA
void skill_stun(combat * c, skill * skil, int who){
  if(!c || !skil ) return;
  if(skill_getSpecial(skil) != 0) return;
  if(who == 0){
    c->stune = TRUE;
  }
  else{
    c->stunp = TRUE;
  }
}


//REVISADA, NO COMPILA
Bool attack_goes(combat * c, skill * skil, int who){
  if(!c || !skil){
    printf("FATAL ERROR FUNTCION ATTACK_GOES");
    return FALSE;
  };
  double res;
  double random;
  int p1, p2;
  p1 = attb_get(c->stats[who], 5);
  if(who == 0){
  p2 = attb_get(c->stats[1], 5);
  }
  else p2 = attb_get(c->stats[0], 5);

  random = (double)rand()
  random /= MAX_RAND;

  res = (p1/100) * 1.8 - (p2/100) - random;

  if(skill_getSpecial(skil) == 1){
    res += 3;
  }
  if(res < 0) return FALSE;
  if(res >= 0) return TRUE;
}

//REVISADA, NO COMPILA
void combat_destroy(combat * c){
  if(!c) return;
  int i;
  room_free(c->room);
  attb_free(c->stats[0]);
  attb_free(c->stats[1]);
  win_free(c->window[0]);
  win_free(c->window[1]);
  win_free(c->window[2]);

  for(i=0; i<3, i++){
    skill_free(c->moveset[0][i]);
    skill_free(c->moveset[1][i]);
  }
