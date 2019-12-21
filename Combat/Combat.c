/*  Combat.c    */

#include "Combat.h"
#include "Display.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PLAYER_ACTIONS 2 //Fixed window index for player actions
#define PLAYER_INFO 3 //Fixed window index for combat info
#define COMBAT_ROOM 0 //Index of the only room in c->cd. For readability only
#define MAX_ATTACKS 4
#define TOP_STATS 150
#define PLAYER 0 //Readability
#define ENEMY 1 //Readability
#define COMBAT_CONSUMABLE 4 //Readability
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
  c->name[PLAYER] = entity_getName(player);
  c->name[ENEMY] = entity_getName(enemy);
  c->stats[PLAYER] = attb_copy(entity_getAttributes(player));
  c->stats[ENEMY] = attb_copy(entity_getAttributes(enemy));
  c->stunplayer = false;
  c->stunenemy = false;

  c->weapons[PLAYER] = inv_getSelected(entity_getInventory(player), WEAPON);
  c->weapons[ENEMY]= inv_getSelected(entity_getInventory(enemy), WEAPON);


  c->moveset[PLAYER] = obj_getAttacks(c->weapons[0]);
  c->moveset[ENEMY] = obj_getAttacks(c->weapons[1]);

  return c;
}

Combat* combat_execute(Combat* c) {
  if(!c||!c->cd) return NULL;

  int selindex=0;
  while(1) {
      char c=getch1();
      switch(c){
          case 'W': case 'O':
              selindex++;
              break;
          case 'S': case 'L':
              selindex--;
              break;
          case 'J':
              if(!_combat_executeMove(c, selindex) return NULL;
              if(!attb_get(c->stats[PLAYER], HEALTH)||!attb_get(c->stats[ENEMY], HEALTH)) return c;
              break;
          case 'Q': //NOTE: THIS IS ONLY TO BE ABLE TO EXIT MID-COMBAT WHILE DEVELOPING THE GAME. REMOVE THIS WHEN THE DELIVERY IS DUE.
              return c;

      }
      selindex=(selindex+MAX_ATTACKS+1)%(MAX_ATTACKS+1);
      if(!disp_setSelIndex(c->cd, PLAYER_ACTIONS,selindex)) return NULL;
  }
}

Combat* _combat_executeMove(Combat* c, int choice) {
  if(!c||choice>MAX_ATTACKS) return NULL;

  if(choice==COMBAT_CONSUMABLE) {
    _combat_applyConsumable(c, c->player, PLAYER);
    if(!_combat_enemyMove(Combat* c)) return NULL;
    return c;
  }
  /*  Normal turn; no consumable used */

  Skill* ps=c->moveset[PLAYER][choice];
  int p1 = attb_get(c->stats[PLAYER], AGILITY);
  int p2 = attb_get(c->stats[ENEMY], AGILITY);
  srand(time(NULL));
  double random = (double)rand()/RAND_MAX;
  double res=((double)p1/100)*2 - ((double)p2/100) - random;

  if(skill_getSpecial(ps)==NORMAL) res += 0.3;
  if(skill_getSpecial(ps)==UNDODGE) res=1;
  if(res<0) { /*  Your attack is dodged */
    if(attb_get(c->stats[PLAYER], SPEED)<attb_get(c->stats[ENEMY], SPEED)) {
      if(!_combat_enemyMove(Combat* c)) return NULL;
      sleep(2);
      _combat_message(c, "The enemy dodged your attack!");
    } else {
      _combat_message(c, "The enemy dodged your attack!");
      sleep(2);
      if(!_combat_enemyMove(Combat* c)) return NULL;
    }
  } else {  /* Your attack hits */
    if(attb_get(c->stats[PLAYER], SPEED)>attb_get(c->stats[ENEMY], SPEED)) {
      if(!_combat_playerMove(Combat* c, choice)) return NULL;
      sleep(2);
      if(!_combat_enemyMove(Combat* c)) return NULL;
    } else {
      if(!_combat_enemyMove(Combat* c)) return NULL;
      sleep(2);
      if(!_combat_playerMove(Combat* c, choice)) return NULL;
    }
  }
  return c;
}

Combat* _combat_playerMove(Combat* c, int choice) {
  if(!c||choice<0||choice>=MAX_ATTACKS) return NULL;

  if(c->stunplayer) {
    _combat_message(Combat* c, "You are stunned! You cannot move!");
    c->stunplayer=false;
    return c;
  }
  if(skill_getSpecial(c->moveset[PLAYER][choice])==STUNNER) c->stunenemy=true;

  Attributes* self=attb_merge(c->stats[PLAYER], skill_getAtbself(c->moveset[PLAYER][choice]));
  if(!self) return NULL;
  Attributes* attk=attb_merge(c->stats[PLAYER], skill_getAtbatk(c->moveset[PLAYER][choice]));
  if(!attk) {
    attb_free(self);
    return NULL;
  }
  /*  LOWERING ENEMY'S DAMAGE  */
  int attack = attb_get(attk, ATTACK) - attb_get(c->stats[ENEMY], DEFENSE);
  if(attack < 0) attack = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], ATTACK) - attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  SLOWING UP ENEMY  */
  int slowing = attb_get(attk, SPEED) - attb_get(c->stats[ENEMY], DEFENSE);
  if(slowing < 0) slowing = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], SPEED) - slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  MAKING ENEMY LOSE AGILITY */
  int clumnsiness = attb_get(attk, AGILITY) - attb_get(c->stats[ENEMY], DEFENSE);
  if(clumnsiness < 0) clumnsiness = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], AGILITY) - clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  LOWERING ENEMY'S DEFENSE  */
  int defense = attb_get(attk, DEFENSE) - attb_get(c->stats[ENEMY], DEFENSE);
  if(defense < 0) defense = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], DEFENSE) - defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  DAMAGE  */
  int damage = attb_get(attk, HEALTH) - attb_get(c->stats[ENEMY], DEFENSE);
  if(damage < 0) damage = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], HEALTH) - damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  RISING OUR ATTACK  */
  attack = min(attb_get(c->stats[PLAYER], ATTACK) + attb_get(self, ATTACK), attb_get(entity_getAttributes(c->player, ATTACK)));
  if(!attb_set(c->stats[PLAYER], attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR SPEED  */
  slowing = min(attb_get(c->stats[PLAYER], SPEED) + attb_get(self, SPEED), attb_get(entity_getAttributes(c->player, SPEED)));
  if(!attb_set(c->stats[PLAYER], slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR AGILITY  */
  clumnsiness = min(attb_get(c->stats[PLAYER], AGILITY) + attb_get(self, AGILITY), attb_get(entity_getAttributes(c->player, AGILITY)));
  if(!attb_set(c->stats[PLAYER], clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR DEFENSE  */
  defense = min(attb_get(c->stats[PLAYER], DEFENSE) + attb_get(self, DEFENSE), attb_get(entity_getAttributes(c->player, DEFENSE)));
  if(!attb_set(c->stats[PLAYER], defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR HEALTH  */
  damage = min(attb_get(c->stats[PLAYER], HEALTH) + attb_get(self, HEALTH), attb_get(entity_getAttributes(c->player, HEALTH)));
  if(!attb_set(c->stats[PLAYER], damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  attb_free(self);
  attb_free(attk);
  char message[128];
  sprintf(message, "You attacked with %s!", skill_getName(c->moveset[PLAYER][choice]));
  _combat_message(c, message);
  return c;
}

Combat* _combat_executeEnemyMove(Combat *c, int choice) {
  if(!c||choice<0||choice>=MAX_ATTACKS) return NULL;

  if(skill_getSpecial(c->moveset[ENEMY][choice])==STUNNER) c->stunplayer=true;

  Attributes* self=attb_merge(c->stats[ENEMY], skill_getAtbself(c->moveset[ENEMY][choice]));
  if(!self) return NULL;
  Attributes* attk=attb_merge(c->stats[ENEMY], skill_getAtbatk(c->moveset[ENEMY][choice]));
  if(!attk) {
    attb_free(self);
    return NULL;
  }
  /*  LOWERING ENEMY'S DAMAGE  */
  int attack = attb_get(attk, ATTACK) - attb_get(c->stats[PLAYER], DEFENSE);
  if(attack < 0) attack = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], ATTACK) - attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  SLOWING UP ENEMY  */
  int slowing = attb_get(attk, SPEED) - attb_get(c->stats[PLAYER], DEFENSE);
  if(slowing < 0) slowing = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], SPEED) - slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  MAKING ENEMY LOSE AGILITY */
  int clumnsiness = attb_get(attk, AGILITY) - attb_get(c->stats[PLAYER], DEFENSE);
  if(clumnsiness < 0) clumnsiness = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], AGILITY) - clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  LOWERING ENEMY'S DEFENSE  */
  int defense = attb_get(attk, DEFENSE) - attb_get(c->stats[PLAYER], DEFENSE);
  if(defense < 0) defense = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], DEFENSE) - defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  DAMAGE  */
  int damage = attb_get(attk, HEALTH) - attb_get(c->stats[PLAYER], DEFENSE);
  if(damage < 0) damage = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], HEALTH) - damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  RISING OUR ATTACK  */
  attack = min(attb_get(c->stats[ENEMY], ATTACK) + attb_get(self, ATTACK), attb_get(entity_getAttributes(c->enemy, ATTACK)));
  if(!attb_set(c->stats[ENEMY], attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR SPEED  */
  slowing = min(attb_get(c->stats[ENEMY], SPEED) + attb_get(self, SPEED), attb_get(entity_getAttributes(c->enemy, SPEED)));
  if(!attb_set(c->stats[ENEMY], slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR AGILITY  */
  clumnsiness = min(attb_get(c->stats[ENEMY], AGILITY) + attb_get(self, AGILITY), attb_get(entity_getAttributes(c->enemy, AGILITY)));
  if(!attb_set(c->stats[ENEMY], clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR DEFENSE  */
  defense = min(attb_get(c->stats[ENEMY], DEFENSE) + attb_get(self, DEFENSE), attb_get(entity_getAttributes(c->enemy, DEFENSE)));
  if(!attb_set(c->stats[ENEMY], defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  /*  OUR HEALTH  */
  damage = min(attb_get(c->stats[ENEMY], HEALTH) + attb_get(self, HEALTH), attb_get(entity_getAttributes(c->enemy, HEALTH)));
  if(!attb_set(c->stats[ENEMY], damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    return NULL;
  }

  attb_free(self);
  attb_free(attk);
  char message[128];
  sprintf(message, "Your enemy attacked with %s!", skill_getName(c->moveset[ENEMY][choice]));
  _combat_message(c, message);
  return c;
}

Combat* _combat_enemyMove(Combat* c) {
  if(!c) return NULL;
  if(c->stunenemy) {
    _combat_message(Combat* c, "Enemy is stunned! He cannot move!");
    c->stunenemy=false;
    return c;
  }

  int max_attack=0;
  //EMERGENCY HEALING
  if(attb_get(c->stats[ENEMY], HEALTH)<30) {
    for(int i=1; i<4; ++i) {
      if(attb_get(skill_getAtbself(c->moveset[ENEMY][i]), HEALTH)>attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), HEALTH)) {
        max_attack = i;
      }
    }
    if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]),HEALTH)!=0) {
      return _combat_executeEnemyMove(c, max_attack);
    }
  }
  //STAT BOOSTING ATTACK
  max_attack=0;
  if(attb_get(c->stats[ENEMY], ATTACK)<attb_get(c->stats[PLAYER],ATTACK)-20) {
    for(int i=1; i<4; ++i) {
      if(attb_get(skill_getAtbself(c->moveset[ENEMY][i]), ATTACK) > attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), ATTACK)) {
        max_attack = i;
      }
    }
    if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]),ATTACK) != 0){
      return _combat_executeEnemyMove(c, max_attack);
    }
  }
  //STAT BOOSTING DEF
  max_attack=0;
  if(attb_get(c->stats[ENEMY], DEFENSE)<attb_get(c->stats[PLAYER],ATTACK)-25) {
    for(int i=1;i<4;++i) {
      if(attb_get(skill_getAtbself(c->moveset[ENEMY][i]), DEFENSE)>attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), DEFENSE)) {
        max_attack = i;
      }
    }
    if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), DEFENSE) != 0) {
      return _combat_executeEnemyMove(c, max_attack);
    }
  }
  //STAT BOOSTING AGL
  max_attack=0;
  if(attb_get(c->stats[ENEMY], DEFENSE)<attb_get(c->stats[PLAYER],DEFENSE)-15) {
    for(int i=1;i<4;++i) {
      if (attb_get(skill_getAtbself(c->moveset[ENEMY][i]),SPEED) > attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), SPEED)) {
        max_attack = i;
      }
    }
    if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), SPEED) != 0){
      return _combat_executeEnemyMove(c, max_attack);
    }
  }
  //DEFAULT BEHAVIOUR
  max_attack=0;
  for(int i=1;i<4;i++) {
    if(attb_get(skill_getAtbatk(c->moveset[ENEMY][i]), ATTACK) > attb_get(skill_getAtbatk(c->moveset[ENEMY][max_attack]), ATTACK)) {
        max_attack = i;
    }
  }
  return _combat_executeEnemyMove(c, max_attack);
}


void _combat_applyConsumable(Combat* c, Entity* e, int id) {
  if(!c||!e||id>1||id<0) return;
  Attributes* attr = obj_getAttributes(inv_getSelected(entity_getInventory(e), CONSUMABLE)));
  if(!attr) return;
  if(!attb_merge(c->stats[id], attr)) return;

  if(attb_get(c->stats[id], HEALTH)>attb_get(entity_getAttributes(e), HEALTH)) {
    attb_set(c->stats[id], attb_get(entity_getAttributes(e), HEALTH), HEALTH);
  }
  if(attb_get(c->stats[id], ATTACK)>attb_get(entity_getAttributes(e), ATTACK)) {
    attb_set(c->stats[id], attb_get(entity_getAttributes(e), ATTACK), ATTACK);
  }
  if(attb_get(c->stats[id], DEFENSE)>attb_get(entity_getAttributes(e), DEFENSE)) {
    attb_set(c->stats[id], attb_get(entity_getAttributes(e), DEFENSE), DEFENSE);
  }
  if(attb_get(c->stats[id], SPEED)>attb_get(entity_getAttributes(e), SPEED)) {
    attb_set(c->stats[id], attb_get(entity_getAttributes(e), SPEED), SPEED);
  }
  if(attb_get(c->stats[id], AGILITY)>attb_get(entity_getAttributes(e), AGILITY)) {
    attb_set(c->stats[id], attb_get(entity_getAttributes(e), AGILITY), AGILITY);
  }
  attb_free(attr);
  if(id==ENEMY) _combat_message(c, "The enemy used a consumable to rise his stats!");
  else _combat_message(c, "You used your selected consumable to rise your stats!");
  if(!inv_decrease(entity_getInventory(e),obj_getAttributes(inv_getSelected(entity_getInventory(e), CONSUMABLE)), CONSUMABLE)) return NULL;
  return;
}

void _combat_message(Combat* c, char* message) {
  if(!c||!message||!c->cd) return;
  if(!win_remWindowElement(c->cd->latWindow[PLAYER_INFO], 0)) return;
  FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
  Font* f6=font_load(f);
  fclose(f);
  if(!f6) return;
  Welem* we =we_createLabel(message,f6,10);
  if(!we) {
    font_free(f6);
    return;
  }
  if(!win_addWindowElement(c->cd->latWindow[PLAYER_INFO], we)) {
    font_free(f6);
    we_free(we);
    return;
  }
  font_free(f6);
  we_free(we);

  if(!print_Window(c->cd, PLAYER_INFO)) return;
  return;
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
