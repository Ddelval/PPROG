/*  Dialog.c  */

#include "Dialog.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

char* diag_getNext(Dialog* diag) {
  if(!diag) return NULL;

  char* c=strdup(diag->lines[diag->linepos]);
  diag->linepos++;
  return c;
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