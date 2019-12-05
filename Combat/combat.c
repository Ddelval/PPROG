#include "combat.h"
#include "Window.h"
#include "Room.h"
#include "Font.h"
#include "Pixel.h"
#include <stdlib.h>
#include <time.h>





struct _Combat{
  Entity *p, *e;
  char * name[2];
  Attributes * stats[2];
  skill * moveset[2][4];
  Window * window[3];
  Room * room;
  Bool stunp, stune;
};



/*Funcion de inicialización de la estructura combate */
//REVISADA NO COMPILA
Combat * combat_ini(Entity * player, Entity * enemy){
  Combat * c;
  int aux = 0;
  int load;
  if(!player || !enemy) return NULL;

  c = (Combat*)calloc(1,sizeof(Combat));
  if(!c) return NULL;

  c->p = player;
  c->e = enemy;

  c->name[0] = entity_getName(player);
  c->name[1] = entity_getName(enemy);

  c->stats[0] = attb_copy(entity_getAttribute(player));
  c->stats[1]= attb_copy(entity_getAttribute(enemy));

  c->stunp = FALSE;
  c->stune = FALSE;

//SKILLS LOADING ??
//  for(load = 0, load < 3, load++){
//    c->moveset[0][load]= skill_readFromFile("skill.txt",entity_getSelected(c->p));//OJO
//    c->moveset[1][load]= skill_readFromFile("skill.txt",entity_getSelected(c->e));
//  }

  Pixel* backroom=pix_ini(134, 151, 179, 255);
  FILE* f;
  f = fopen("Display/Fonts/Robo_Mono/08.dat","r");
  Font* titlef = font_load(f);
  c->window[0] = win_ini(entity_getName(player), NULL, 0, 0, 0, 0, 0, titlef);
  c->window[1] = win_ini(entity_getName(enemy), NULL, 0, 0, 0, 0, 0, titlef);
  c->window[2] = win_ini("ACTIONS", NULL, 0, 0, 0, 0, 0, titlef);
  c->room = room_ini(902, "COMBAT!",0, 0, backroom);


  return c;
}

//REVISADA, NO COMPILA
int player_choice(){
  int move = 0;
  while(move < 1 || move > 6){
    scanf("%d", &move);
    if(move < 6 && move > 1) break;
    fprintf(stderr, "Please use a valid movement:\n");
  }
  return move;
}


// For now the IA will just choose the attack with the highest attack field.
//REVISADA, NO COMPILA
int IA_choice(Combat * state){
  int max_attack = 0;
  int i = 0;

  if(!state) return -1;
//ESTAMOS ASUMIENDO QUE EL ENEMIGO TIENE 4 ATAQUES SINO QUIERES QUE ESTO OCURRA AÑADE UN CAMPO A LA ESTRUCTURA SKILL
  while(i <= 3){
    if(attb_get(skill_getAtbatk(state->moveset[1][i]), 1) > attb_get(skill_getAtbatk(state->moveset[1][max_attack]), 1)){
        max_attack = i;
    }
    i++;
  }

  return max_attack;
}



int combat_exe(Combat *c){
  int i = 0, aux = 1, move = 0;

  if(!c) return -1;

  if(attb_get(c->stats[0],3) > attb_get(c->stats[1],3)){
    aux = 0;
  }



  while(attb_get(c->stats[0],0) > 0 && attb_get(c->stats[1],0)>0){
    if(i+aux % 2 == 0){
      if(c->stunp == FALSE){
        fprintf(stdout, "El jugador ataca primero, selecciona una acción:\n");
        fprintf(stdout, "Listado de movimientos:\n %s\t %s\n%s\t%s\n", skill_getName(c->moveset[0][0]),skill_getName(c->moveset[0][1]),skill_getName(c->moveset[0][2]),skill_getName(c->moveset[0][03]));
        move = player_choice();
        movement_exe(c, move, 0);
      }
      if(c->stunp == TRUE){
        fprintf(stdout, "You have been stunned, meanwhile, you can have some tea.\n");
        c->stunp = FALSE;
      }

    }
    else{
      if(c->stune == FALSE){
        move = IA_choice(c);
        movement_exe(c, move, 1);
      }
      if(c->stunp == TRUE){
        fprintf(stdout, "THe enemy has been stunned, his damage increased by 100, just joking.\n");
        c->stunp = FALSE;
      }
    }
    i++;
  }

  combat_destroy(c);
  //decir qn ha muerto y liberar la memoria de la strutura combat
  return 0;
}

//REVISADA, NO COMPILA
Bool attack_goes(Combat * c, skill * skil, int who){
  if(!c || !skil){
    printf("FATAL ERROR FUNCTION ATTACK_GOES");
    return FALSE;
  }
  double res;
  double random;
  int p1, p2;
  p1 = attb_get(c->stats[who], 5);
  if(who == 0){
  p2 = attb_get(c->stats[1], 5);
  }
  else p2 = attb_get(c->stats[0], 5);

  random = (double)rand();
  random /= RAND_MAX;

  res = (p1/100) * 1.8 - (p2/100) - random;

  if(skill_getSpecial(skil) == 1){
    res += 3;
  }
  if(res < 0) return FALSE;
  if(res >= 0) return TRUE;
}


//REVISADA, NO COMPILA
void skill_stun(Combat * c, skill * skil, int who){
  if(!c || !skil ) return;
  if(skill_getSpecial(skil) != 0) return;
  if(who == 0){
    c->stune = TRUE;
  }
  else{
    c->stunp = TRUE;
  }
}




//ESTA FUNCION REQUERE DE UNA ACTUALIZACION DEL ATB.H
int movement_exe(Combat * c, int action, int ent){
  int other =0;
  int dmgout;
  Attributes * aux1;
  Attributes * aux2;
  int * attr;
  fprintf(stdout, "%s ataca con %s.\n",c->name[ent], skill_getName(c->moveset[ent][action]));
  if(ent == 0){
    other = 1;
  }
  if (attack_goes(c,c->moveset[ent][action],ent) == TRUE){
      skill_stun(c,c->moveset[ent][action], ent);

      aux1 = attb_merge(c->stats[ent], skill_getAtbatk(c->moveset[ent][action]));

      if(attb_get(skill_getAtbatk(c->moveset[ent][action]),1) == 0){
      aux2 = attb_merge(c->stats[ent], skill_getAtbself(c->moveset[ent][action]));
      
      
      attr = attb_getAll(aux2);
      
      attb_setAll(c->stats[ent],attr);
      attb_free(aux2);
    }
    else{
      dmgout = attb_get(aux1,1) - attb_get(c->stats[other], 2);
      attb_set(c->stats[other],attb_get(c->stats[other],0) - dmgout,0);
    }
    free(attr);
    attb_free(aux1);
  }
  else fprintf(stdout, "Attack dogded");
  return 0;
}






//REVISADA, NO COMPILA
void combat_destroy(Combat * c){
  if(!c) return;
  int i;
  room_free(c->room);
  attb_free(c->stats[0]);
  attb_free(c->stats[1]);
  win_free(c->window[0]);
  win_free(c->window[1]);
  win_free(c->window[2]);

  for(i=0; i<3;i++){
    skill_free(c->moveset[0][i]);
    skill_free(c->moveset[1][i]);
  }
}