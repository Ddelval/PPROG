#include "TrigReact.h"
/*** Functions to process triggers ***/

void trig_give(Trigger* t, void* e, void* d){

    if(!t||!e)return;
    Room* r=disp_getrefRoom(d);
    if(tr_getType(t)!=OBTAIN)return;
    entity_addItem(e,tr_getObj_id(t),tr_getQuantity(t));
    if(tr_getSpr_remove(t)){
        room_removeB(r,tr_getSpr_index(t));
        room_printModBackg(r,0,0);
    }
} 
void trig_showInv(Trigger* t, void* e,void* d){
    if(!e||!d)return;
    const Inventory* in =entity_getInvRef(e);
    disp_InventoryWindow(d,in,fcat_lookup(M8),fcat_lookup(M6),fcat_lookup(M4),fcat_lookup(M6));
}
void trig_showRec(Trigger* t,void *e, void* d){
    if(!e||!d)return;
    const Inventory* in=entity_getInvRef(e);
    disp_CraftingWindow(d,in);
}