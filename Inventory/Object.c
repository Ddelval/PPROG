//  PPROG
//	Object.c
//  Created by David del Val on 05/12/2019
//
//

#include "Object.h"

#define NAME_SIZE 50
#define MAX_ATTACKS 4

struct _Object
{
    int id;
    char name[NAME_SIZE];
    int icon_id;
    int spr_id;
    obj_type type;
    Attributes* atb;
    Skill * attacks[MAX_ATTACKS];
    int n_attacks;
    bool destroyable;
};
char** obj_type_def(){
    char** c=calloc(3,sizeof(char*));
    c[0]=calloc(10,sizeof(char));
    c[1]=calloc(10,sizeof(char));
    c[2]=calloc(10,sizeof(char));
    char* cc="Weapons";
    strcpy(c[0],cc);
    cc="Consumables";
    strcpy(c[1],cc);
    cc="Resources";
    strcpy(c[2],cc);
    return c;
}
Object* obj_ini(){
    Object* ob = calloc(1,sizeof(Object));
    if(!ob)return NULL;
    ob->icon_id=ob->spr_id=-1;
    return ob;
}

void obj_free(Object* ob){
    if(!ob)return;
    //atb_free(ob->atb);
    if(ob->n_attacks){
        for(int i=0;i<ob->n_attacks;++i){
            //skill_free(ob->attack[i]);
        }
        free(ob->attacks);
    }
    free(ob);
}
/**
 * @brief Loads an object from a file
 * The format is the following:
 * id
 * name
 * icon_id
 * sprite_id
 * type
 * attributes
 * number of attacks
 * attacks
 * destroyable
 * @param f File from where the object will be read
 * @return Object* New object with the data
 */
Object* obj_load(FILE* f){
    if(!f)return NULL;
    
    Object* ob=obj_ini();
    if(!ob)return NULL;
    fscanf(f,"%d",&ob->id);
    ob->name[0]='\n';
    while(ob->name[0]=='\n')fgets(ob->name,NAME_SIZE,f);
    ob->name[strlen(ob->name)-1]=0;

    fscanf(f, "%d %d %d",(int*)(&ob->icon_id),(int*)(&ob->spr_id),(int*)(&ob->type));
    //ob->atb=atb_load(f);
    fscanf(f,"%d",&ob->n_attacks);
    for(int i=0;i<ob->n_attacks;++i){
        //ob->attacks[i]=skill_load(f);
    }
    fscanf(f,"%d",(int*)(&ob->destroyable));
    return ob;
}
/**
 * @brief Compares two objects
 * 
 * @param ob1 First object to be compared
 * @param ob2 Second object to be compared
 * @return int  >0 if ob1>ob2
 *              =0 if ob1=ob2
 *              <0 if ob1<ob2
 */
int obj_cmp(Object* ob1, Object* ob2){
    if(!ob1||!ob2)return 0;
    return ob1->id-ob2->id;
}


Object* obj_copy(Object* ob){
    if(!ob)return NULL;
    Object* res= obj_ini();
    res->id         = ob->id;
    res->n_attacks  = ob->n_attacks;
    res->icon_id    = ob->icon_id;
    res->spr_id     = ob->spr_id;
    res->type       = ob->type;
    res->destroyable= ob->destroyable;
    //res->atb=atb_copy(ob->atb);
    strcpy(res->name,ob->name);
    for(int i=0;i<ob->n_attacks;++i){
        //res->attacks[i]=skill_copy(ob->attacks[i]);
    }
    return res;
}

Sprite* obj_getIcon(Object* ob){
    if(!ob||ob->icon_id==-1)return NULL;
    return sdic_lookup(ob->icon_id);
}

Sprite* obj_getSprite(Object* ob){
    if(!ob||ob->spr_id==-1)return NULL;
    return sdic_lookup(ob->spr_id);
}

char* obj_getName(Object* ob){
    if(!ob)return NULL;
    char* ch=calloc(strlen(ob->name)+1,sizeof(char));
    if(!ch)return NULL;
    strcpy(ch,ob->name);
    return ch;
}
obj_type obj_getType(Object* ob){
    return ob? ob->type: -1;
}
int obj_getId(Object* ob){
    return ob? ob->id: -1;
}
Canvas* obj_render(Object* ob, int number,Font* ftext, Font* fnum){
    if(!ob)return 0;
    Sprite* sp=sdic_lookup(ob->icon_id);
    Canvas* c=canv_copy(spr_getDispData(sp));
    spr_free(sp);
    Wlabel* nam=wl_ini(ob->name,ftext,10);
    char snum[10];
    sprintf(snum,"%d",number);
    Wlabel* num=wl_ini(snum,fnum,10);
    Canvas* c2=wl_render(nam,canv_getWidth(c));
    canv_appendVI(c,c2);
    canv_free(c2);
    c2=wl_render(num,canv_getWidth(c));
    canv_appendVI(c,c2);
    
    Canvas* bb=canv_backGrnd(50,50,150,255,canv_getWidth(c)+4,canv_getHeight(c)+2);
    canv_addOverlay(bb,c,1,2);
    canv_free(c);
    Canvas* back=canv_backGrnd(255,255,255,255,canv_getWidth(bb)+4,canv_getHeight(bb)+2);
    canv_addOverlay(back,bb,1,2);
    canv_free(bb);
    return back;
}