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
    Inventory* in =entity_getInvRef(e);
    disp_InventoryWindow(d,in,fcat_lookup(M8),fcat_lookup(M6),fcat_lookup(M4),fcat_lookup(M6));
}
void trig_showRec(Trigger* t,void *e, void* d){
    if(!e||!d)return;
    Inventory* in=entity_getInvRef(e);
    disp_CraftingWindow(d,in);
}
void trig_showQuest(Trigger* t, void *e, void* d){
    if(!e||!d)return;
    int s=0;
    Quest**q= entity_getQuests(e,&s);
    disp_QuestWindow(d,s,q);
}
void trig_talk(Trigger* t,void* e, void* d){
    if(!t||!e||!d)return;
    char* n=entity_getName(tr_getEntityRef(t));
    DialogMan* dd=entity_getDialogs(tr_getEntityRef(t));
    
    disp_DialogWindow(d,dd,n);
    Quest* a=dman_fetchQuest(dd);
    if(a){
        entity_addQuest(e,a);
        fprintf(stderr,"hello");
    }
    free(n);
}