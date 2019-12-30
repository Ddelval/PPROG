/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: jaime
 *
 * Created on 12 de diciembre de 2019, 11:39
 */

#include <stdio.h>
#include <stdlib.h>
#include "Combat.h"

/*
 *
 */
int main(int argc, char** argv) {
    Entity * player;
    Entity * enemy;
  //  Attributes * atb;
  //  Attributes * atbe;
    Combat * c;
    int set[5];
    int set1[5];
    set[0] = 100;
    set[1] = 14;
    set[2] = 10;
    set[3] = 5;
    set[4] = 6;
    set1[0] = 100;
    set1[1] = 10;
    set1[2] = 13;
    set1[3] = 1;
    set1[4] = 10;

  //  atb = attb_ini();
  //  atbe = attb_ini();
    player = entity_ini ("Jugador", 1, 1, 1);
    enemy = entity_ini ("Esqueleto", 2, 1, 2);



    attb_setAll(entity_getAttributes(player),set);
    attb_setAll(entity_getAttributes(enemy),set1);


    c = combat_ini(player,enemy);

    combat_exe(c);
    entity_free(player);
    entity_free(enemy);




    return 0;
}
