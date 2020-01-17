/*  Combat.c    */

#include "Combat.h"
#include "Display.h"
#include "Attributes.h"
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
void _combat_info(Combat* c, int index);


/**
 * @brief Starts and executes a combat
 *
 * @param player Player entity
 * @param enemy  Enemy entity
 * @return  -1 if there has been any error
 *           0 if the combat was won by the player
 *           1 if the combat was won by the enemy
 */
int combat_launch(Entity* player, Entity* enemy){
  if(!player||!enemy)return -1;
  int retval=-1;
  Combat* c=combat_ini(player, enemy);
  if(!combat_load(c)) return -1;
  combat_execute(c);
  if(attb_get(c->stats[ENEMY], HEALTH)<0)retval=0;
  if(attb_get(c->stats[PLAYER], HEALTH)<0)retval=1;
  combat_free(c);

  return retval;
}


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
  Room* cr=room_ini(902, "Combat_room", DISP_HEIGHT, DISP_WIDTH, pb);
  if(!cr) {
    free(c);
    pix_free(pb);
    font_free(f8);
    return NULL;
  }

  c->cd = disp_ini(DISP_WIDTH, DISP_HEIGHT, cr, 800, "COMBAT!", f8);
  room_free(cr);
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
  /*if(!entity_addtoDisplay(c->player, c->cd)) {
    combat_free(c);
    font_free(f8);
    return NULL;
  }*/
  c->enemy = entity_copy(enemy);
  if(!c->enemy) {
    combat_free(c);
    return NULL;
  }
  /*if(!entity_addtoDisplay(c->enemy, c->cd)) {
    combat_free(c);
    font_free(f8);
    return NULL;
  }*/
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
              selindex--;
              break;
          case 'S': case 'L':
              selindex++;
              break;
          case 'J':
              if(!_combat_executeMove(c, selindex)) return NULL;
              if(attb_get(c->stats[PLAYER], HEALTH)<=0||attb_get(c->stats[ENEMY], HEALTH)<=0) {
                FILE* pad=fopen("pad", "w");
                fprintf(pad, "%d %d\n", attb_get(c->stats[PLAYER], HEALTH), attb_get(c->stats[ENEMY], HEALTH));
                _combat_message(c, "1... 2... 3... This combat is over!");
                return c;
              }
              break;
          case 'Q': //NOTE: THIS IS ONLY TO BE ABLE TO EXIT MID-COMBAT WHILE DEVELOPING THE GAME. REMOVE THIS WHEN THE DELIVERY IS DUE.
              return c;

      }
      selindex=(selindex+OBJ_MAX_ATTACKS+1)%(OBJ_MAX_ATTACKS+1);
      if(!disp_setSelIndex(c->cd, PLAYER_ACTIONS,selindex)) return NULL;
      _combat_info(c, selindex);

  }
}

Combat* _combat_executeMove(Combat* c, int choice) {
  if(!c||choice>OBJ_MAX_ATTACKS) return NULL;

  if(choice==COMBAT_CONSUMABLE) {
    _combat_applyConsumable(c, c->player, PLAYER);
    sleep(2);
    if(!_combat_enemyMove(c)) return NULL;
    sleep(2);
    if(!win_clear(disp_getLWindow(c->cd,PLAYER_STATS))) return NULL;

    char ch[ATT_NUMBER][128];
    sprintf(ch[0], "Health: %d", attb_get(c->stats[PLAYER], HEALTH));
    sprintf(ch[1], "Attack: %d", attb_get(c->stats[PLAYER], ATTACK));
    sprintf(ch[2], "Defense: %d", attb_get(c->stats[PLAYER], DEFENSE));
    sprintf(ch[3], "Speed: %d", attb_get(c->stats[PLAYER], SPEED));
    sprintf(ch[4], "Agility: %d", attb_get(c->stats[PLAYER], AGILITY));
    Welem* pstats[ATT_NUMBER];
    for(int i=0;i<ATT_NUMBER;++i) {
        pstats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
        if(!pstats[i]) {
          for(int j=0;j<i;++j)we_free(pstats[j]);
          return NULL;
        }
    }
    for(int i=0;i<ATT_NUMBER;++i) {
      if(!win_addWindowElement(disp_getLWindow(c->cd,PLAYER_STATS), pstats[i])) {
        for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
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
    Welem* estats[ATT_NUMBER];
    for(int i=0;i<ATT_NUMBER;++i) {
        estats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
        if(!estats[i]) {
          for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
          for(int j=0;j<i;++j)we_free(estats[j]);
          return NULL;
        }
    }
    for(int i=0;i<ATT_NUMBER;++i) {
      if(!win_addWindowElement(disp_getLWindow(c->cd,ENEMY_STATS), estats[i])) {
        for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
        for(int j=0;j<ATT_NUMBER;++j)we_free(estats[j]);
        return NULL;
      }
    }
    for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
    for(int j=0;j<ATT_NUMBER;++j)we_free(estats[j]);

    _combat_message(c, "Please select a movement");
    if(!win_setSelected(disp_getLWindow(c->cd, PLAYER_ACTIONS), -1)) {
      return NULL;
    }
    print_Window(c->cd, PLAYER_STATS);
    print_Window(c->cd, ENEMY_STATS);
    return c;
  }
  /*  Normal turn; no consumable used */

  Skill* ps=c->moveset[PLAYER][choice];
  int p1 = attb_get(c->stats[PLAYER], AGILITY);
  int p2 = attb_get(c->stats[ENEMY], AGILITY);
  srand(time(NULL));
  double random = (double)rand()/RAND_MAX;
  double res=((double)p1/100)*2.5 - ((double)p2/100) - random;
  int* e=NULL;
  int* p=NULL;
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
    char ch[ATT_NUMBER][128];
    sprintf(ch[0], "Health: %d", attb_get(c->stats[PLAYER], HEALTH));
    sprintf(ch[1], "Attack: %d", attb_get(c->stats[PLAYER], ATTACK));
    sprintf(ch[2], "Defense: %d", attb_get(c->stats[PLAYER], DEFENSE));
    sprintf(ch[3], "Speed: %d", attb_get(c->stats[PLAYER], SPEED));
    sprintf(ch[4], "Agility: %d", attb_get(c->stats[PLAYER], AGILITY));
    Welem* pstats[ATT_NUMBER];
    for(int i=0;i<ATT_NUMBER;++i) {
        pstats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
        if(!pstats[i]) {
          for(int j=0;j<i;++j)we_free(pstats[j]);
          return NULL;
        }
    }
    for(int i=0;i<ATT_NUMBER;++i) {
      if(!win_addWindowElement(disp_getLWindow(c->cd,PLAYER_STATS), pstats[i])) {
        for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
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
    Welem* estats[ATT_NUMBER];
    for(int i=0;i<ATT_NUMBER;++i) {
        estats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
        if(!estats[i]) {
          for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
          for(int j=0;j<i;++j)we_free(estats[j]);
          return NULL;
        }
    }
    for(int i=0;i<ATT_NUMBER;++i) {
      if(!win_addWindowElement(disp_getLWindow(c->cd,ENEMY_STATS), estats[i])) {
        for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
        for(int j=0;j<ATT_NUMBER;++j)we_free(estats[j]);
        return NULL;
      }
    }
    for(int j=0;j<ATT_NUMBER;++j)we_free(pstats[j]);
    for(int j=0;j<ATT_NUMBER;++j)we_free(estats[j]);
  }

  _combat_message(c, "Please select a movement");
  if(!win_setSelected(disp_getLWindow(c->cd, PLAYER_ACTIONS), -1)) {
    return NULL;
  }
  print_Window(c->cd, PLAYER_STATS);
  print_Window(c->cd, ENEMY_STATS);
  return c;
}

int* _combat_playerMove(Combat* c, int choice) {
  if(!c||choice<0||choice>=OBJ_MAX_ATTACKS) return NULL;
  int* ret=(int*)calloc(10, sizeof(int));
  for(int i=0;i<10;i++) ret[i]=0;
  if(c->stunplayer) {
    _combat_message(c, "You are stunned! You cannot move!");
    c->stunplayer=false;
    return ret;
  }
  if(skill_getSpecial(c->moveset[PLAYER][choice])==STUNNER) c->stunenemy=true;

  Attributes* self=/*attb_merge(c->stats[PLAYER],*/ skill_getAtbself(c->moveset[PLAYER][choice])/*)*/;
  if(!self) return NULL;
  Attributes* attk=/*attb_merge(c->stats[PLAYER],*/ skill_getAtbatk(c->moveset[PLAYER][choice])/*)*/;
  if(!attk) {
    attb_free(self);
    free(ret);
    return NULL;
  }
  /*  LOWERING ENEMY'S DAMAGE  */
  FILE* pa=fopen("pa", "w");
  int attack = attb_get(c->stats[PLAYER], ATTACK)*0.2- attb_get(c->stats[ENEMY], DEFENSE);
  if(attack < 0) attack = attb_get(attk, ATTACK);
  else attack+=attb_get(attk, ATTACK);
  if(attb_get(attk, ATTACK)==0) attack=0;
  if(attb_get(c->stats[ENEMY], ATTACK) - attack<0) attack=attb_get(c->stats[ENEMY], ATTACK);
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], ATTACK) - attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[0]=attack;
  fprintf(pa, "%d\n", attack);
  /*  SLOWING UP ENEMY  */
  int slowing = attb_get(c->stats[PLAYER], ATTACK)*0.2 - attb_get(c->stats[ENEMY], DEFENSE);
  if(slowing < 0) slowing = attb_get(attk, SPEED);
  else slowing+=attb_get(attk, SPEED);
  if(attb_get(attk, SPEED)==0) slowing=0;
  if(attb_get(c->stats[ENEMY], SPEED) - slowing<0) slowing=attb_get(c->stats[ENEMY], SPEED);
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], SPEED) - slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[1]=slowing;

  /*  MAKING ENEMY LOSE AGILITY */
  int clumnsiness = attb_get(c->stats[PLAYER], ATTACK)*0.2 - attb_get(c->stats[ENEMY], DEFENSE);
  if(clumnsiness < 0) clumnsiness = attb_get(attk, AGILITY);
  else clumnsiness+=attb_get(attk, AGILITY);
  if(attb_get(attk, AGILITY)==0) clumnsiness=0;
  if(attb_get(c->stats[ENEMY], AGILITY) - clumnsiness<0) clumnsiness=attb_get(c->stats[ENEMY], AGILITY);
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], AGILITY) - clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[2]=clumnsiness;

  /*  LOWERING ENEMY'S DEFENSE  */
  int defense = attb_get(c->stats[PLAYER], ATTACK)*0.2 - attb_get(c->stats[ENEMY], DEFENSE);
  if(defense < 0) defense = attb_get(attk, DEFENSE);
  else defense=attb_get(attk, DEFENSE);
  if(attb_get(attk, DEFENSE)==0) defense=0;
  if(attb_get(c->stats[ENEMY], DEFENSE) - defense<0) defense=attb_get(c->stats[ENEMY], DEFENSE);
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], DEFENSE) - defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[3]=defense;

  /*  DAMAGE  */
  int damage = attb_get(c->stats[PLAYER], ATTACK)*0.5 + attb_get(c->stats[PLAYER], ATTACK)*0.2 - attb_get(c->stats[ENEMY], DEFENSE);
  // FILE* g=fopen("tty2", "w");
  // fprintf(g, "%d\n%d", damage, attb_get(c->stats[ENEMY], HEALTH));
  if(damage < 0) damage = attb_get(attk, HEALTH);
  else damage+=attb_get(attk, HEALTH);
  if(attb_get(attk, HEALTH)==0) damage=0;
  if(attb_get(c->stats[ENEMY], HEALTH) - damage<0) damage=attb_get(c->stats[ENEMY], HEALTH);
  if(!attb_set(c->stats[ENEMY], attb_get(c->stats[ENEMY], HEALTH) - damage, HEALTH)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[4]=damage;

  /*  RISING OUR ATTACK  */
  attack = min(attb_get(c->stats[PLAYER], ATTACK) + attb_get(self, ATTACK), attb_get(entity_getAttributes(c->player), ATTACK));
  if(attack<0) attack=0;
  if(!attb_set(c->stats[PLAYER], attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[5]=attack;

  /*  OUR SPEED  */
  slowing = min(attb_get(c->stats[PLAYER], SPEED) + attb_get(self, SPEED), attb_get(entity_getAttributes(c->player), SPEED));
  if(slowing<0) slowing=0;
  if(!attb_set(c->stats[PLAYER], slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[6]=slowing;

  /*  OUR AGILITY  */
  clumnsiness = min(attb_get(c->stats[PLAYER], AGILITY) + attb_get(self, AGILITY), attb_get(entity_getAttributes(c->player), AGILITY));
  if(clumnsiness<0) clumnsiness=0;
  if(!attb_set(c->stats[PLAYER], clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[7]=clumnsiness;

  /*  OUR DEFENSE  */
  defense = min(attb_get(c->stats[PLAYER], DEFENSE) + attb_get(self, DEFENSE), attb_get(entity_getAttributes(c->player), DEFENSE));
  if(defense<0) defense=0;
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
  // fprintf(g, "\n%d", attb_get(self, HEALTH));
  // fclose(g);
  fclose(pa);
  attb_free(self);
  attb_free(attk);
  char message[128];
  sprintf(message, "You attacked with %s!", skill_getName(c->moveset[PLAYER][choice]));
  _combat_message(c, message);
  return ret;
}

int* _combat_executeEnemyMove(Combat *c, int choice) {
  if(!c||choice<0||choice>=OBJ_MAX_ATTACKS) return NULL;

  if(skill_getSpecial(c->moveset[ENEMY][choice])==STUNNER) c->stunplayer=true;

  Attributes* self=/*attb_merge(c->stats[ENEMY],*/ skill_getAtbself(c->moveset[ENEMY][choice])/*)*/;
  if(!self) return NULL;
  Attributes* attk=/*attb_merge(c->stats[ENEMY],*/ skill_getAtbatk(c->moveset[ENEMY][choice])/*)*/;
  if(!attk) {
    attb_free(self);
    return NULL;
  }
  int* ret=(int*)calloc(10, sizeof(int));
  for(int i=0;i<10;++i) ret[i]=0;

  /*  LOWERING ENEMY'S DAMAGE  */
  int attack =  attb_get(c->stats[ENEMY], ATTACK)*0.2 - attb_get(c->stats[PLAYER], DEFENSE);
  if(attack < 0) attack = attb_get(attk, ATTACK);
  else attack+=attb_get(attk, ATTACK);
  if(attb_get(attk, ATTACK)==0) attack=0;
  if(attb_get(c->stats[PLAYER], ATTACK) - attack<0) attack=attb_get(c->stats[PLAYER], ATTACK);
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], ATTACK) - attack, ATTACK)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[0]=attack;

  /*  SLOWING UP ENEMY  */
  int slowing =  attb_get(c->stats[ENEMY], ATTACK)*0.2 - attb_get(c->stats[PLAYER], DEFENSE);
  if(slowing < 0) slowing = attb_get(attk, SPEED);
  else slowing+=attb_get(attk, SPEED);
  if(attb_get(attk, SPEED)==0) slowing=0;
  if(attb_get(c->stats[PLAYER], SPEED) - slowing<0) slowing=attb_get(c->stats[PLAYER], SPEED);
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], SPEED) - slowing, SPEED)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[1]=slowing;

  /*  MAKING ENEMY LOSE AGILITY */
  int clumnsiness =  attb_get(c->stats[ENEMY], ATTACK)*0.2 - attb_get(c->stats[PLAYER], DEFENSE);
  if(clumnsiness < 0) clumnsiness = attb_get(attk, AGILITY);
  else clumnsiness+=attb_get(attk, AGILITY);
  if(attb_get(attk, AGILITY)==0) clumnsiness=0;
  if(attb_get(c->stats[PLAYER], AGILITY) - clumnsiness<0) clumnsiness=attb_get(c->stats[PLAYER], AGILITY);
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], AGILITY) - clumnsiness, AGILITY)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[2]=clumnsiness;

  /*  LOWERING ENEMY'S DEFENSE  */
  int defense = attb_get(c->stats[ENEMY], ATTACK)*0.2 - attb_get(c->stats[PLAYER], DEFENSE);
  if(defense < 0) defense = attb_get(attk, DEFENSE);
  else defense=attb_get(attk, DEFENSE);
  if(attb_get(attk, DEFENSE)==0) defense=0;
  if(attb_get(c->stats[PLAYER], DEFENSE) - defense<0) defense=attb_get(c->stats[PLAYER], DEFENSE);
  if(!attb_set(c->stats[PLAYER], attb_get(c->stats[PLAYER], DEFENSE) - defense, DEFENSE)) {
    attb_free(self);
    attb_free(attk);
    free(ret);
    return NULL;
  }
  ret[3]=defense;

  /*  DAMAGE  */
  int damage = attb_get(c->stats[ENEMY], ATTACK)*0.5- attb_get(c->stats[PLAYER], DEFENSE);
  // FILE* g=fopen("tty", "w");
  // fprintf(g, "%d\n%d", damage, attb_get(c->stats[PLAYER], HEALTH));
  if(damage < 0) damage = attb_get(attk, HEALTH)-attb_get(c->stats[PLAYER], DEFENSE)*0.5;
  else damage+=attb_get(attk, HEALTH);
  if(attb_get(attk, HEALTH)==0) damage=0;
  if(attb_get(c->stats[PLAYER], HEALTH) - damage<0) damage=attb_get(c->stats[PLAYER], HEALTH);
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
  // fprintf(g, "\n%d", damage);
  // fclose(g);
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
  if(attb_get(c->stats[ENEMY], HEALTH)<26) {
    for(int i=0; i<4; ++i) {
      if(attb_get(skill_getAtbself(c->moveset[ENEMY][i]), HEALTH)>attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), HEALTH)) {
        max_attack = i;
      }
    }
    if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]),HEALTH)!=0) {
      return _combat_executeEnemyMove(c, max_attack);
    }
  }

//   //STUN TACTICS
//   max_attack=0;
//   if(attb_get(c->stats[ENEMY], ATTACK)<attb_get(c->stats[PLAYER],ATTACK)-35){
//   for(int i=0;i<4;i++) {
//     if(attb_get(skill_getAtbatk(c->moveset[ENEMY][i]), ATTACK) > attb_get(skill_getAtbatk(c->moveset[ENEMY][max_attack]), ATTACK) && skill_getSpecial(c->moveset[ENEMY][i]) == STUNNER) {
//         max_attack = i;
//     }
//   }
//   if(skill_getSpecial(c->moveset[ENEMY][max_attack]) == STUNNER){
//     return _combat_executeEnemyMove(c, max_attack);
//   }
// }
//   //STAT BOOSTING ATTACK
//   max_attack=0;
//   if(attb_get(c->stats[ENEMY], ATTACK)<attb_get(c->stats[PLAYER],ATTACK)-20) {
//     for(int i=0; i<4; ++i) {
//       if(attb_get(skill_getAtbself(c->moveset[ENEMY][i]), ATTACK) > attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), ATTACK)) {
//         max_attack = i;
//       }
//     }
//     if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]),ATTACK) != 0){
//       return _combat_executeEnemyMove(c, max_attack);
//     }
//   }
//   //STAT BOOSTING DEF
//   max_attack=0;
//   if(attb_get(c->stats[ENEMY], DEFENSE)<attb_get(c->stats[PLAYER],ATTACK)-25) {
//     for(int i=0;i<4;++i) {
//       if(attb_get(skill_getAtbself(c->moveset[ENEMY][i]), DEFENSE)>attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), DEFENSE)) {
//         max_attack = i;
//       }
//     }
//     if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), DEFENSE) != 0) {
//       return _combat_executeEnemyMove(c, max_attack);
//     }
//   }
//   //STAT BOOSTING AGL
//   max_attack=0;
//   if(attb_get(c->stats[ENEMY], DEFENSE)<attb_get(c->stats[PLAYER],DEFENSE)-15) {
//     for(int i=0;i<4;++i) {
//       if (attb_get(skill_getAtbself(c->moveset[ENEMY][i]),SPEED) > attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), SPEED)) {
//         max_attack = i;
//       }
//     }
//     if(attb_get(skill_getAtbself(c->moveset[ENEMY][max_attack]), SPEED) != 0){
//       return _combat_executeEnemyMove(c, max_attack);
//     }
//   }
  //DEFAULT BEHAVIOUR
  max_attack=0;
  FILE* aa=fopen("aa", "w");
  for(int i=0;i<4;i++) {
    fprintf(aa, "%d %d \n", attb_get(skill_getAtbatk(c->moveset[ENEMY][i]), HEALTH), attb_get(skill_getAtbatk(c->moveset[ENEMY][max_attack]), HEALTH));
    if(attb_get(skill_getAtbatk(c->moveset[ENEMY][i]), HEALTH) > attb_get(skill_getAtbatk(c->moveset[ENEMY][max_attack]), HEALTH)) {
        max_attack = i;
    }
  }
  srand(time(NULL));
  fprintf(aa, "%d\n", max_attack);
  double random = (double)rand();
  if((int)random%2) {
    while(random>=MAX_ATTACKS) random/=2;
    max_attack=random;
  }
  fprintf(aa, "%f\n", random);
  fclose(aa);
  return _combat_executeEnemyMove(c, max_attack);
}


void _combat_applyConsumable(Combat* c, Entity* e, int id) {
  if(!c||!e||id>1||id<0) return;

Inventory* in =entity_getInvRef(e);
  disp_InventoryWindow(c->cd,in,fcat_lookup(M8),fcat_lookup(M6),fcat_lookup(M4),fcat_lookup(M6));
  FILE* wod=fopen("wod", "w");
  fprintf(wod, "back\n");
  fclose(wod);

   Attributes* attr = obj_getAttributes(inv_getSelected(entity_getInventory(e), CONSUMABLE));

   if(!attr) return;
   if(!attb_merge(c->stats[id], attr)) return;
  
  int testing = attb_get(attr , 0);
  if(testing == 20) _combat_message(c, "UPSSS");


   //if(attb_get(c->stats[id], HEALTH)>attb_get(entity_getAttributes(e), HEALTH)) {
   //  attb_set(c->stats[id], attb_get(entity_getAttributes(e), HEALTH), HEALTH);
   //}
   attb_free(attr);
   if(id==ENEMY) _combat_message(c, "The enemy used a consumable to rise his stats!");
   //else _combat_message(c, "You used your selected consumable to rise your stats!");

   // inv_decrease(entity_getInventory(e),inv_getSelected(entity_getInventory(e), CONSUMABLE), 1);



  
  return;
}

void _combat_message(Combat* c, char* message) {
  if(!c||!message||!c->cd) return;
  //win_remWindowElement(disp_getLWindow(c->cd,PLAYER_INFO), 0);
  int* disp_dim=disp_getDimensions(c->cd);
  if(!disp_dim) return;
  Window* winmsg=win_ini("Combat status", NULL,0,disp_dim[VD_DATA], disp_dim[H_DATA]/4-10, disp_dim[H_DATA]-30, 0, fcat_lookup(M8));
  if(!winmsg) return;
  FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
  Font* f6=font_load(f);
  fclose(f);
  if(!f6) return;
  Welem* we =we_createLabel(message,f6,3);
  if(!we) {
    font_free(f6);
    free(disp_dim);
    return;
  }
  if(!win_addWindowElement(winmsg, we)) {
    font_free(f6);
    we_free(we);
    free(disp_dim);
    return;
  }
  Canvas* td=win_render(winmsg);
  if(!td) {
    font_free(f6);
    we_free(we);
    free(disp_dim);
    return;
  }
  canv_print(stdout, td, disp_dim[H_DATA]-disp_dim[H_DATA]/4, 0);
  canv_free(td);
  win_free(winmsg);
  font_free(f6);
  we_free(we);
  free(disp_dim);
  return;
}

Combat* combat_load(Combat*c) {
  if(!c||!c->cd||!c->player||!c->enemy) return NULL;

  if(disp_getNLatWindow(c->cd)>0) {
    for(int i=disp_getNLatWindow(c->cd)-1; i>=0; --i) {
      if(!disp_RemLwindow(c->cd, i)) return NULL;
    }
  }

  int* disp_dim=disp_getDimensions(c->cd);
  Welem* pstats[ATT_NUMBER];
  Welem* estats[ATT_NUMBER];
  Welem* movs[OBJ_MAX_ATTACKS+1]; //Attacks + consumable

  for(int i=0;i<sizeof(pstats)/sizeof(pstats[0]);++i)pstats[i]=0;
  for(int i=0;i<sizeof(estats)/sizeof(estats[0]);++i)estats[i]=0;
  for(int i=0;i<sizeof(movs)/sizeof(movs[0]);++i)movs[i]=0;


  char ch[ATT_NUMBER][128];

  Window* winplayer=NULL;
  Window* winenemy =NULL;
  Window* winoptions=NULL;
  Window* wininfo=NULL;

  Sprite* ps;
  Sprite* es;

  sprintf(ch[0], "Health: %d", attb_get(c->stats[PLAYER], HEALTH));
  sprintf(ch[1], "Attack: %d", attb_get(c->stats[PLAYER], ATTACK));
  sprintf(ch[2], "Defense: %d", attb_get(c->stats[PLAYER], DEFENSE));
  sprintf(ch[3], "Speed: %d", attb_get(c->stats[PLAYER], SPEED));
  sprintf(ch[4], "Agility: %d", attb_get(c->stats[PLAYER], AGILITY));
  for(int i=0;i<ATT_NUMBER;++i) {
      pstats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
      if(!pstats[i])goto ERR_END;
  }

  sprintf(ch[0], "Health: %d", attb_get(c->stats[ENEMY], HEALTH));
  sprintf(ch[1], "Attack: %d", attb_get(c->stats[ENEMY], ATTACK));
  sprintf(ch[2], "Defense: %d", attb_get(c->stats[ENEMY], DEFENSE));
  sprintf(ch[3], "Speed: %d", attb_get(c->stats[ENEMY], SPEED));
  sprintf(ch[4], "Agility: %d", attb_get(c->stats[ENEMY], AGILITY));
  for(int i=0;i<ATT_NUMBER;++i) {
      estats[i]=we_createLabel(ch[i],fcat_lookup(M4),0);
      if(!estats[i])goto ERR_END;
  }

  for(int i=0;i<OBJ_MAX_ATTACKS;++i) {
      movs[i]=we_createLabel(skill_getName(c->moveset[PLAYER][i]),fcat_lookup(M4),0);
      if(!movs[i])goto ERR_END;
  }
  const char* caux=obj_getName(inv_getSelected(entity_getInventory(c->player), CONSUMABLE));
  movs[4]=we_createLabel(caux? caux:" ", fcat_lookup(M4),0);
  if(!movs[4])goto ERR_END;

  winplayer=win_ini(c->name[0],pstats,5,disp_dim[W_DATA]-disp_dim[VD_DATA]-1,disp_dim[H_DATA]/4-10,0,0,fcat_lookup(M8));
  if(!winplayer)goto ERR_END;

  winenemy=win_ini(c->name[1],estats,5,disp_dim[W_DATA]-disp_dim[VD_DATA]-1,disp_dim[H_DATA]/4-10,disp_dim[H_DATA]/4-10,0,fcat_lookup(M8));
  if(!winenemy)goto ERR_END;

  winoptions=win_ini("Movements",movs,5,disp_dim[W_DATA]-disp_dim[VD_DATA]-1,disp_dim[H_DATA]/4-10, disp_dim[H_DATA]/2-20,0,fcat_lookup(M8));
  if(!winoptions)goto ERR_END;

  wininfo=win_ini(" ",NULL,0,disp_dim[W_DATA]-disp_dim[VD_DATA]-1,disp_dim[H_DATA]/4-10,disp_dim[H_DATA]-30,0,fcat_lookup(M4));
  if(!wininfo)goto ERR_END;

  if(!disp_AddLWindow(c->cd, winplayer))goto ERR_END;
  if(!disp_AddLWindow(c->cd, winenemy))goto ERR_END;
  if(!disp_AddLWindow(c->cd, winoptions))goto ERR_END;
  if(!disp_AddLWindow(c->cd, wininfo))goto ERR_END;

  ps=entity_getSprite(c->player);
  if(!ps)goto ERR_END;
  spr_setCoordinates(ps, 200,200);
  es=entity_getSprite(c->enemy);
  if(!es)goto ERR_END;
  spr_setCoordinates(es, 50,600);
  if(!room_addBSprite(disp_getrefRoom(c->cd), ps))goto ERR_END;
  if(!room_addBSprite(disp_getrefRoom(c->cd), es))goto ERR_END;
  Canvas* d=disp_Render(c->cd);
  canv_print(stdout,d,0,0);
  canv_free(d);
  char message[256];
  sprintf(message, "You are in a combat against %s!", entity_getName(c->enemy));
  _combat_message(c, message);
  _combat_info(c, 0);
  disp_setSelIndex(c->cd, PLAYER_ACTIONS,0);

  goto END;
ERR_END:
  c=NULL;
END:
  for(int j=0;j<5;++j)we_free(pstats[j]);
  for(int j=0;j<5;++j)we_free(estats[j]);
  for(int j=0;j<5;++j)we_free(movs[j]);
  win_free(winplayer);
  win_free(winenemy);
  win_free(winoptions);
  win_free(wininfo);
  free(disp_dim);
  spr_free(ps);
  spr_free(es);
  return c;
}


void combat_free(Combat* c) {
  if(!c) return;
  attb_free(c->stats[0]);
  attb_free(c->stats[1]);
  entity_free(c->player);
  entity_free(c->enemy);
  obj_free(c->weapons[0]);
  obj_free(c->weapons[1]);
  free(c->name[0]);
  free(c->name[1]);
  disp_free(c->cd);
  free(c);
}

void _combat_info(Combat* c, int index) {
  if(index>MAX_ATTACKS||index<0||!c||!c->cd) return;

  if(index==MAX_ATTACKS) {
    win_remWindowElement(disp_getLWindow(c->cd,PLAYER_INFO), 0);
    FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
    Font* f6=font_load(f);
    fclose(f);
    if(!f6) return;
    Welem* we =we_createLabel("Here you can use some consumables from your inventory",fcat_lookup(M6),3);
    if(!we) {
      font_free(f6);
      return;
    }
    if(!win_addWindowElement(disp_getLWindow(c->cd,PLAYER_INFO), we)) {
      font_free(f6);
      we_free(we);
      return;
    }

    if(!print_Window(c->cd, PLAYER_INFO)) return;
    font_free(f6);
    we_free(we);
    return;
  }

  win_remWindowElement(disp_getLWindow(c->cd,PLAYER_INFO), 0);
  FILE* f=fopen("Display/Fonts/Robo_Mono/06.txt", "r");
  Font* f6=font_load(f);
  fclose(f);
  if(!f6) return;
  Welem* we =we_createLabel(skill_getDesc(c->moveset[PLAYER][index]),f6,3);
  if(!we) {
    font_free(f6);
    return;
  }
  if(!win_addWindowElement(disp_getLWindow(c->cd,PLAYER_INFO), we)) {
    font_free(f6);
    we_free(we);
    return;
  }

  if(!print_Window(c->cd, PLAYER_INFO)) return;
  font_free(f6);
  we_free(we);
  return;
}
