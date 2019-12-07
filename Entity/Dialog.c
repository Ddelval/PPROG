//File created by Jaime Pons Garrido

#include "Dialog.h"
#include <string.h>


struct _DialogMan{
    FILE * f;  
    int counter;
    
};

DialogMan * dman_ini(){
    DialogMan *dman;
    
    dman = (DialogMan*)calloc(1, sizeof(DialogMan));
    if(!dman) return NULL;
    
    dman->f = NULL;
    dman->counter = 0;
}

void dman_free(DialogMan * dman){
    if(!dman) return;
    fclose(dman->f);
    free(dman);
    dman = NULL;
    return;
}

void dman_setCounter(DialogMan * dman, int counter){
    if(!dman) return;
    dman->counter = counter;
    return;
}
int dman_getCounter(DialogMan * dman){
    if(!dman) return -1;
    return dman->counter;
}


Bool dman_available(DialogMan * dman){
    int i;
   char * q = NULL;
   char buf[1000];    
    
    if(!dman){
        fprintf(stdout, "ERROR invalid pointer dman_available");
        return FALSE;
    }
    else if (dman->f == NULL){
        fprintf(stdout,"ERROR no entity attached, execute dman_getDialog first");
        return FALSE;
    }


        do {
                q  = fgets(buf, 1000, dman->f);
                if (atoi(buf) == dman->counter) {
                       //CREO QUE ESTO ESTA BIEN PREGUNTAR
                        return TRUE;
                }
                  q = fgets(buf, 1000, dman->f);
    
        }while(q);
        return FALSE;
}
    
    
    
    


DialogMan* dman_getDialog(int entity_id){
    DialogMan * dman;
    char str[10];

    sprintf(str, "%d.txt", entity_id);
    
    dman = dman_ini();
    
    dman->f = fopen(str, "r");
    
    return dman;
}

char* dman_nextLine(DialogMan* dman){
    char buf[1000];
    char * q;
    if(!dman) return 0;
    
    if(dman_available(dman) == TRUE){
      q = fgets(buf, 1000, dman->f);
    }
    return q;
}