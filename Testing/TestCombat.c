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
  Entity* player=edic_lookup(0, NULL);
  Entity* enemy=edic_lookup(0, NULL);

  Object* o=inv_getSelected(entity_getInventory(player), WEAPON);
  printf("AA");
  return 0;
}
