#include <stdio.h>

#include "Window.h"
#include "Welem.h"
#include "Wlabel.h"
#include "Display.h"
#include "Combat.h"
#include "EntityDic.h"
#include "Entity.h"

int main() {
  Entity* player=edic_lookup(0, NULL);
  Entity* enemy=edic_lookup(0, NULL);
  printf("%d",inv_getSelectedIndex(entity_getInvRef(player), WEAPON));
  if(!player||!enemy) return 1;
  Combat* co=combat_ini(player, enemy);
  if(!co) return 1;
  combat_load(co);
  combat_execute(co);
  combat_free(co);

  return 0;
}
