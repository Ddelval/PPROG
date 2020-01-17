//  PPROG
//	Inventory.c
//  Created by David del Val on 05/12/2019
//
//

#include "Inventory.h"
#define INITIAL_SIZE 10
#define INCREMENT 2
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

Inventory* inv_ini() {
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
    for(int i=0;i<OBJ_TYPE_SIZE;++i) inv->selected[i]=0;
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

Inventory* inv_copy(const Inventory* inv) {
    if(!inv) return NULL;
    Inventory* in =inv_ini();
    for(int i=0;i<OBJ_TYPE_SIZE;++i){
        free(in->times[i]);
        free(in->items[i]);
    }
    memcpy(in,inv,sizeof(Inventory));
    for(int i=0;i<OBJ_TYPE_SIZE;i++) {
        in->times[i]=(int*)    calloc(in->alloc[i], sizeof(int));
        in->items[i]=(Object**) calloc(in->alloc[i], sizeof(Object*));
        if(!in->times[i]||!in->items[i]) {
            inv_free(in);
            return NULL;
        }
        memcpy(in->times[i],inv->times[i],sizeof(int)*in->size[i]);
        for(int j=0;j<in->size[i];++j){
            in->items[i][j]=obj_copy(inv->items[i][j]);
            if(!in->items[i]){
                inv_free(in);
                return NULL;
            }
        }
    }
  return in;
}

Inventory* inv_insertSeveral(Inventory* inv,const Object*ob,int quantity){
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
        int * itmp =realloc(inv->times[ob_ty],nsiz*sizeof(int));
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
Inventory* inv_insert(Inventory* inv, const Object* ob){
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
        int * itmp =realloc(inv->times[ob_ty],nsiz*sizeof(int));
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
Inventory* inv_remove(Inventory* inv, const Object* ob){
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
    inv->size[ob_ty]-=found;
//    return found? inv: NULL;
      return inv;
}
Inventory* inv_decrease(Inventory* inv, const Object* ob, int quantity){
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
Canvas *** inv_render(const Inventory* inv, int* dim, int ** dimens,char *** texts, pairii** sizes,const Font* ftext, const Font* fnum){
    if(!inv)return NULL;
    *dim=OBJ_TYPE_SIZE;
    *texts=obj_type_def();
    Canvas*** tot=calloc(OBJ_TYPE_SIZE,sizeof(Canvas**));
    *dimens=calloc(OBJ_TYPE_SIZE,sizeof(int));
    *sizes=calloc(OBJ_TYPE_SIZE,sizeof(pairii));
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
        (*sizes)[ty].fi=he;
        (*sizes)[ty].se=wi;
        for(int el=0;el<inv->size[ty];++el){
            tot[ty][el]=obj_render(inv->items[ty][el],inv->times[ty][el],ftext,fnum,he,wi,el==inv->selected[ty]);
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


int inv_getQuantity(const Inventory* inv, int obj_id){
    if(!inv)return -1;
    for (int i=0;i<OBJ_TYPE_SIZE;++i){
        for(int j=0;j<inv->size[i];++j){
            if(obj_getId(inv->items[i][j])==obj_id) return inv->times[i][j];
        }
    }
    return 0;
}
/**
 * @brief Reads an inventory
 * Format:
 * number_of_elements
 * id quantity
 * id quantity
 * ...
 *
 * @param f
 * @return Inventory*
 */
Inventory* inv_load(FILE* f){
    if(!f)return NULL;
    Inventory* inv=inv_ini();
    if(!inv)return NULL;
    int nelem=0;
    int a,b;
    fscanf(f,"%d",&nelem);
    for(int i=0;i<nelem;++i){
        fscanf(f,"%d %d",&a,&b);
        Object* ob=odic_lookup(a);
        inv_insertSeveral(inv,ob,b);
        obj_free(ob);
    }
    return inv;
}

bool inv_checkPresent(const Inventory* inv, pairii* obj, int len){
    if(!inv||!obj)return false;

    for(int i=0;i<len;++i){
        if(inv_getQuantity(inv,obj[i].fi)<obj[i].se)return false;
    }
    return true;
}

const Object* inv_getSelected(const Inventory * inv, obj_type t){
  if(!inv) return NULL;
  if(inv->selected[t]>=inv->size[t])return NULL;
  return inv->items[t][inv->selected[t]];
}
Object* inv_getSelectedCopy(const Inventory* inv, obj_type t){
  if(!inv) return NULL;
  return obj_copy(inv->items[t][inv->selected[t]]);
}
Inventory* inv_incrementSelected(Inventory* inv, obj_type t, int incr){
    if(!inv)return NULL;
    if(inv->size[t]==0)return NULL;
    inv->selected[t]=(inv->selected[t]+incr+inv->size[t])%inv->size[t];
    return inv;
}
Canvas* inv_renderObj(Inventory* inv, obj_type t, int hei, int wid, const Font* ftext, const Font* fnum, int pos,bool selected){
    if(!inv)return NULL;
    if(inv->size[t]==0||pos>inv->size[t])return NULL;
    return obj_render(inv->items[t][pos],inv->times[t][pos],ftext,fnum,hei,wid,selected);
}
int inv_getSelectedIndex(const Inventory* inv, obj_type t){
    return inv? inv->selected[t]:-1;
}
