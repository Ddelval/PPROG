#include "TrigReact.h"
/*** Functions to process triggers ***/

void trig_give(Trigger* t, void* e, void* r){

    if(!t||!e)return;
    if(tr_getType(t)!=OBTAIN)return;
    entity_addItem(e,tr_getObj_id(t),tr_getQuantity(t));
    if(tr_getSpr_remove(t)){
        room_removeB(r,tr_getSpr_index(t));
        room_printModBackg(r,0,0);
    }
    
    
} 