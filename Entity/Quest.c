
#include "Quest.h"

#define DESC_LENGTH 200
#define TITLE_LENGTH 30

struct _Quest{
    char desc[200];
    char title [30];
    char* asigner;
    Canvas* ent_pic;
    pairii* obj;
    int len;
    bool completed;
};

Quest* quest_ini(){
    Quest* q=calloc(1,sizeof(Quest));
    return q;
}
void quest_free(Quest*q){
    if(!q)return;
    free(q->obj);
    free(q->asigner);
    canv_free(q->ent_pic);
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
Quest* quest_load(FILE* f,Canvas* ent_pic){
    if(!f)return NULL;
    Quest* q=quest_ini();
    if(!q)return NULL;
    fgets(q->title,TITLE_LENGTH,f);
    fgets(q->desc,DESC_LENGTH,f);
    if(strlen(q->title))q->title[strlen(q->title)-1]=0;
    if(strlen(q->desc)) q->desc [strlen(q->desc) -1]=0;
    fscanf(f,"%d",&q->len);
    q->obj=calloc(q->len,sizeof(pairii));
    for(int i=0;i<q->len;++i){
        fscanf(f,"%d %d",&q->obj[i].fi, &q->obj[i].se);
    }
    q->ent_pic=canv_copy(ent_pic);
    return q;
}

Quest* quest_copy(const Quest* src){
    if(!src)return NULL;
    Quest* n=quest_ini();
    if(!n)return NULL;
    memcpy(n,src,sizeof(Quest));
    if(n->asigner)n->asigner=strdup(src->asigner);
    if(n->ent_pic)n->ent_pic=canv_copy(src->ent_pic);
    if(n->obj)n->obj=calloc(n->len,sizeof(pairii));
    memcpy(n->obj,src->obj,src->len*sizeof(pairii));
    return n;
}
Canvas* quest_render(Quest* src, int wid){
    if(!src)return NULL;
    Canvas* mrg=canv_backGrnd(0,0,0,0,0,10);
    Wlabel* wl=wl_ini(src->title,fcat_lookup(M8),0);
    Canvas* t=wl_render(wl,wid);
    wl_free(wl);
    wl=wl_ini(src->desc,fcat_lookup(M4),0);
    Canvas* b=wl_render(wl,wid);
    canv_appendVI(t,b);
    canv_appendVI(t,mrg);
    Canvas* c=canv_backGrnd(0,0,0,0,0,0);
    for(int i=0;i<src->len;++i){
        Object* o =odic_lookup(src->obj[i].fi);
        int h,w;
        obj_renderDims(o,src->obj[i].se,fcat_lookup(M4),fcat_lookup(M4),&h,&w);
        Canvas* oc=obj_render(o,1,fcat_lookup(M4),fcat_lookup(M4),h,w,false);
        canv_appendHI(c,oc);
    }
    
    Wlabic* req=wi_ini("Requierements:",fcat_lookup(M6),0,5,TEXT_WEST);
    wi_setCanvas(req,c);
    Canvas* s=wi_render(req,wid);
    canv_appendVI(t,s);
    canv_appendVI(t,mrg);
    
    Wlabic* asn=wi_ini("Given by: ",fcat_lookup(M6),0,10,TEXT_WEST);
    wi_setCanvas(asn,src->ent_pic);
    Canvas* r=wi_renderSmall(asn,wid);

    char* cc=calloc(strlen(src->asigner)+3,sizeof(char));
    sprintf(cc,"(%s)",src->asigner);
    Wlabel* wla=wl_ini(cc,fcat_lookup(M6),0);
    Canvas* r2=wl_renderSmall(wla,wid);
    canv_appendHI(r,r2);

    Wlabel* down=wl_ini((src->completed? "Completed":"Not completed"),fcat_lookup(M6),0);
    Canvas* stat=wl_renderSmall(down,wid);
    Canvas* ccc=canv_backGrnd(0,0,0,0,wid-canv_getWidth(r)-canv_getWidth(stat),0);
    canv_appendHI(r,ccc);
    canv_appendHI(r,stat);
    canv_appendVI(t,r);

    return t;


}

void quest_setCompleted(Quest* q, bool completed){
    if(q)q->completed=completed;
}

void quest_setAsigner(Quest* q, char* a){
    if(q) q->asigner=strdup(a);
}

char* quest_getAsigner(Quest* q){
    return q? strdup(q->asigner): NULL;
}
bool quest_getFulfilled(Quest* q){
    return q? q->completed: false;
}

pairii* quest_getRequirements(Quest* q,int* siz){
    if(!q)return NULL;
    pairii* p=calloc(q->len,sizeof(pairii));
    if(!p)return NULL;
    memcpy(p,q->obj,sizeof(pairii)*q->len);
    *siz=q->len;
    return p;
}