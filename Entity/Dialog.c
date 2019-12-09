//File created by Jaime Pons Garrido

#include "Dialog.h"
#include <string.h>

#define BUFFER_SIZE 1000
#define NUMBER_TXT 10

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
   char buf[BUFFER_SIZE];    
    
    if(!dman){
        fprintf(stdout, "ERROR invalid pointer dman_available");
        return FALSE;
    }
    else if (dman->f == NULL){
        fprintf(stdout,"ERROR no entity attached, execute dman_getDialog first");
        return FALSE;
    }


        do {
                q  = fgets(buf, BUFFER_SIZE, dman->f);
                if (atoi(buf) == dman->counter) {
                       //CREO QUE ESTO ESTA BIEN PREGUNTAR
                        return TRUE;
                }
                if(atoi(buf) >= dman->counter){
                    fseek(dman->f, strlen(buf), SEEK_CUR); //DONT COMPLETELY TRUST THIS SOLUTION TILL ITS TESTED
                    return FALSE;
                }
                  q = fgets(buf, BUFFER_SIZE, dman->f);
    
        }while(q);
        return FALSE;
}
    
    
    
    


DialogMan* dman_getDialog(int entity_id){
    DialogMan * dman;
    char str[NUMBER_TXT];

    sprintf(str, "%d.txt", entity_id);
    
    dman = dman_ini();
    
    dman->f = fopen(str, "r");
    
    return dman;
}

char* dman_nextLine(DialogMan* dman){
    char buf[BUFFER_SIZE];
    char * q;
    if(!dman) return 0;
    
    if(dman_available(dman) == TRUE){
      q = fgets(buf, BUFFER_SIZE, dman->f);
    }
    return q;
}