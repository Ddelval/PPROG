/*  DialogMan.c */

#include "DialogMan.h"

#define BUFFER_SIZE 1000
#define NUMBER_TXT 10

struct _DialogMan {
  Dialog** d;
  int size;
  int position;
};

DialogMan* dman_ini(FILE* df) {
  if(!df) return NULL;
  DialogMan* dman = (DialogMan*)calloc(1, sizeof(DialogMan));
  if(!dman) return NULL;
  int si;
  fscanf(df,"%d", &si);
  dman->size=si;
  dman->d=(Dialog**)calloc(si, sizeof(Dialog*));
  if(!dman->d){
    free(dman);
    return NULL;
  }
  for(int i=0;i<si;i++) {
    dman->d[i]=diag_load(df);
    if(!dman->d[i]) {
      dman_free(dman);
      return NULL;
    }
  }
  return dman;
}

void dman_free(DialogMan* dman) {
  if(!dman) return;
  for(int i=0;i<dman->size;i++) {
    diag_free(dman->d[i]);
  }
  free(dman->d);
  free(dman);
  return;
}

DialogMan* dman_copy(DialogMan* dman) {
  if(!dman) return NULL;
  DialogMan* dd=(DialogMan*)calloc(1, sizeof(DialogMan));
  memcpy(dd,dman,sizeof(DialogMan));
  if(!dd) return NULL;
  dd->d=(Dialog**)calloc(dman->size, sizeof(Dialog*));
  if(!dd->d) {
    dman_free(dd);
    return NULL;
  }
  for(int i=0;i<dman->size;++i) {
    dd->d[i]=diag_copy(dman->d[i]);
    if(!dd->d[i]) {
      for(int j=0;j<i;++j) {
        diag_free(dd->d[j]);
      }
      free(dd->d);
      dman_free(dd);
      return NULL;
    }
  }
  return dd;
}
char* dman_getLine(DialogMan* dic){
  if(!dic) return NULL;
  return diag_getNext(dic->d[dic->position]);
}

Dialog* dman_lookup(DialogMan* dman, int did) {
  if(!dman) return NULL;
  for(int i=0;i<dman->size;i++) {
    if(diag_getId(dman->d[i])==did) return diag_copy(dman->d[i]);
  }
  return NULL;
}

DialogMan* dman_advance(DialogMan* dd){
  if(!dd)return NULL;
  dd->position++;
  if(dd->position>=dd->size){
    dd->position--;
    return NULL;
  }
  return dd;
}

DialogMan* dman_setDialog(DialogMan* dman, int did){
  for(int i=0;i<dman->size;i++) {
    if(diag_getId(dman->d[i])==did)dman->position=i;
  }
  return dman;
}
DialogMan* dman_resetDialog(DialogMan* dd) {
  if(!dd) return NULL;
  if(!diag_setLine(dd->d[dd->position], 0)) return NULL;
  return dd;
  return NULL;
}
