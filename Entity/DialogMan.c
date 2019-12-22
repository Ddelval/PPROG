/*  DialogMan.c */

#include "DialogMan.h"

#define BUFFER_SIZE 1000
#define NUMBER_TXT 10

struct _DialogMan {
  Dialog** d;
  int size;
  int position
};

DialogMan* ddic_ini(FILE* df) {
  if(!df) return NULL;
  DialogMan* ddic = (DialogMan*)calloc(1, sizeof(DialogMan));
  if(!ddic) return NULL;
  int si;
  fscanf(df,"%d", &si);
  ddic->size=si;
  ddic->d=(Dialog**)calloc(si, sizeof(Dialog*));
  if(!ddic->d){
    free(ddic);
    return NULL;
  }
  for(int i=0;i<si;i++) {
    ddic->d[i]=diag_load(df);
    if(!ddic->d[i]) {
      ddic_free(ddic);
      return NULL;
    }
  }
  return ddic;
}

void ddic_free(DialogMan* ddic) {
  if(!ddic) return;
  for(int i=0;i<ddic->size;i++) {
    diag_free(ddic->d[i]);
  }
  free(ddic->d);
  free(ddic);
  return;
}

DialogMan* ddic_copy(DialogMan* ddic) {
  if(!ddic) return NULL;
  DialogMan* dd=(DialogMan*)calloc(1, sizeof(DialogMan));
  memcpy(dd,ddic,sizeof(DialogMan));
  if(!dd) return NULL;
  dd->d=(Dialog**)calloc(ddic->size, sizeof(Dialog*));
  if(!dd->d) {
    ddic_free(dd);
    return NULL;
  }
  for(int i=0;i<ddic->size;++i) {
    dd->d[i]=diag_copy(ddic->d[i]);
    if(!dd->d[i]) {
      for(int j=0;j<i;++j) {
        diag_free(dd->d[j]);
      }
      free(dd->d);
      ddic_free(dd);
      return NULL;
    }
  }
  return dd;
}
char* ddic_getLine(DialogMan* dic){
  if(!dic) return NULL;
  return diag_getNext(dic->d[dic->position]);
}

Dialog* ddic_lookup(DialogMan* ddic, int did) {
  if(!ddic) return NULL;
  for(int i=0;i<ddic->size;i++) {
    if(diag_getId(ddic->d[i])==did) return diag_copy(ddic->d[i]);
  }
  return NULL;
}

DialogMan* ddic_advance(DialogMan* dd){
  if(!dd)return NULL;
  dd->position++;
  if(dd->position>=dd->size){
    dd->position--;
    return NULL;
  }
  return dd;
}

DialogMan* ddic_setDialog(DialogMan* ddic, int did){
  for(int i=0;i<ddic->size;i++) {
    if(diag_getId(ddic->d[i])==did)ddic->position=i;
  }
  return ddic;
}
DialogMan* ddic_resetDialog(DialogMan* dd) {
  if(!dd) return NULL;
  if(!diag_setLine(dd->d[dd->position], 0)) return NULL;
  return dd;
  return NULL;
}
