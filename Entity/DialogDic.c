/*  DialogDic.c */

#include "DialogDic.h"

#define BUFFER_SIZE 1000
#define NUMBER_TXT 10

struct _DialogDic {
  Dialog** d;
  int size;
};

DialogDic* ddic_ini(FILE* df) {
  if(!df) return NULL;
  DialogDic* ddic = (DialogDic*)calloc(1, sizeof(DialogDic));
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

void ddic_free(DialogDic* ddic) {
  if(!ddic) return;
  for(int i=0;i<ddic->size;i++) {
    diag_free(ddic->d[i]);
  }
  free(ddic->d);
  free(ddic);
  return;
}

Dialog* ddic_copy(DialogDic* ddic) {
  if(!ddic) return NULL;
  DialogDic* dd=(DialogDic*)calloc(1, sizeof(DialogDic));
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
  dd->size=ddic->size;
  return dd;
}

Dialog* ddic_lookup(DialogDic* ddic, int did) {
  if(!ddic) return NULL;
  for(int i=0;i<ddic->size;i++) {
    if(diag_getId(ddic->d[i])==did) return diag_copy(ddic->d[i]);
  }
  return NULL;
}
