/*  Combat.c    */

#include "Combat.h"
#include "Window.h"
#include "Room.h"
#include "Font.h"
#include "Pixel.h"
#include <stdlib.h>
#include <time.h>

struct _Combat {
    Entity *player, *enemy;
    char* name[2];
    Attributes* stats[2];
    Skill* moveset[2][4];
    bool stunplayer, stunenemy;
};


Combat* combat_ini(Entity* player, Entity* enemy) {
  if(!player||!enemy) return NULL;
  Combat* c = (Combat*)calloc(1, sizeof(Combat));
  if(!c) return NULL;

  c->player = entity_copy(player);
  if(!c->player) {
    free(c);
    return NULL;
  }
  c->enemy = entity_copy(enemy);
  if(!c->enemy) {
    combat_free(c);
    return NULL;
  }
  c->name[0] = entity_getName(player);
  c->name[1] = entity_getName(enemy);
  c->stats[0] = attb_copy(entity_getAttributes(player));
  c->stats[1] = attb_copy(entity_getAttributes(enemy));
  c->stunplayer = false;
  c->stunenemy = false;
  for(int i=0; i<4; i++){
    c->moveset[0][i]= skill_readFromFile("skill.txt",i+1);
    c->moveset[1][i]= skill_readFromFile("skill.txt",i+1);
  }
  return c;
}

//REVISADA, NO COMPILA

int player_choice() {
    int move = 0;
    while (move < 1 || move > 4) {
        scanf("%d", &move);
        if (move <= 4 && move >= 1) break;
        fprintf(stderr, "Please use a valid movement:\n");
    }
    return move - 1;
}


// For now the IA will just choose the attack with the highest attack field.
//REVISADA,COMPILA

int IA_choice(Combat * state) {
    int max_attack = 0;
    int i = 0;

    if (!state) return -1;
    //ESTAMOS ASUMIENDO QUE EL ENEMIGO TIENE 4 ATAQUES SINO QUIERES QUE ESTO OCURRA AÑADE UN CAMPO A LA ESTRUCTURA SKILL
    while (i <= 3) {
        if (attb_get(skill_getAtbatk(state->moveset[1][i]), 1) > attb_get(skill_getAtbatk(state->moveset[1][max_attack]), 1)) {
            max_attack = i;
        }
        i++;
    }

    return max_attack;
}

int combat_exe(Combat *c) {
    int i = 0, aux = 1, move = 0;

    if (!c) return -1;

    if (attb_get(c->stats[0], 3) > attb_get(c->stats[1], 3)) {
        aux = 0;
    }



    while (attb_get(c->stats[0], 0) > 0 && attb_get(c->stats[1], 0) > 0) {
        if ((i + aux) % 2 == 0) {
            if (c->stunplayer == false) {
                fprintf(stdout, "El jugador ataca primero, selecciona una acción:\n");
                fprintf(stdout, "Listado de movimientos:\n %s\t %s\n%s\t%s\n", skill_getName(c->moveset[0][0]), skill_getName(c->moveset[0][1]), skill_getName(c->moveset[0][2]), skill_getName(c->moveset[0][03]));
                move = player_choice();
                movement_exe(c, move, 0);
            }

            if (c->stunplayer == true) {
                fprintf(stdout, "You have been stunned, meanwhile, you can have some tea.\n");
                c->stunplayer = false;
            }

        } else {
            if (c->stunenemy == false) {
                move = IA_choice(c);
                movement_exe(c, move, 1);
            }
            if (c->stunenemy == true) {
                fprintf(stdout, "THe enemy has been stunned, his damage increased by 100, just joking.\n");
                c->stunenemy = false;
            }
        }
        i++;
    }

    combat_destroy(c);
    //decir qn ha muerto y liberar la memoria de la strutura combat
    return 0;
}

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






int movement_exe(Combat * c, int action, int ent) {
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

void combat_destroy(Combat* c) {
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
  free(c);
}
