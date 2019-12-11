
#ifndef FONT_CATALOG_H
#define FONT_CATALOG_H
#include "Font.h"
#define FSIZE 3
typedef enum {M4,M6,M8} fname;

void fcat_free();
Font* fcat_lookup(fname f);

#endif