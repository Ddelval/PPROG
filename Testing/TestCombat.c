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
  Inventory* inv=entity_getInventory(player);
  if(!inv) printf("noo\n");

  printf("%d\n", inv_getQuantity(inv, 6));

  return 0;
}
