
#include "Quest.h"

#define DESC_LENGTH 200
#define TITLE_LENGTH 30

struct _Quest{
    char desc[200];
    char title [30];
    int obj_to_get_id;
};

Quest* quest_ini(){
    Quest* q=calloc(1,sizeof(Quest));
    return q;
}
void quest_free(Quest*q){
    free(q);
}
/**
 * @brief Reads the quest from a file
 * 
 * The format is the following:
 * Title
 * Description
 * obj_to_get_id
 * 
 * @param f File from which the data will be read
 * @return  New quest 
 */
Quest* quest_load(FILE* f){
    if(!f)return NULL;
    Quest* q=quest_ini();
    if(!q)return NULL;
    fgets(q->title,TITLE_LENGTH,f);
    fgets(q->desc,DESC_LENGTH,f);
    if(strlen(q->title))q->title[strlen(q->title)-1]=0;
    if(strlen(q->desc)) q->desc [strlen(q->desc) -1]=0;
    fscanf(f,"%d",&q->obj_to_get_id);

    return q;
}

Quest* quest_copy(const Quest* src){
    if(!src)return NULL;
    Quest* n=quest_ini();
    if(!n)return NULL;
    memcpy(n,src,sizeof(Quest));
    return n;
}
Canvas* quest_render(Quest* src, int wid){
    if(!src)return NULL;

    Wlabel* wl=wl_ini(src->title,fcat_lookup(M8),0);
    Canvas* t=wl_render(wl,wid);
    wl_free(wl);
    wl=wl_ini(src->desc,fcat_lookup(M6),0);
    Canvas* b=wl_render(wl,wid);
    canv_appendVI(t,b);
    Object* o =odic_lookup(src->obj_to_get_id);
    int h,w;
    obj_renderDims(o,1,fcat_lookup(M4),fcat_lookup(M4),&h,&w);
    Canvas* oc=obj_render(o,1,fcat_lookup(M4),fcat_lookup(M4),h,w,false);
    Wlabic* req=wi_ini("Requierements:",fcat_lookup(M6),0,5,TEXT_WEST);
    wi_setCanvas(req,oc);
    Canvas* s=wi_render(req,wid);
    canv_appendVI(t,s);

    return t;


}