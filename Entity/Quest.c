
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
Quest* quest_load(FILE* f,const Canvas* ent_pic){
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

Quest* quest_storeinFile(Quest* q, FILE* f){
    
    if(!q||!f)return NULL;
    
    fprintf(f,"%s\n%s\n%d\n",q->title,q->desc,q->len);
    for(int i=0;i<q->len;++i){
        fprintf(f,"%d %d\n",q->obj[i].fi,q->obj[i].se);
    }
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

    Canvas* mrg, *res, *tmp;
    Wlabel* wl;
    Wlabic* wic;

    mrg=canv_backGrnd(0,0,0,0,0,10);

    //Title
    wl=wl_ini(src->title,fcat_lookup(M8),0);
    res=wl_render(wl,wid);

    wl_free(wl);
    //Description
    wl=wl_ini(src->desc,fcat_lookup(M4),0);
    tmp=wl_render(wl,wid);
    canv_appendVI(res,tmp);
    canv_appendVI(res,mrg);

    canv_free(tmp);
    tmp=NULL;
    //Requirements

    Canvas* c=canv_backGrnd(0,0,0,0,0,0);
    for(int i=0;i<src->len;++i){
        Object* o =odic_lookup(src->obj[i].fi);
        int h,w;
        obj_renderDims(o,src->obj[i].se,fcat_lookup(M4),fcat_lookup(M4),&h,&w);
        Canvas* oc=obj_render(o,src->obj[i].se,fcat_lookup(M4),fcat_lookup(M4),h,w,false);
        Canvas* ooc=oc;
        if(i!=src->len-1)ooc=canv_addMargin(oc,0,10,0,0);
        canv_appendHI(c,ooc);

        if(i!=src->len-1)canv_free(ooc);
        canv_free(oc);
        obj_free(o);
    }

    wic=wi_ini("Requirements:",fcat_lookup(M6),0,5,TEXT_WEST);
    wi_setCanvas(wic,c);
    tmp=wi_render(wic,wid);
    canv_appendVI(res,tmp);
    canv_appendVI(res,mrg);

    canv_free(c);
    canv_free(tmp);
    wi_free(wic);

    wic=wi_ini("Given by: ",fcat_lookup(M6),0,10,TEXT_WEST);
    wi_setCanvas(wic,src->ent_pic);
    Canvas* lef=wi_renderSmall(wic,wid);



    char* cc=calloc(strlen(src->asigner)+3,sizeof(char));
    sprintf(cc,"(%s)",src->asigner);
    wl=wl_ini(cc,fcat_lookup(M6),0);
    Canvas* r2=wl_renderSmall(wl,wid);
    canv_appendHI(lef,r2);

    wi_free(wic);
    wl_free(wl);
    canv_free(r2);
    free(cc);

    wl=wl_ini((src->completed? "Completed":"Not completed"),fcat_lookup(M6),0);
    Canvas* stat=wl_renderSmall(wl,wid);
    Canvas* ccc=canv_addMargin(stat,0,0,0,wid-canv_getWidth(lef)-canv_getWidth(stat));
    canv_appendHI(lef,ccc);
    canv_appendVI(res,lef);

    canv_free(lef);
    canv_free(ccc);
    canv_free(mrg);
    wl_free(wl);
    canv_free(stat);

    return res;


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
bool quest_getCompleted(Quest* q){
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
