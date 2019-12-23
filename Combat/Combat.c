/*  Combat.c    */

#include "Combat.h"
#include "Display.h"
#include "Attributes.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PLAYER_STATS 0 //Fixed window index for player stats
#define ENEMY_STATS 1 //Fixed window index for enemy stats
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

/*  PROTOTYPES FOR PRIVATE FUNCTIONS  */
Combat* _combat_executeMove(Combat* c, int choice);
int* _combat_playerMove(Combat* c, int choice);
int* _combat_executeEnemyMove(Combat *c, int choice);
int* _combat_enemyMove(Combat* c);
void _combat_applyConsumable(Combat* c, Entity* e, int id);
void _combat_message(Combat* c, char* message);


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
  Pixel* pb=pix_ini(150, 146, 137,255);
  if(!pb) {
    free(c);
    font_free(f8);
    return NULL;
  }
  Room* cr=room_ini(902, "Combat_room", DISP_HEIGHT, DISP_WIDTH-800, pb);
  if(!cr) {
    free(c);
    pix_free(pb);
    font_free(f8);
    return NULL;
  }

  c->cd = disp_ini(DISP_WIDTH, DISP_HEIGHT, cr, 800, "COMBAT!", f8);
  if(!c->cd) {
    combat_free(c);
    pix_free(pb);
    font_free(f8);
    return NULL;
  }
  pix_free(pb);
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

  c->weapons[PLAYER] = inv_getSelectedCopy(entity_getInventory(player), WEAPON);
  c->weapons[ENEMY]= inv_getSelectedCopy(entity_getInventory(enemy), WEAPON);


  c->moveset[PLAYER] = obj_getAttacks(c->weapons[PLAYER]);
  c->moveset[ENEMY] = obj_getAttacks(c->weapons[ENEMY]);

  return c;
}

Combat* combat_execute(Combat* c) {
  if(!c||!c->cd) return NULL;

  int selindex=0;
  while(1) {
      char ch=getch1();
      switch(ch){
          case 'W': case 'O':
              selindex++;
              break;
          case 'S': case 'L':
              selindex--;
              break;
          case 'J':
              if(!_combat_executeMove(c, selindex)) return NULL;
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
    sleep(2);
    if(!_combat_enemyMove(c)) return NULL;
    sleep(2);
    return c;
  }
  /*  Normal turn; no consumable used */

  Skill* ps=c->moveset[PLAYER][choice];
  int p1 = attb_get(c->stats[PLAYER], AGILITY);
  int p2 = attb_get(c->stats[ENEMY], AGILITY);
  srand(time(NULL));
  double random = (double)rand()/RAND_MAX;
  double res=((double)p1/100)*2 - ((double)p2/100) - random;
  int* e;
  int* p;
  if(skill_getSpecial(ps)==NORMAL) res += 0.3;
  if(skill_getSpecial(ps)==UNDODGE) res=1;
  if(res<0) { /*  Your attack is dodged */
    if(attb_get(c->stats[PLAYER], SPEED)<attb_get(c->stats[ENEMY], SPEED)) {
      e=_combat_enemyMove(c);
      if(!e) return NULL;
      sleep(2);
      _combat_message(c, "The enemy dodged your attack!");
    } else {
      _combat_message(c, "The enemy dodged your attack!");
      sleep(2);
      e=_combat_enemyMove(c);
      if(!e) return NULL;
    }
  } else {  /* Your attack hits */
    if(attb_get(c->stats[PLAYER], SPEED)>attb_get(c->stats[ENEMY], SPEED)) {
      p=_combat_playerMove(c, choice);
      if(!p) return NULL;
      sleep(2);
      e=_combat_enemyMove(c);
      if(!e) return NULL;
    } else {
      e=_combat_enemyMove(c);
      if(!e) return NULL;
      sleep(2);
      p=_combat_playerMove(c, choice);
      if(!p) return NULL;
    }
  }
  sleep(2);
  bool enemyattacks=false;
  bool playerattacks=false;
  for(int i=0;i<10;i++) {
    if(e[i]!=0) enemyattacks=true;
    if(e[i]!=0) playerattacks=true;
  }

  if(playerattacks||enemyattacks) {
    /*  PLAYER STATS  */
    if(!win_clear(disp_getLWindow(c->cd,PLAYER_STATS))) {
      free(p);
      free(e);
      return NULL;
    }
    free(p);
    free(e);
    char ch[5][128];
    sprintf(ch[0], "Health: %d", attb_get(c->stats[PLAYER], HEALTH));
    sprintf(ch[1], "Attack: %d", attb_get(c->stats[PLAYER], ATTACK));
    sprintf(ch[2], "Defense: %d", attb_get(c->stats[PLAYER], DEFENSE));
    sprintf(ch[3], "Speed: %d", attb_get(c->stats[PLAYER], SPEED));
    sprintf(ch[4], "Agility: %d", attb_get(c->stats[PLAYER], AGILITY));
    Welem* pstats[5];
    for(int i=0;i<5;++i) {
        pstats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
        if(!pstats[i]) {
          for(int j=0;j<i;++j)we_free(pstats[j]);
          return NULL;
        }
    }
    /*  ENEMY STATS */
    if(!win_clear(disp_getLWindow(c->cd,ENEMY_STATS))) {
      for(int j=0;j<5;++j)we_free(pstats[j]);
      return NULL;
    }
    sprintf(ch[0], "Health: %d", attb_get(c->stats[ENEMY], HEALTH));
    sprintf(ch[1], "Attack: %d", attb_get(c->stats[ENEMY], ATTACK));
    sprintf(ch[2], "Defense: %d", attb_get(c->stats[ENEMY], DEFENSE));
    sprintf(ch[3], "Speed: %d", attb_get(c->stats[ENEMY], SPEED));
    sprintf(ch[4], "Agility: %d", attb_get(c->stats[ENEMY], AGILITY));
    Welem* estats[5];
    for(int i=0;i<5;++i) {
        estats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
        if(!estats[i]) {
          for(int j=0;j<5;++j)we_free(pstats[j]);
          for(int j=0;j<i;++j)we_free(estats[j]);
          return NULL;
        }
    }
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
  }
  free(p);
  free(e);
  _combat_message(c, "Please select a movement");
  if(!win_setSelected(disp_getLWindow(c->cd, PLAYER_ACTIONS), -1)) {
    return NULL;
  }

  return c;
}

int* _combat_playerMove(Combat* c, int choice) {
  if(!c||choice<0||choice>=MAX_ATTACKS) return NULL;
  int* ret=(int*)calloc(10, sizeof(int));
  for(int i=0;i<10;i++) ret[i]=0;
  if(c->stunplayer) {
    _combat_message(c, "You are stunned! You cannot move!");
    c->stunplayer=false;
    return ret;
  }
  if(skill_getSpecial(c->moveset[PLAYER][choice])==STUNNER) c->stunenemy=true;

  Attributes* self=attb_merge(c->stats[PLAYER], skill_getAtbself(c->moveset[PLAYER][choice]));
  if(!self) return NULL;
  Attributes* attk=attb_merge(c->stats[PLAYER], skill_getAtbatk(c->moveset[PLAYER][choice]));
  if(!attk) {
    attb_free(self);
    free(ret);
    return NULL;
  }
  /*  LOWERING ENEMY'S DAMAGE  */
  int attack = attb_get(attk, ATTACK) - attb_get(c->stats[ENEMY], DEFENSE);
  if(attack < 0) attack = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], ATTACK) - attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[0]=attack;

  /*  SLOWING UP ENEMY  */
  int slowing = attb_get(attk, SPEED) - attb_get(c->stats[ENEMY], DEFENSE);
  if(slowing < 0) slowing = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], SPEED) - slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[1]=slowing;

  /*  MAKING ENEMY LOSE AGILITY */
  int clumnsiness = attb_get(attk, AGILITY) - attb_get(c->stats[ENEMY], DEFENSE);
  if(clumnsiness < 0) clumnsiness = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], AGILITY) - clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[2]=clumnsiness;

  /*  LOWERING ENEMY'S DEFENSE  */
  int defense = attb_get(attk, DEFENSE) - attb_get(c->stats[ENEMY], DEFENSE);
  if(defense < 0) defense = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], DEFENSE) - defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[3]=defense;

  /*  DAMAGE  */
  int damage = attb_get(attk, HEALTH) - attb_get(c->stats[ENEMY], DEFENSE);
  if(damage < 0) damage = 0;
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], HEALTH) - damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[4]=damage;

  /*  RISING OUR ATTACK  */
  attack = min(attb_get(c->stats[PLAYER], ATTACK) + attb_get(self, ATTACK), attb_get(entity_getAttributes(c->player), ATTACK));
  if(!attb_set(c->stats[PLAYER], attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[5]=attack;

  /*  OUR SPEED  */
  slowing = min(attb_get(c->stats[PLAYER], SPEED) + attb_get(self, SPEED), attb_get(entity_getAttributes(c->player), SPEED));
  if(!attb_set(c->stats[PLAYER], slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[6]=slowing;

  /*  OUR AGILITY  */
  clumnsiness = min(attb_get(c->stats[PLAYER], AGILITY) + attb_get(self, AGILITY), attb_get(entity_getAttributes(c->player), AGILITY));
  if(!attb_set(c->stats[PLAYER], clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[7]=clumnsiness;

  /*  OUR DEFENSE  */
  defense = min(attb_get(c->stats[PLAYER], DEFENSE) + attb_get(self, DEFENSE), attb_get(entity_getAttributes(c->player), DEFENSE));
  if(!attb_set(c->stats[PLAYER], defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[8]=defense;

  /*  OUR HEALTH  */
  damage = min(attb_get(c->stats[PLAYER], HEALTH) + attb_get(self, HEALTH), attb_get(entity_getAttributes(c->player), HEALTH));
  if(!attb_set(c->stats[PLAYER], damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[9]=damage;

  attb_free(self);
  attb_free(attk);
  char message[128];
  sprintf(message, "You attacked with %s!", skill_getName(c->moveset[PLAYER][choice]));
  _combat_message(c, message);
  return ret;
}

int* _combat_executeEnemyMove(Combat *c, int choice) {
  if(!c||choice<0||choice>=MAX_ATTACKS) return NULL;

  if(skill_getSpecial(c->moveset[ENEMY][choice])==STUNNER) c->stunplayer=true;

  Attributes* self=attb_merge(c->stats[ENEMY], skill_getAtbself(c->moveset[ENEMY][choice]));
  if(!self) return NULL;
  Attributes* attk=attb_merge(c->stats[ENEMY], skill_getAtbatk(c->moveset[ENEMY][choice]));
  if(!attk) {
    attb_free(self);
    return NULL;
  }
  int* ret=(int*)calloc(10, sizeof(int));
  for(int i=0;i<10;++i) ret[i]=0;

  /*  LOWERING ENEMY'S DAMAGE  */
  int attack = attb_get(attk, ATTACK) - attb_get(c->stats[PLAYER], DEFENSE);
  if(attack < 0) attack = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], ATTACK) - attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[0]=attack;

  /*  SLOWING UP ENEMY  */
  int slowing = attb_get(attk, SPEED) - attb_get(c->stats[PLAYER], DEFENSE);
  if(slowing < 0) slowing = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], SPEED) - slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[1]=slowing;

  /*  MAKING ENEMY LOSE AGILITY */
  int clumnsiness = attb_get(attk, AGILITY) - attb_get(c->stats[PLAYER], DEFENSE);
  if(clumnsiness < 0) clumnsiness = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], AGILITY) - clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[2]=clumnsiness;

  /*  LOWERING ENEMY'S DEFENSE  */
  int defense = attb_get(attk, DEFENSE) - attb_get(c->stats[PLAYER], DEFENSE);
  if(defense < 0) defense = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], DEFENSE) - defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[3]=defense;

  /*  DAMAGE  */
  int damage = attb_get(attk, HEALTH) - attb_get(c->stats[PLAYER], DEFENSE);
  if(damage < 0) damage = 0;
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], HEALTH) - damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[4]=damage;

  /*  RISING OUR ATTACK  */
  attack = min(attb_get(c->stats[ENEMY], ATTACK) + attb_get(self, ATTACK), attb_get(entity_getAttributes(c->enemy), ATTACK));
  if(!attb_set(c->stats[ENEMY], attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[5]=attack;

  /*  OUR SPEED  */
  slowing = min(attb_get(c->stats[ENEMY], SPEED) + attb_get(self, SPEED), attb_get(entity_getAttributes(c->enemy), SPEED));
  if(!attb_set(c->stats[ENEMY], slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[6]=slowing;

  /*  OUR AGILITY  */
  clumnsiness = min(attb_get(c->stats[ENEMY], AGILITY) + attb_get(self, AGILITY), attb_get(entity_getAttributes(c->enemy), AGILITY));
  if(!attb_set(c->stats[ENEMY], clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[7]=clumnsiness;

  /*  OUR DEFENSE  */
  defense = min(attb_get(c->stats[ENEMY], DEFENSE) + attb_get(self, DEFENSE), attb_get(entity_getAttributes(c->enemy), DEFENSE));
  if(!attb_set(c->stats[ENEMY], defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[8]=defense;

  /*  OUR HEALTH  */
  damage = min(attb_get(c->stats[ENEMY], HEALTH) + attb_get(self, HEALTH), attb_get(entity_getAttributes(c->enemy), HEALTH));
  if(!attb_set(c->stats[ENEMY], damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[9]=damage;

  attb_free(self);
  attb_free(attk);
  char message[128];
  sprintf(message, "Your enemy attacked with %s!", skill_getName(c->moveset[ENEMY][choice]));
  _combat_message(c, message);
  return ret;
}

int* _combat_enemyMove(Combat* c) {
  if(!c) return NULL;
  int* ret=(int*)calloc(10, sizeof(int));
  for(int i=0;i<10;++i) ret[i]=0;
  if(c->stunenemy) {
    _combat_message(c, "Enemy is stunned! He cannot move!");
    c->stunenemy=false;
    return ret;
  }
  free(ret);

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
  Attributes* attr = obj_getAttributes(inv_getSelected(entity_getInventory(e), CONSUMABLE));
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
  if(!inv_decrease(entity_getInventory(e),obj_getAttributes(inv_getSelected(entity_getInventory(e), CONSUMABLE)), CONSUMABLE)) return;
  return;
}

void _combat_message(Combat* c, char* message) {
  if(!c||!message||!c->cd) return;
  if(!win_remWindowElement(disp_getLWindow(c->cd,PLAYER_INFO), 0)) return;
  FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
  Font* f6=font_load(f);
  fclose(f);
  if(!f6) return;
  Welem* we =we_createLabel(message,f6,10);
  if(!we) {
    font_free(f6);
    return;
  }
  if(!win_addWindowElement(disp_getLWindow(c->cd,PLAYER_INFO), we)) {
    font_free(f6);
    we_free(we);
    return;
  }
  font_free(f6);
  we_free(we);

  if(!print_Window(c->cd, PLAYER_INFO)) return;
  return;
}

Combat* combat_load(Combat*c) {
  if(!c||!c->cd||!c->player||!c->enemy) return NULL;

  if(disp_getNLatWindow(c->cd)>0) {
    for(int i=disp_getNLatWindow(c->cd)-1; i>=0; --i) {
      if(!disp_RemLwindow(c->cd, i)) return NULL;
    }
  }
  Welem* pstats[5];
  Welem* estats[5];
  Welem* movs[5];
  char ch[5][128];
  sprintf(ch[0], "Health: %d", attb_get(c->stats[PLAYER], HEALTH));
  sprintf(ch[1], "Attack: %d", attb_get(c->stats[PLAYER], ATTACK));
  sprintf(ch[2], "Defense: %d", attb_get(c->stats[PLAYER], DEFENSE));
  sprintf(ch[3], "Speed: %d", attb_get(c->stats[PLAYER], SPEED));
  sprintf(ch[4], "Agility: %d", attb_get(c->stats[PLAYER], AGILITY));
  for(int i=0;i<5;++i) {
      pstats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
      if(!pstats[i]) {
        for(int j=0;j<i;++j)we_free(pstats[j]);
        return NULL;
      }
  }

  sprintf(ch[0], "Health: %d", attb_get(c->stats[ENEMY], HEALTH));
  sprintf(ch[1], "Attack: %d", attb_get(c->stats[ENEMY], ATTACK));
  sprintf(ch[2], "Defense: %d", attb_get(c->stats[ENEMY], DEFENSE));
  sprintf(ch[3], "Speed: %d", attb_get(c->stats[ENEMY], SPEED));
  sprintf(ch[4], "Agility: %d", attb_get(c->stats[ENEMY], AGILITY));
  for(int i=0;i<5;++i) {
      estats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
      if(!estats[i]) {
        for(int j=0;j<5;++j)we_free(pstats[j]);
        for(int j=0;j<i;++j)we_free(estats[j]);
        return NULL;
      }
  }

  for(int i=0;i<4;++i) {
      movs[i]=we_createLabel(skill_getName(c->moveset[PLAYER][i]),fcat_lookup(M6),0);
      if(!movs[i]) {
        for(int j=0;j<5;++j)we_free(pstats[j]);
        for(int j=0;j<5;++j)we_free(estats[j]);
        for(int j=0;j<i;++j)we_free(movs[j]);
        return NULL;
      }
  }
  movs[4]=we_createLabel(obj_getName(inv_getSelected(entity_getInventory(c->player), CONSUMABLE)), fcat_lookup(M6),0);
  if(!movs[4]) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<4;++j)we_free(movs[j]);
    return NULL;
  }
  int* y=disp_getDimensions(c->cd);
  Window* winplayer=win_ini("Your stats",pstats,5,y[0]-y[2]-1,y[2]/4-20,0,0,fcat_lookup(M8));
  if(!winplayer) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    free(y);
    return NULL;
  }

  Window* winenemy=win_ini("Enemy's stats",estats,5,y[0]-y[2]-1,y[2]/4-20,y[2]/4-20,0,fcat_lookup(M8));
  if(!winenemy) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    win_free(winplayer);
    free(y);
    return NULL;
  }
  Window* winoptions=win_ini("Movements",movs,5,y[0]-y[2]-1,y[2]/4-20, y[2]/2-40,0,fcat_lookup(M8));
  if(!winoptions) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    win_free(winplayer);
    win_free(winenemy);
    free(y);
    return NULL;
  }
  Window* wininfo=win_ini("State of the combat",NULL,0,y[0]-y[2]-1,y[2]/4-20,y[2]-80,0,fcat_lookup(M8));
  if(!wininfo) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    win_free(winplayer);
    win_free(winenemy);
    win_free(winoptions);
    free(y);
    return NULL;
  }

  if(!disp_AddLWindow(c->cd, winplayer)) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    win_free(winplayer);
    win_free(winenemy);
    win_free(winoptions);
    free(y);
    return NULL;
  }
  if(!disp_AddLWindow(c->cd, winenemy)) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    win_free(winplayer);
    win_free(winenemy);
    win_free(winoptions);
    return NULL;
  }
  if(!disp_AddLWindow(c->cd, winoptions)) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    win_free(winplayer);
    win_free(winenemy);
    win_free(winoptions);
    free(y);
    return NULL;
  }
  if(!disp_AddLWindow(c->cd, wininfo)) {
    for(int j=0;j<5;++j)we_free(pstats[j]);
    for(int j=0;j<5;++j)we_free(estats[j]);
    for(int j=0;j<5;++j)we_free(movs[j]);
    win_free(winplayer);
    win_free(winenemy);
    win_free(winoptions);
    free(y);
    return NULL;
  }

  Sprite* ps=entity_getSprite(c->player);
  if(!ps) return NULL;
  spr_setCoordinates(ps, 200,200);
  Sprite* es=entity_getSprite(c->enemy);
  if(!es) return NULL;
  spr_setCoordinates(es, 50,600);
  if(!room_addBSprite(disp_getrefRoom(c->cd), ps)) return NULL;
  if(!room_addBSprite(disp_getrefRoom(c->cd), es)) return NULL;
  Canvas* d=disp_Render(c->cd);
  canv_print(stdout,d,0,0);
  canv_free(d);
  for(int j=0;j<5;++j)we_free(pstats[j]);
  for(int j=0;j<5;++j)we_free(estats[j]);
  for(int j=0;j<5;++j)we_free(movs[j]);
  win_free(winplayer);
  win_free(winenemy);
  win_free(winoptions);
  free(y);
  return c;
}


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
