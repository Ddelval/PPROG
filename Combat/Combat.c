/*  Combat.c    */

#include "Combat.h"
#include "Display.h"
#include <stdlib.h>
#include <time.h>

#define PLAYER_ACTIONS 2 //Fixed window index for player actions
#define COMBAT_ROOM 0 //Index of the only room in c->cd. For readability only
#define MAX_ATTACKS 4
#define TOP_STATS 150
struct _Combat {
    Entity *player, *enemy;
    Object *weapons[2];
    char* name[2];
    Attributes* stats[2];
    Skill** moveset[2];
    bool stunplayer, stunenemy;
    Display* cd;
};


Combat* combat_ini(Entity* player, Entity* enemy) {
  if(!player||!enemy) return NULL;
  Combat* c = (Combat*)calloc(1, sizeof(Combat));
  if(!c) return NULL;
  FILE* f=fopen("Display/Fonts/Robo_Mono/08.txt", "r");
  Font* f8=font_load(f);
  fclose(f);
  if(!f8) {
    free(c);
    return NULL;
  }
  f=fopen("Worlds/c1.txt", "r");
  Room* cr=room_load(f);
  if(!cr) {
    free(c);
    font_free(f8);
    return NULL;
  }
  fclose(f);
  c->cd = disp_ini(DISP_WIDTH, DISP_HEIGHT, cr, 800, "COMBAT!", f8);
  if(!c->cd) {
    free(c);
    font_free(f8);
    return NULL;
  }
  c->player = entity_copy(player);
  if(!c->player) {
    combat_free(c);
    font_free(f8);
    return NULL;
  }
  if(!entity_addtoDisplay(c->player, c->cd)) {
    combat_free(c);
    font_free(f8);
    return NULL;
  }
  c->enemy = entity_copy(enemy);
  if(!c->enemy) {
    combat_free(c);
    return NULL;
  }
  if(!entity_addtoDisplay(c->enemy, c->cd)) {
    combat_free(c);
    font_free(f8);
    return NULL;
  }
  c->name[0] = entity_getName(player);
  c->name[1] = entity_getName(enemy);
  c->stats[0] = attb_copy(entity_getAttributes(player));
  c->stats[1] = attb_copy(entity_getAttributes(enemy));
  c->stunplayer = false;
  c->stunenemy = false;

  c->weapons[0] = inv_getSelected(entity_getInventory(player), WEAPON);
  c->weapons[1]= inv_getSelected(entity_getInventory(enemy), WEAPON);


  c->moveset[0] = obj_getAttacks(c->weapons[0]);
  c->moveset[1] = obj_getAttacks(c->weapons[1]);

  return c;
}

Combat* combat_incrMove(Combat* c, int incr) {
  if(!c||!c->cd) return NULL;
  if(!disp_incSelIndex(c->cd, PLAYER_ACTIONS,incr)) return NULL;

  return c;
}

Combat* combat_executeMove(Combat* c) {
  if(!c||!c->cd||!c->player) return NULL;

  int mindex=disp_getSelIndex(c->cd, PLAYER_ACTIONS);
  if(mindex==-1) return NULL;
  /*So far*/
  int ent=0;
  int action=mindex;
  /*So far*/
  int other = 0;
  int dmgout;
  Attributes * aux1;
  Attributes * aux2;
  int * attr;
  fprintf(stdout, "%s ataca con %s.\n", c->name[ent], skill_getName(c->moveset[ent][action]));
  if (ent == 0) {
      other = 1;
  }
  if (attack_goes(c, c->moveset[ent][action], ent) == true) {
      skill_stun(c, c->moveset[ent][action], ent);

      aux1 = attb_merge(c->stats[ent], skill_getAtbatk(c->moveset[ent][action]));

      if (attb_get(skill_getAtbatk(c->moveset[ent][action]), 1) == 0) {
          aux2 = attb_merge(c->stats[ent], skill_getAtbself(c->moveset[ent][action]));

          // HOTFIX IN ORDER TO NOT ALLOW OVERHEALING
          if (other == 1) {
              if (attb_get(aux2, 0) > attb_get(entity_getAttributes(c->player), 0)) {
                  attb_set(aux2, attb_get(entity_getAttributes(c->player), 0), 0);
              }
          } else {
              if (attb_get(aux2, 0) > attb_get(entity_getAttributes(c->enemy), 0)) {
                  attb_set(aux2, attb_get(entity_getAttributes(c->enemy), 0), 0);
              }
          }


          attr = attb_getAll(aux2);

          attb_setAll(c->stats[ent], attr);
          attb_free(aux2);
      } else {
          dmgout = attb_get(aux1, 1) - attb_get(c->stats[other], 2);
          if (dmgout < 0) dmgout = 0;
          attb_set(c->stats[other], attb_get(c->stats[other], 0) - dmgout, 0);
      }

      attb_free(aux1);
  } else fprintf(stdout, "Attack dogded");
  return 0;
}


Combat* combat_enemyMove(Combat* c) {
  if(!c||!c->enemy) return NULL;
  return c; /*Adaptation*/
}
/*
 int IA_choice(Combat * state) {
     int max_attack = 0;
     int i = 0;
     bool founded[4];
     if (!state) return -1;




//EMERGENCY HEALING
     if(attb_get(state->stats[1], 0) < 30){
       i=0;
       while (i <= 3) {
           if (attb_get(skill_getAtbself(state->moveset[1][i]), 0) > attb_get(skill_getAtbself(state->moveset[1][max_attack]), 0)) {
               max_attack = i;
           }
           i++;
       }

       if(attb_get(skill_getAtbself(state->moveset[1][max_attack]), 0) != 0){
         return max_attack;
       }
     }
//STAT BOOSTING ATK
    if(attb_get(state->stats[1], 1) < attb_get(state->stats[0],1)-20 ){
      i=0;
      while (i <= 3) {
          if (attb_get(skill_getAtbself(state->moveset[1][i]), 1) > attb_get(skill_getAtbself(state->moveset[1][max_attack]), 1)) {
              max_attack = i;
          }
          i++;
      }

      if(attb_get(skill_getAtbself(state->moveset[1][max_attack]), 1) != 0){
        return max_attack;
      }
    }
    //STAT BOOSTING DEF
    if(attb_get(state->stats[1], 2) < attb_get(state->stats[0],1)-25 ){
      i=0;
      while (i <= 3) {
          if (attb_get(skill_getAtbself(state->moveset[1][i]), 2) > attb_get(skill_getAtbself(state->moveset[1][max_attack]), 2)) {
              max_attack = i;
          }
          i++;
      }

      if(attb_get(skill_getAtbself(state->moveset[1][max_attack]), 2) != 0){
        return max_attack;
      }
    }
    //STAT BOOSTING AGL
    if(attb_get(state->stats[1], 2) < attb_get(state->stats[0],2)-15 ){
      i=0;
      while (i <= 3) {
          if (attb_get(skill_getAtbself(state->moveset[1][i]), 4) > attb_get(skill_getAtbself(state->moveset[1][max_attack]), 4)) {
              max_attack = i;
          }
          i++;
      }

      if(attb_get(skill_getAtbself(state->moveset[1][max_attack]), 4) != 0){
        return max_attack;
      }
    }


//DEFAULT BEHAVIOUR
      i = 0;
      while (i <= 3) {
          if (attb_get(skill_getAtbatk(state->moveset[1][i]), 1) > attb_get(skill_getAtbatk(state->moveset[1][max_attack]), 1)) {
              max_attack = i;
            }
            i++;
          }




     return max_attack;
}
*/
// int combat_exe(Combat *c) {
//     int i = 0, aux = 1, move = 0;
//
//     if (!c) return -1;
//
//     if (attb_get(c->stats[0], 3) > attb_get(c->stats[1], 3)) {
//         aux = 0;
//     }
//
//
//
//     while (attb_get(c->stats[0], 0) > 0 && attb_get(c->stats[1], 0) > 0) {
//         if ((i + aux) % 2 == 0) {
//             if (c->stunplayer == false) {
//                 fprintf(stdout, "El jugador ataca primero, selecciona una acción:\n");
//                 fprintf(stdout, "Listado de movimientos:\n %s\t %s\n%s\t%s\n", skill_getName(c->moveset[0][0]), skill_getName(c->moveset[0][1]), skill_getName(c->moveset[0][2]), skill_getName(c->moveset[0][3]));
//                 move = player_choice();
//                  if(move != 5){
//                    movement_exe(c, move, 0);
//                  }
//                else{
//                    apply_consumable(c);
//}
//             }
//
//             if (c->stunplayer == true) {
//                 fprintf(stdout, "You have been stunned, meanwhile, you can have some tea.\n");
//                 c->stunplayer = false;
//             }
//
//         } else {
//             if (c->stunenemy == false) {
//                 move = IA_choice(c);
//                 movement_exe(c, move, 1);
//             }
//             if (c->stunenemy == true) {
//                 fprintf(stdout, "THe enemy has been stunned, his damage increased by 100, just joking.\n");
//                 c->stunenemy = false;
//             }
//         }
//         i++;
//     }
//
//     combat_free(c);
//     //decir qn ha muerto y liberar la memoria de la strutura combat
//     return 0;
// }

//REVISADA,COMPILA

bool attack_goes(Combat * c, Skill * skil, int who) {
    if (!c || !skil) {
        printf("FATAL ERROR FUNCTION ATTACK_GOES");
        return false;
    }
    double res;
    double random;
    int p1, p2;
    p1 = attb_get(c->stats[who], 4);
    if (who == 0) {
        p2 = attb_get(c->stats[1], 4);
    } else p2 = attb_get(c->stats[0], 4);
    srand(time(NULL));
    random = (double) rand();
    random /= RAND_MAX;

    res = ((double) p1 / 100) * 7 - ((double) p2 / 100) - (random / 100);

    if (skill_getSpecial(skil) == 2) {
        res += 3;
    }
    if (res < 0) return false;
    if (res >= 0) return true;
}


//REVISADA, COMPILA

void skill_stun(Combat * c, Skill * skil, int who) {
    if (!c || !skil) return;
    if (skill_getSpecial(skil) != 1) return;
    if (who == 0) {
        c->stunenemy = true;
    } else {
        c->stunplayer = true;
    }
}






// int movement_exe(Combat * c, int action, int ent) {
//     int other = 0;
//     int dmgout;
//     Attributes * aux1;
//     Attributes * aux2;
//     int * attr;
//     fprintf(stdout, "%s ataca con %s.\n", c->name[ent], skill_getName(c->moveset[ent][action]));
//     if (ent == 0) {
//         other = 1;
//     }
//     if (attack_goes(c, c->moveset[ent][action], ent) == true) {
//         skill_stun(c, c->moveset[ent][action], ent);
//
//         aux1 = attb_merge(c->stats[ent], skill_getAtbatk(c->moveset[ent][action]));
//
//         if (attb_get(skill_getAtbatk(c->moveset[ent][action]), 1) == 0) {
//             aux2 = attb_merge(c->stats[ent], skill_getAtbself(c->moveset[ent][action]));
//
//             // HOTFIX IN ORDER TO NOT ALLOW OVERHEALING
//             if (other == 1) {
//                 if (attb_get(aux2, 0) > attb_get(entity_getAttributes(c->player), 0)) {
//                     attb_set(aux2, attb_get(entity_getAttributes(c->player), 0), 0);
//                 }
//             } else {
//                 if (attb_get(aux2, 0) > attb_get(entity_getAttributes(c->enemy), 0)) {
//                     attb_set(aux2, attb_get(entity_getAttributes(c->enemy), 0), 0);
//                 }
//             }
//
//
//             attr = attb_getAll(aux2);
//
//             attb_setAll(c->stats[ent], attr);
//             attb_free(aux2);
//         } else {
//             dmgout = attb_get(aux1, 1) - attb_get(c->stats[other], 2);
//             if (dmgout < 0) dmgout = 0;
//             attb_set(c->stats[other], attb_get(c->stats[other], 0) - dmgout, 0);
//         }
//
//         attb_free(aux1);
//     } else fprintf(stdout, "Attack dogded");
//     return 0;
// }

/*void apply_consumable(Combat * c){
  int * attr;
  if(!combat) return;
  attr = attb_getAll(obj_getAttributes(inv_getSelected(entity_getInventory(player), CONSUMABLE)));
  attb_setAll(c->stats[0], attr);

//DISABLE OVERHEALING
  if (attb_get(c->stats[0], 0) > attb_get(entity_getAttributes(c->player), 0)) {

    attb_set(c->stats[0], attb_get(entity_getAttributes(c->player), 0), 0);
    }

  inv_decrease(entity_getInventory(player),obj_getAttributes(inv_getSelected(entity_getInventory(player), CONSUMABLE)),1);

  return;

}*/


void combat_free(Combat* c) {
  if(!c) return;
  attb_free(c->stats[0]);
  attb_free(c->stats[1]);
  entity_free(c->player);
  entity_free(c->enemy);
  for(int i=0; i<4; i++) {
      skill_free(c->moveset[0][i]);
      skill_free(c->moveset[1][i]);
  }
  free(c->name[0]);
  free(c->name[1]);
  disp_free(c->cd);
  free(c);
}
