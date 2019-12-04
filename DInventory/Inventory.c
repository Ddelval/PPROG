
#include "Inventory.h"
#define INITIAL_SIZE 10
#define INCREMENT 1.5  
struct _Inventory
{
    Object** items[OBJ_TYPE_SIZE];
    int* times[OBJ_TYPE_SIZE];
    int size[OBJ_TYPE_SIZE];
    int alloc[OBJ_TYPE_SIZE];
};

Inventory* inv_ini(){
    Inventory* inv = calloc(1,sizeof(Inventory));
    if(!inv)return NULL;
    for(int i=0;i<OBJ_TYPE_SIZE;++i){
        inv->items[i]=calloc(INITIAL_SIZE,sizeof(Object*));
        inv->times[i]=calloc(INITIAL_SIZE,sizeof(int));
        if(!inv->times[i]||!inv->items[i]){
            inv_free(inv);
            return NULL;
        }
        inv->size[i]=0;
        inv->alloc[i]=INITIAL_SIZE;
    }
    return inv;
}

void inv_free(Inventory* inv){
    if(!inv)return;
    for(int i=0;i<INITIAL_SIZE;++i){
        if(inv->items[i]){
            for(int j=0;j<inv->size[i];++j)obj_free(inv->items[i][j]);
        }
        free(inv->items[i]);
        free(inv->times[i]);
    }
    free(inv);
}
Inventory* inv_insert(Inventory* inv, Object* ob){
    if(!inv||!ob)return NULL;
    obj_type ob_ty=obj_getType(ob);

    for(int i=0;i<inv->size[ob_ty];++i){
        if(obj_cmp(inv->items[ob_ty][i],ob)==0){
            inv->times[ob_ty][i]++;
            return inv;
        }
    }
    
    // We already now that the object is not in the inventory    
    if(inv->size[ob_ty]==inv->alloc[ob_ty]){
        int nsiz=inv->alloc[ob_ty]*INCREMENT;
        Object** tmp=realloc(inv->items[ob_ty],nsiz*sizeof(Object*));
        int * itmp =realloc(inv->times,nsiz*sizeof(int));
        if(!tmp||!itmp){
            return NULL;
        }
        inv->alloc[ob_ty]=nsiz;
        inv->times[ob_ty]=itmp;
        inv->items[ob_ty]=tmp;
    }

    inv->items[ob_ty][inv->size[ob_ty]]=obj_copy(ob);
    inv->times[ob_ty][inv->size[ob_ty]]++;
    inv->size[ob_ty]++;
    return inv;    
}
Inventory* inv_remove(Inventory* inv, Object* ob){
    if(!inv||!ob)return NULL;
    obj_type ob_ty=obj_getType(ob);
    bool found=false;
    for(int i=0;i<inv->size[ob_ty];++i){
        if(found){
            inv->items[ob_ty][i-1]=inv->items[ob_ty][i];
            inv->times[ob_ty][i-1]=inv->times[ob_ty][i];

            inv->times[ob_ty][i]=0;
            inv->items[ob_ty][i]=0;
        }
        if(!found && obj_cmp(inv->items[ob_ty][i],ob)==0){
            obj_free(inv->items[ob_ty][i]);
            inv->items[ob_ty][i]=NULL;
            inv->times[ob_ty][i]=0;
            found=true;
        }
    }
    return found? inv: NULL;
}
Inventory* inv_decrease(Inventory* inv, Object* ob){
    if(!inv||!ob)return NULL;
    obj_type ob_ty=obj_getType(ob);

    for(int i=0;i<inv->size[ob_ty];++i){
        if(obj_cmp(inv->items[ob_ty][i],ob)==0){
            inv->times[ob_ty][i]--;
            if(inv->times[ob_ty][i]==0){
                inv_remove(inv,ob);
            }
            return inv;
        }
    }
}
