/*  Dialog.c  */

#include "Dialog.h"
#include <string.h>

// #define BUFFER_SIZE 1000
// #define NUMBER_TXT 10
#define MAX_DIALOG 128

struct _Dialog {
  char** lines;
  int id;
  int nlines;
  int linepos;
};

Dialog* diag_ini() {
  Dialog* diag = (Dialog*)calloc(1,sizeof(Dialog));
  return diag;
}

void diag_free(Dialog* diag){
  if(!diag) return;
  for(int i=0;i<diag->nlines;i++) if(diag->lines[i]) free(diag->lines[i]);
  free(diag->lines);
  free(diag);
}

Dialog* diag_copy(Dialog* diag) {
  if(!diag) return NULL;
  Dialog* d = diag_ini();
  if(!d) return NULL;
  d->id=diag->id;
  d->nlines=diag->nlines;
  d->linepos=diag->linepos;
  d->lines=(char**)calloc(d->nlines,sizeof(char*));
  for(int i=0;i<d->nlines;i++) {
    d->lines[i]=(char *)calloc(MAX_DIALOG, sizeof(char));
    if(!d->lines[i]) {
      diag_free(d);
      return NULL;
    }
    if(!strcpy(d->lines[i],diag->lines[i])) {
      diag_free(d);
      return NULL;
    }
  }
  return d;
}

const char* diag_getNext(Dialog* diag) {
  if(!diag) return NULL;
  diag->linepos++;
  return diag->lines[diag->linepos];
}

int diag_getId(Dialog* diag) {
  if(!diag) return -1;
  return diag->id;
}

int diag_getLength(Dialog* diag) {
  if(!diag) return -1;
  return diag->nlines;
}

int diag_getCurrentLine(Dialog* diag) {
  if(!diag) return -1;
  return diag->linepos;
}

Dialog* diag_setLine(Dialog* diag, int newline) {
  if(!diag||newline<0||newline>=diag->nlines) return NULL;
  diag->linepos=newline;
  return diag;
}

Dialog* diag_jumpLines(Dialog* diag, int lines) {
  if(!diag||diag->linepos+lines>=diag->nlines||diag->linepos+lines<0) return NULL;
  diag->linepos+=lines;
  return diag;
}

Dialog* diag_load(FILE* f) {
  if(!f) return NULL;
  Dialog* d=diag_ini();
  if(!d) return NULL;
  fscanf(f,"%d %d\n",&d->id,&d->nlines);
  d->linepos=0;
  d->lines=(char**)calloc(d->nlines,sizeof(char*));
  for(int i=0;i<d->nlines;i++) {
    d->lines[i]=(char *)calloc(MAX_DIALOG, sizeof(char));
    if(!d->lines[i]) {
      diag_free(d);
      return NULL;
    }
    if(!fgets(d->lines[i],MAX_DIALOG,f)) {
      diag_free(d);
      return NULL;
    }
    strtok(d->lines[i], "\n");
  }
  return d;
}































































































// void dman_setCounter(DialogMan * dman, int counter){
//     if(!dman) return;
//     dman->counter = counter;
//     return;
// }
// int dman_getCounter(DialogMan * dman){
//     if(!dman) return -1;
//     return dman->counter;
// }
//
//
// Bool dman_available(DialogMan * dman){
//     int i;
//    char * q = NULL;
//    char buf[BUFFER_SIZE];
//
//     if(!dman){
//         fprintf(stdout, "ERROR invalid pointer dman_available");
//         return false;
//     }
//     else if (dman->f == NULL){
//         fprintf(stdout,"ERROR no entity attached, execute dman_getDialog first");
//         return false;
//     }
//
//
//         do {
//                 q  = fgets(buf, BUFFER_SIZE, dman->f);
//                 if (atoi(buf) == dman->counter) {
//                        //CREO QUE ESTO ESTA BIEN PREGUNTAR
//                         return true;
//                 }
//                 if(atoi(buf) > dman->counter){
//                     fseek(dman->f, strlen(buf), SEEK_CUR); //DONT COMPLETELY TRUST THIS SOLUTION TILL ITS TESTED
//                     return false;
//                 }
//                   q = fgets(buf, BUFFER_SIZE, dman->f);
//
//         }while(q);
//         return false;
// }
//
//
//
//
//
//
// DialogMan* dman_getDialog(int entity_id){
//     DialogMan * dman;
//     char str[NUMBER_TXT];
//
//     sprintf(str, "%d.txt", entity_id);
//
//     dman = dman_ini();
//
//     dman->f = fopen(str, "r");
//
//     return dman;
// }
//
// char* dman_nextLine(DialogMan* dman){
//     char buf[BUFFER_SIZE];
//     char * q;
//     if(!dman) return 0;
//
//     if(dman_available(dman) == true){
//       q = fgets(buf, BUFFER_SIZE, dman->f);
//     }
//     return q;
// }
