#include <stdio.h>

#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include "Combat.h"
#include "EntityDic.h"
#include "Entity.h"
#include "Inventory.h"
#include "Object.h"

int main() {

  term_init();

  Entity* player=edic_lookup(0, NULL);
  Entity* enemy=edic_lookup(2, NULL);

  Combat* c=combat_ini(player, enemy);
  printf("Combat initialized\n");

  if(!combat_load(c)) return 1;
  printf("Combat loaded\n");
  combat_execute(c);
  combat_free(c);
  entity_free(player);
  entity_free(enemy);
  sdic_free();
  edic_free();
  fcat_free();
  odic_free();
  trdic_free();
  term_restore();
  return 0;
}
