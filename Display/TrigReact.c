#include "TrigReact.h"
/*** Functions to process triggers ***/
extern int tier;

void _trig_checkQuest(Entity* e, Display* d);

void trig_give(Trigger* t, void* e, void* d){

    if(!t||!e)return;
    Room* r=disp_getrefRoom(d);
    if(tr_getType(t)!=OBTAIN)return;
    entity_addItem(e,tr_getObj_id(t),tr_getQuantity(t));

    if(tr_getSpr_remove(t)){
        room_removeB(r,tr_getSpr_index(t));
        room_printModBackg(r,0,0);
    }

    _trig_checkQuest(e,d);
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
    _trig_checkQuest(e,d);
}
void trig_showQuest(Trigger* t, void *e, void* d){
    if(!e||!d)return;
    int s=0;
    Quest**q= entity_getQuests(e,&s);
    if(!q)return;
    disp_QuestWindow(d,s,q);
    
    for(int i=0;i<s;++i)quest_free(q[i]);
    free(q);

}
void trig_talk(Trigger* t,void* e, void* d){
    if(!t||!e||!d)return;
    char* n=entity_getName(tr_getEntityRef(t));
    

    Quest* qa=entity_fetchFulfilledQuest(e,n);
    if(qa){
        tier++;
    }
    quest_free(qa);
    //fprintf(stderr, "tier: %d\n",tier);
    DialogMan* dd=entity_getDialogs(tr_getEntityRef(t));
    
    disp_DialogWindow(d,dd,n);
    
    Quest* a=dman_fetchQuest(dd);
    if(a){
        quest_setAsigner(a,n);
        entity_addQuest(e,a);
        //fprintf(stderr,"hello");
    }
    quest_free(a);
    free(n);
    
    dman_autoAdvance(dd);
    extern char* next_world;
    if (tier>=TIER_FINISHED){
        next_world=strdup("End");
    }
    
}
void trig_enter(Trigger* t, void* e, void* d){
    extern char* next_world;
    next_world=tr_getNWorld(t);
}

//Check for completion of quest:
void _trig_checkQuest(Entity* e, Display* d){
        
    int len=0;
    Quest** q= entity_questJustCompleted(e,&len);
    if(len){
        for(int i=0;i<len;++i){
            disp_QuestFulfilledWindow(d,q[i]);
            quest_free(q[i]);
        }
    }
    free(q);
}