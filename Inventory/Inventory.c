//  PPROG
//	Inventory.c
//  Created by David del Val on 05/12/2019
//
//

#include "Inventory.h"
#define INITIAL_SIZE 10
#define INCREMENT 1.5
/**
 * @brief Structure that holds all the items that an entity has.
 * The macro OBJ_TYPE_SIZE is the amount of different types of objects
 * that are defined.
 * For each type of object we have the follwowing:
 *  - Object* items: Array of pointers to objects.
 *  - int*    times: Int array that stores the amount of each object
 *  - int      size: Int that stores the amount of different objets stored in items
 *  - int     alloc: Int that stores the space that has been allocated.
 *  - int  selected: The object that is being selected in each type
 *
 * alloc and size will be compared to decide when is it necessary to increment the size of the array
 *
 */
struct _Inventory
{
    Object** items[OBJ_TYPE_SIZE];
    int* times[OBJ_TYPE_SIZE];
    int size[OBJ_TYPE_SIZE];
    int alloc[OBJ_TYPE_SIZE];
    int selected[OBJ_TYPE_SIZE];
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
    for(int i=0;i<OBJ_TYPE_SIZE;++i){
        if(inv->items[i]){
            for(int j=0;j<inv->size[i];++j)obj_free(inv->items[i][j]);
        }
        free(inv->items[i]);
        free(inv->times[i]);
    }
    free(inv);
}

Inventory* inv_copy(Inventory* inv) {
    if(!inv) return NULL;
    Inventory* in =inv_ini();
    memcpy(in,inv,sizeof(Inventory));
    for(int i=0;i<OBJ_TYPE_SIZE;i++) {

        in->times[i]=(int*)    calloc(in->size[i], sizeof(int));
        in->alloc[i]=(Object*) calloc(in->size[i], sizeof(Object*));
        if(!in->times[i]||!in->alloc[i]) {
            inv_free(in);
            return NULL;
        }
        memcpy(in->times,inv->times,sizeof(int)*in->size[i]);
        for(int j=0;j<in->size[i];++j){
            in->alloc[i]=obj_copy(inv->alloc[i]);
            if(!in->alloc[i]){
                inv_free(in);
                return NULL;
            }   
  }
  return in;
}

Inventory* inv_insertSeveral(Inventory* inv,Object*ob,int quantity){
    if(!inv||!ob)return NULL;
    obj_type ob_ty=obj_getType(ob);

    for(int i=0;i<inv->size[ob_ty];++i){
        if(obj_cmp(inv->items[ob_ty][i],ob)==0){
            inv->times[ob_ty][i]+=quantity;
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
    inv->times[ob_ty][inv->size[ob_ty]]+=quantity;
    inv->size[ob_ty]++;
    return inv;
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
Inventory* inv_decrease(Inventory* inv, Object* ob, int quantity){
    if(!inv||!ob)return NULL;
    obj_type ob_ty=obj_getType(ob);

    for(int i=0;i<inv->size[ob_ty];++i){
        if(obj_cmp(inv->items[ob_ty][i],ob)==0){
            inv->times[ob_ty][i]-=quantity;
            if(inv->times[ob_ty][i]<=0){
                inv_remove(inv,ob);
            }
            return inv;
        }
    }
    return inv;
}
Canvas *** inv_render(Inventory* inv, int* dim, int ** dimens,char *** texts,Font* ftext, Font* fnum){
    if(!inv)return NULL;
    *dim=OBJ_TYPE_SIZE;
    *texts=obj_type_def();
    Canvas*** tot=calloc(OBJ_TYPE_SIZE,sizeof(Canvas**));
    *dimens=calloc(OBJ_TYPE_SIZE,sizeof(int));
    if(!tot||!(*dimens)||!texts){
        free(dimens);
        free(tot);
        free(texts);
        return NULL;
    }
    for(int ty=0;ty<OBJ_TYPE_SIZE;++ty){
        tot[ty]=calloc(inv->size[ty],sizeof(Canvas*));
        (*dimens)[ty]=inv->size[ty];
        int he=0;
        int wi=0;
        int h,w;
        for(int el=0;el<inv->size[ty];++el){
            obj_renderDims(inv->items[ty][el],inv->times[ty][el],ftext,fnum,&h,&w);
            he=max(he,h);
            wi=max(wi,w);
        }
        for(int el=0;el<inv->size[ty];++el){
            tot[ty][el]=obj_render(inv->items[ty][el],inv->times[ty][el],ftext,fnum,he,wi);
            if(!tot[ty][el]){
                for(int z=0;z<ty;++z)for(int j=0;j<inv->size[z];++j)canv_free(tot[z][j]);
                for(int j=0;j<el;++j)canv_free(tot[ty][j]);
                for(int j=0;j<=ty;++j)free(tot[ty]);
                free(dimens);
                free(tot);
                free(*texts);
                return NULL;
            }
        }
    }
    return tot;
}


int inv_getQuantity(Inventory* inv, int obj_id){
    if(!inv)return -1;
    for (int i=0;i<OBJ_TYPE_SIZE;++i){
        for(int j=0;j<inv->size[i];++j){
            if(obj_getId(inv->items[i][j])==obj_id) return inv->times[i][j];
        }
    }
    return 0;
}
